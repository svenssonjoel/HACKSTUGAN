/*
    Copyright 2021 Joel Svensson	svenssonjoel@yahoo.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "chprintf.h"

#include "stm32_tim.h" /* platform specific */
#include "stm32_rcc.h"

/* chibios messages are large enough to hold a pointer. 
   so if you want store more data in the message you need 
   to put that data elsewhere and then point to it. */

typedef struct {  /* message type */ 
  uint32_t tick;
}timer_tick_t;


/***********/
/* MAILBOX */

#define MAX_MESSAGES 64

//static mailbox_t mb; /* mailbox struct */
msg_t box_contents[MAX_MESSAGES]; /* mailbox storage */
MAILBOX_DECL(mb, box_contents, MAX_MESSAGES);


timer_tick_t tick_storage[MAX_MESSAGES];
MEMORYPOOL_DECL(tick_pool,MAX_MESSAGES, PORT_NATURAL_ALIGN, NULL);

static int send_mail(timer_tick_t t);

/*********************************/
/* Low level timer setup attempt */

stm32_tim_t *tim5 = NULL;

void setup_timer(void) {

  
  rccEnableTIM5(true);
  rccResetTIM5();
  
  nvicEnableVector(STM32_TIM5_NUMBER, STM32_GPT_TIM5_IRQ_PRIORITY); /* use GPT level prio */

  
  tim5 = STM32_TIM5;  /* gives direct access to the tim5 registers */

  /*
    typedef struct {
    volatile uint32_t     CR1;      - Control register 1
    volatile uint32_t     CR2;      - Control register 2
    volatile uint32_t     SMCR;     - Slave mode control register
    volatile uint32_t     DIER;     - DMA/Interrupt enable register
    volatile uint32_t     SR;       - Status register
    volatile uint32_t     EGR;      - Event generation register
    volatile uint32_t     CCMR1;    - Capture/Compare mode register 1
    volatile uint32_t     CCMR2;    - Capture/Compare mode register 2
    volatile uint32_t     CCER;     - Capture/Compare enable register
    volatile uint32_t     CNT;      - Count register.
    volatile uint32_t     PSC;      - Prescaler (1 - 65535).
    volatile uint32_t     ARR;      - Auto reload register.
    volatile uint32_t     RCR;      
    volatile uint32_t     CCR[4];   - Compare/Capture registers.
    volatile uint32_t     BDTR;
    volatile uint32_t     DCR;      - DMA control register
    volatile uint32_t     DMAR;     - DMA Address for full transfer
    volatile uint32_t     OR;       - Option register.
    volatile uint32_t     CCMR3;    - Capture/compare mode register 3 
    volatile uint32_t     CCXR[2];
    } stm32_tim_t;
  */

  tim5->PSC = 0xFFFF;     // counter rate is input_clock / (0xFFFF+1)
  tim5->ARR = 0xFFFFFFFF; // Value when counter should flip to zero.

  tim5->CCR[0] = 0xFFFFFFFF; /* init compare values */ 
  tim5->CCR[1] = 0xFFFFFFFF;
  tim5->CCR[2] = 0xFFFFFFFF;
  tim5->CCR[3] = 0xFFFFFFFF;

  tim5->CCER |= 0x1; /* activate compare on ccr channel 1 */
  tim5->DIER |= 0x2; 
  
  tim5->CNT = 0;
  tim5->EGR = 0x1; // Update event (Makes all the configurations stick)
  tim5->CR1 = 0x1; // enable

}
 
OSAL_IRQ_HANDLER(STM32_TIM5_HANDLER) {
  OSAL_IRQ_PROLOGUE();
  
  uint32_t sr = tim5->SR;
  sr &= tim5->DIER & STM32_TIM_DIER_IRQ_MASK;
  tim5->SR = ~sr;  

  timer_tick_t t; 
  t.tick = tim5->CNT;

  osalSysLockFromISR();
  send_mail(t);
  osalSysUnlockFromISR();
  
  OSAL_IRQ_EPILOGUE();
}


void set_ccr_tim5(int ix, uint32_t c_value) {
  if (ix >= 0 && ix <= 3) {
    tim5->CCR[ix] = c_value;
  }
}


/* Quick-hack led stuff */ 
void led_init(void) {
  palSetPadMode(GPIOD,
		13,
		PAL_MODE_OUTPUT_PUSHPULL |
		PAL_STM32_OSPEED_HIGHEST);
}

void led_set(int led, int value) {

  (void)led; /* ignore this as a hack */

  palWritePad(GPIOD, 13, value);
}

 
static int send_mail(timer_tick_t t) {
 
  /* will be called from inside of the timer interrupt */
  
  int r = 1; /*success*/
  timer_tick_t *m = (timer_tick_t*)chPoolAllocI(&tick_pool);
  
  if (m) { 
    *m = t;
    
    msg_t msg_val = chMBPostI(&mb, (msg_t)m);
    if (msg_val != MSG_OK) {  /* failed to send */
      chPoolFree(&tick_pool, m);
      r = 0;
    }
  }
 
  return r;
}


bool block_mail(timer_tick_t *t) {

  bool r = true;
  msg_t msg_val;

  int m = chMBFetchTimeout(&mb, &msg_val, TIME_INFINITE);

  if (m == MSG_OK) {
    *t = *(timer_tick_t*)msg_val;

    chPoolFree(&tick_pool, msg_val); /* free the pool allocated pointer */
  } else {
    r = false;
  }
  return r;
}


/*****************/ 
/* TICKER THREAD */

static THD_WORKING_AREA(thread_wa, 1024); /* name, size */
static thread_t *thread;

static THD_FUNCTION(tick_thread, arg) {
 chRegSetThreadName("tick");
 (void) arg;


 chprintf((BaseSequentialStream *)&SDU1, "Entering tick_thread\r\n"); 

 setup_timer();  /* setup timer for continuous runing compare mode */
 set_ccr_tim5(0, 5500);
 
 while (1) {
   chprintf((BaseSequentialStream *)&SDU1, "Waiting for mail\r\n"); 
   timer_tick_t t;
   bool r = block_mail(&t);
   if (r) { 
     chprintf((BaseSequentialStream *)&SDU1, "Got mail: %u\r\n", t.tick);
   } else {
     chprintf((BaseSequentialStream *)&SDU1, "Got mail: Error\r\n", t.tick);
   }
   set_ccr_tim5(0, t.tick + 5000);
 }
}


int main(void) {
  halInit();
  chSysInit();
  
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(500);

  /* Initialize the memory pool for tick messages */
  chPoolLoadArray(&tick_pool, tick_storage, MAX_MESSAGES);
  
  /* initialize mailbox */
  chMBObjectInit(&mb, box_contents, MAX_MESSAGES);


  /* Give the user some time to hook up a usb cable 
     or to connect a terminal */ 
  chThdSleepMilliseconds(2000);

  
  /* start the ssm tick thread */
  thread = chThdCreateStatic(&thread_wa, sizeof(thread_wa), /* working area */
  			     (tprio_t)(NORMALPRIO-20),     /* priority */
  			     tick_thread,                  /* thread function */
  			     NULL);                        /* argument */
  
  while(true) {

    chprintf((BaseSequentialStream *)&SDU1, "Hello world: %u \r\n", tim5->CNT); 
    chThdSleepMilliseconds(500);
  }

  return 0; //unreachable
}


extern crate sdl2;
extern crate rand;

use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::rect::Point;

use rand::prelude::*;
use std::convert::TryInto;
use std::time::Duration;
const WIDTH : u32 = 640;
const HEIGHT : u32 = 480;

fn main() {
    let ctx = sdl2::init().unwrap();
    let vid = ctx.video().unwrap();

    let mut far = [Point::new(0,0); 100];
    let mut mid = [Point::new(0,0); 100];
    let mut near =  [Point::new(0,0); 100];

    let mut counter = 0; 
    
    let mut rng = thread_rng();
    for i in 0..100 {
	far[i].x = rng.gen_range(0,WIDTH).try_into().unwrap();
	far[i].y = rng.gen_range(0,HEIGHT).try_into().unwrap();

	mid[i].x = rng.gen_range(0,WIDTH).try_into().unwrap();
	mid[i].y = rng.gen_range(0,HEIGHT).try_into().unwrap();

	near[i].x = rng.gen_range(0,WIDTH).try_into().unwrap();
	near[i].y = rng.gen_range(0,HEIGHT).try_into().unwrap();
    }

    let w = vid.window("SDL WINDOW", WIDTH, HEIGHT)
	.position_centered()
	.build()
	.unwrap();

    let mut canvas = w.into_canvas().build().unwrap();

    canvas.set_draw_color(Color::RGB(0, 0, 0));
    canvas.clear();
    canvas.present();

    let mut event_pump = ctx.event_pump().unwrap();
    
    'running: loop {

	for event in event_pump.poll_iter() {
	    match event {
		Event::Quit {..} |
		Event::KeyDown {keycode: Some(Keycode::Escape), ..} => {
		    break 'running;
		},
		_ => {}
	    }
	}

	
	for i in 0..100 {
	    near[i].x -= 1;
	    if near[i].x < 0 { near[i].x = WIDTH.try_into().unwrap() };
	}
	if counter%3 == 0 {
	    for i in 0..100 {
		mid[i].x -= 1;
		if mid[i].x < 0 { mid[i].x = WIDTH.try_into().unwrap() };
	    }
	}
	if counter%5 == 0 {
	    for i in 0..100 {
		far[i].x -= 1;
		if far[i].x < 0 { far[i].x = WIDTH.try_into().unwrap() };
	    }
	} 
	if counter == 10 {counter = 0}
	else { counter += 1}

	canvas.set_draw_color(Color::RGB(0,0,0));
	canvas.clear();
	
	canvas.set_draw_color(Color::RGB(100,100,100));
	for p in far.iter() {
	    canvas.draw_point(*p).unwrap();
	}

	canvas.set_draw_color(Color::RGB(160,160,160));
	for p in mid.iter() {
	    canvas.draw_point(*p).unwrap();
	}
	canvas.set_draw_color(Color::RGB(255,255,255));
	for p in near.iter() {
	    canvas.draw_point(*p).unwrap();
	}

	canvas.present();
	std::thread::sleep(Duration::new(0, 100000));
    }
    
}

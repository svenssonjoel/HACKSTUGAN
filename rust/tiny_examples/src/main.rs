

// This is a rust comment!

/*
   This is also a rust comment 
   but it is spanning multiple
   lines. 
*/


// ////////////////////////////////////////////////////////////
// Table of contents
// ////////////////////////////////////////////////////////////
/* 
  - size_of_different_things
  - for_loop_example
  - while_loop_example
  - main
*/






// ////////////////////////////////////////////////////////////
// Tiny Examples starts
// ////////////////////////////////////////////////////////////


// std::mem::size_of_val  returns the size of a value.
// The number of bytes used by the representation of the
// value in memory.
fn size_of_different_things() {

    let a = 10;
    let b : u8 = 10;
    let c : u16 = 10;
    let d : u32 = 10;
    let e : u64 = 10;

    println!("size of a is {}", std::mem::size_of_val(&a));
    println!("size of b is {}", std::mem::size_of_val(&b));
    println!("size of c is {}", std::mem::size_of_val(&c));
    println!("size of d is {}", std::mem::size_of_val(&d));
    println!("size of e is {}", std::mem::size_of_val(&e));
}



fn for_loop_example() {

    for a in 1..10 {
	println!("In this iteration of the loop, a is {}.", a);
    }
}


fn ranges() {

    let a = 1..3;

    println!("This is a range: {:?}.", a);

    let b = a.clone(); 
    
    for i in a {
	println!("Yay {}, I am looping over a range stored in a variable", i);
    }

    /* If you do not care about the iteration index */
    for _ in b {
	println!("I have no idea which index I am at!!");
    }
}

fn while_loop_example() {

    let mut a = 0;

    while a < 10 {

	println!("a is now {}", a);
	
	a += 1;
    }
}


fn main() {

    size_of_different_things();
    for_loop_example();
    ranges();
    while_loop_example();
}

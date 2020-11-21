

use std::io::{stdout, stdin, Read, Write};

fn main() {

    loop {

	let mut c = [0];

	match stdin().read(&mut c) {
	    Ok(n) => { 
		if n == 0 {
		    break;
		}
		while let Err(e) = stdout().write(&c) {
		    println!("Error: {}", e);
		}	  
	    }
	    Err(e) => println!("error {}", e),
	}
    }
}

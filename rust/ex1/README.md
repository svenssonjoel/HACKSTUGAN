
# Rust : Variables, functions and the build system


## Create packages using 'cargo'

The command:

```
> cargo new cargo_example
```

This creates a new directory called `cargo_example` with the following contents:

```
cargo_example/
├── Cargo.toml
└── src
    └── main.rs
```

That is, `cargo new` creates a new directory structure and some files
for a rust "package" (Project). To make an executable you can add
`--bin` so the complete command becomes:

```
> cargo new cargo_example --bin 
```

If the plan is to make a library you can add the `--lib` flag.

```
> cargo new cargo_example_lib --lib
```

This creates the following directory structure:

```
cargo_example_lib/
├── Cargo.toml
└── src
    └── lib.rs
```

The src directory will contain the rust source code, `.rs` files.

## The `Cargo.toml` file

The `Cargo.toml` file contains this:

```
[package]
name = "cargo_example"
version = "0.1.0"
authors = ["Joel Svensson <svenssonjoel@yahoo.se>"]
edition = "2018"

# See more keys and their definitions at https://doc.rust-lang.org/cargo/reference/manifest.html

[dependencies]
```

I think that over time the contents of these toml files will become
more interesting. The `dependencies` look interesting.

## Build and run using cargo

Go into the `cargo_example` directory and build:

```
> cd cargo_example
> cargo build
```

The command above creates a `target` directory with the following contents.

```
target
├── CACHEDIR.TAG
└── debug
    ├── build
    ├── cargo_example
    ├── cargo_example.d
    ├── deps
    │   ├── cargo_example-78cb9e0f216db304
    │   └── cargo_example-78cb9e0f216db304.d
    ├── examples
    └── incremental
        └── cargo_example-1wd1a7mlapvbo
            ├── s-fsx75b58aq-do71kb-1u3838xlvrn6k
            │   ├── 1w6xtpyfyobd8229.o
            │   ├── 33jp0o99zk7f8y70.o
            │   ├── 3dyvoq9elx9todeg.o
            │   ├── 3ingfrql64tu276j.o
            │   ├── 47ch0mphpdbubspf.o
            │   ├── 50jp8tgb8gpmphwn.o
            │   ├── 5goqwntyifhdc86y.o
            │   ├── 812wbx0o88h8j9.o
            │   ├── dep-graph.bin
            │   ├── query-cache.bin
            │   └── work-products.bin
            └── s-fsx75b58aq-do71kb.lock
``` 

The executable that has been built is `target/debug/cargo_example` and
it can be executed using `./target/debug/cargo_example` (under linux).
Another way to run the executable is to just type:

```
> cargo run
```

Running the program outputs "Hello, world!", so cargo created the hello
world example for us with no effort ;)



## Variables

Let's set up a package for some experimentation with variables.

```
> cargo new variables --bin
```

Then enter the directory created.

```
cd variables
```

Then open `src/main.rs` in your favourite editor. The contents
will look like this:

```
fn main() {
    println!("Hello, world!");
}
```

The `main` function is special in Rust, just like in C, in the way
that it is the entry point of your program.

`println!` prints strings to the terminal. The `!` exclamation mark looks
very odd, right? It seems to have to do with `println` being a macro.
Maybe we will get to macroes at some point in the future.

### Local variables

Variables within a function are declared using `let`, for example:

```
fn main() {

    let a = 5;
   	   	   
    println!("Hello, world!");
}
```

The code above defines a to be 5 and then prints "Hello, world!". Rust
does not like if you define a variable and not use it. So there will be
complaints when you build this. In the terminal type: 

```
> cargo build
```

and it will output something like:

```
...
warning: unused variable: `a`
 --> src/main.rs:3:9
  |
3 |     let a = 5;
  |         ^ help: if this is intentional, prefix it with an underscore: `_a`
  |
  = note: `#[warn(unused_variables)]` on by default

warning: 1 warning emitted

    Finished dev [unoptimized + debuginfo] target(s) in 0.23s
```

One way to use the value of `a` is to print it. So let's change
the program as follows:

```
fn main() {

    let a = 5;
   	   	   
    println!("The value of a is {}",a);
}
``` 

When builing this program `cargo build` will not complain. Running
the program with `cargo run` should output:

```
The value of a is 5
```

The value of a cannot be changed! So if you try the following,
you get loud complaints when building:

```
fn main() {

    let a = 5;

    a = 199;
    
    println!("The value of a is {}",a);
}
```

If you really want a variable that you can change the value of,
then it has to be defined as mutable. Like this:

```
fn main() {

    let mut a = 5;

    a = 199;
    
    println!("The value of a is {}",a);
}
```

### Types

When we did `let a = 5` we did not specify any particular type
for `a`. This is fine and Rust will come up with a type automatically.
But if we wanted it to be a specific type we would have to say so.

```
let a : u8 = 5;
```

The type is specified by adding `: <type>` after the variable.
basic types include: u8, u16, u32, u64, i8, i16, i32, i64, f32, f64.
`u` for unsigned, `i` for signed and `f` for float. The number after
the `u`, `i` or `f` is the number of bits.

example:

```
fn main() {

    let a : u8 = 5;
    
    println!("The value of a is {}",a);
}

``` 

### Global variables

Global constants are defined using the `const` keyword and the variable
name used should be upper case. When defining a global constant you must give
it a type.

example:

```
const B : u8 = 100;

fn main() {

    let a : u8 = 5;
    
    println!("The value of a is {}",a);
    println!("The value of B is {}",B);
}
```

Another way to define a global is to use the `static` keyword.

example:

```
const B : u8 = 100;

static C : u8 = 0;

fn main() {

    let a : u8 = 5;
    
    println!("The value of a is {}",a);
    println!("The value of B is {}",B);
    println!("The value of C is {}",C);
}
```

It also requires that the variable is upper case and that the type is specified.

A `static` global variable can be declared as mutable.

```
static mut C : u8 = 0;
```

But using a static mutable is dangerous!

```
const B : u8 = 100;

static mut C : u8 = 0;

fn main() {

    let a : u8 = 5;
    
    println!("The value of a is {}",a);
    println!("The value of B is {}",B);
    unsafe { 
	println!("The value of C is {}",C);
    
	C = 2;

	println!("The value of C is {}",C);
    }
}
``` 



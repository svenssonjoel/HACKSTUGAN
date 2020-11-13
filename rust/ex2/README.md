
# Rust: A first look at Functions


Let's create a new "workspace" for this experiment session.

```
> cargo new ex2
> cd ex2
```

Open `src/main.rs` in your favourite editor.

The contents of `src/main.rs` should look like this:

```
fn main() {
    println!("Hello, world!");
}
```

## Our first function

Let's write a function that prints a greeting. The function takes
no arguments and returns nothing.

```
fn print_greeting()
{
    println!("Hello, world!!!");
}
``` 

Change the `main` function like this:

```
fn main() {
    print_greeting();
}
```

Build and run in the terminal:

```
> cargo build
> cargo run
Hello, world!!!
```


# Functions of one and more arguments

A function with one `i32` argument looks like this:

```
fn print_value(a : i32)
{
    println!("The value is {}", a);
}
```

This function just prints the value of the argument `a`. These
arguments are passed by value.

Similarly a two arguments function looks like this:

```
fn print_sum(a : i32, b : i32)
{
    println!("The sum is {}", a + b);
}
```

This function takes two `i32` arguments and print their sum.

Augment the main function as follows:

```
fn main() {
    print_greeting();
    print_value(199);
    print_sum(10, 5);
}
```

Then build and run.

```
> cargo build
> cargo run
Hello, world!!!
The value is 199
The sum is 15
```

## Functions that return values

The syntax for a function that returns a value looks like this:

```
fn calc_sum(a : i32, b : i32) -> i32
{
    a + b
}
```

The `-> i32` part means that the `calc_sum` function returns an `i32`.
Also note that there is no `return` statement as in C. instead the expression
`a + b` that is occuring as the last and only line of the function is returned.

So, another way to write the same function that clarifies this a bit
is as follows:

```
fn calc_sum(a : i32, b : i32) -> i32
{
    let c = a + b;
    c
}
``` 
Here the last line contains just `c` and note that it is not ended with a semicolon.

Now change main to call the `calc_sum` function:

```
fn main() {
    print_greeting();
    print_value(199);
    print_sum(10, 5);
    println!("The sum of 4 and 5 is {}", calc_sum(4, 5));
}
```

Then build and run:

```
> cargo build
> cargo run
Hello, world!!!
The value is 199
The sum is 15
The sum of 4 and 5 is 9

## Functions that return two values

To return two values (or more) from a function we can use a tuple. A tuple
is a collection of different values, that can also be of different types.
First an example that computer both the sum and the difference.

```
fn sum_diff(a : i32, b : i32) -> (i32, i32)
{
    let sum = a + b;
    let diff = a - b;
    (sum, diff)
}
```

We can add the following to the `main` function.

```
    let a = sum_diff(10, 5);
    println!("The sum is {} and the diff is {}", a.0, a.1);
```

Here `a` is defined to be the result of `sum_diff` applied to 10 and 5.
So `a` is a tuple, it is a pair of two `i32` values. To access
the first and the second of these values index into the tuple with the
`.x` syntax, for example `a.0` and `a.1`.

This is what `main` looks like now:

```
fn main() {
    print_greeting();
    print_value(199);
    print_sum(10, 5);
    println!("The sum of 4 and 5 is {}", calc_sum(4, 5));
    let a = sum_diff(10, 5);
    println!("The sum is {} and the diff is {}", a.0, a.1);
}

```

And running it has the following result:

```
Hello, world!!!
The value is 199
The sum is 15
The sum of 4 and 5 is 9
The sum is 15 and the diff is 5
```

Let's try to return a tuple (pair) where the two elements
are of different type.

```
fn sum_cmp(a : i32, b : i32) -> (i32, bool)
{
    let sum = a + b;
    let cmp = a > b;
    (sum, cmp)
}
```

`sum_cmp` returns an `(i32, bool)` pair.

Now add the following to main:

```
    let b = sum_cmp(10, 5);
    println!("The sum is {} and 10 > 5 is {}", b.0, b.1);		
```

Here `b.0` is an `i32` and `b.1` is a `bool`.

Update `main` as follows:

```
fn main() {
    print_greeting();
    print_value(199);
    print_sum(10, 5);
    println!("The sum of 4 and 5 is {}", calc_sum(4, 5));
    let a = sum_diff(10, 5);
    println!("The sum is {} and the diff is {}", a.0, a.1);
    let b = sum_cmp(10, 5);
    println!("The sum is {} and 10 > 5 is {}", b.0, b.1);
}
``` 

And run it!

```
> cargo run
Hello, world!!!
The value is 199
The sum is 15
The sum of 4 and 5 is 9
The sum is 15 and the diff is 5
The sum is 15 and 10 > 5 is true
```







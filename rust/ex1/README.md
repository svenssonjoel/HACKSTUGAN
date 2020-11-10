
# Rust : Variables, functions and the build system


## Build and package management using 'cargo'

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
cargo new cargo_example --bin 
```

If the plan is to make a library you can add the `--lib` flag.

```
cargo new cargo_example_lib --lib
```

This creates the following directory structure:

```
cargo_example_lib/
├── Cargo.toml
└── src
    └── lib.rs
```

## The `Cargo.toml` file
# Ember

**Elegant Chaos™ Build Tool** — A spicy alternative to `make` for C projects.

## Features
- Assembly-inspired `.ember {}` syntax
- Supports tasks (`clean`, `install`, `run`, you name it)
- *NOT a Makefile transpiler, it has dignity*

## Install
```bash
make                  # Build Ember (if you must)
sudo ./ember install  # Symlink to /usr/local/bin
```

## Usage
Inside a project with an `Emberfile`:
```bash
ember               # Build project
ember run           # Run task "run"
ember clean         # Clean object files
```

## Example project structure
```
project/
├── Emberfile
├── src/
|   ├── code.c
|   └── main.c
├── include/
|   └── code.h
└── build/
```

## Example `Emberfile`
```ember
.target {
    main
}

.src {
    src/main.c
    src/code.c
}

.flags {
    -Wall
    -Wextra
    -O2
}

.include {
    include
}

.libs {
    -lm
}

.out {
    ./build/
}

.languages {
    c
}

.task {
    run: ./build/main;
    clean: rm -f ./build/*
}

.pre {
    echo "Compiling ember..."
}

.post {
    echo "Building complete"
}

```

## Markers
When working with `ember` you will see colored output in the terminal, starting with square brackets and a letter. For example:

```
[c] gcc -c src/main.c -o ./build//main.o -Iinclude -Wall -Wextra -O2
```

Each marker tells you what ember is doing at that time.

- [C], compiling
- [L], linking
- [B], build succeeded
- [S], skipping (pre)compiled code
- [T], executing task
- [X], error
- [D], creating output directory if it doesn't exist yet

## Philosophy
"Documentation is like a safety net — and Ember walks the wire without it."
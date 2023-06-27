# Assignment 4 - The Game of Life

The program **life.c** will be implementing the **Two-dimensional (2-D) grid of cells that represents a universe.**

## Files In The Repository
1. Makefile
2. DESIGN.pdf
3. WRITEUP.pdf
4. README.md
5. universe.h
6. universe.c
7. life.c

# Make commands

```
$ make clean
```
This command will make the repository clean by deleting the object file and executable file of **life.c**.


```
$ make format
```

This command will format C file into clang format.

```
$ make
```

This command will just compile other C files and make the executable and object file for **life.c**.

## Running the program

```
$ make
```
1. This command will just compile and make the executable and object file for **life.c**.

```
$ ./life -flag
```

2. flags = t,s,h,n,i and o you can pass one or many flags together.

### Flags

-t
: Create your universe as a toroidal.

-s
: Silent - do not use animate the evolution

-n {number}
: Number of generations [default: 100]

-i {file}
: Input file [default: stdin]

-o {file}
: Output file [default: stdout]


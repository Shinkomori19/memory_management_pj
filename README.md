# Memory Management Project

## Tatsuhiko Araki, Shin Komori

We implemented memory management functions(malloc and free) by ourselvs.

## Work distribution

Tatsuhiko worked on:

- getmem.c(100%)
- bench.c(100%)
- mem_impl.h(100%)
- Makefile(50%)

Shin worked on:

- freemem.c(100%)
- mem.h(100%)
- mem_utils.c(100%)
- Makefile(50%)
- Readme(100%)
- run.sh(100%)

## How we organized heap data structure and the algorithms behind it

The heap list is sorted by block header address in ascending order. The header of a block has the information of its size and a pointer to the next block in the list.Each block in the list are not in contact with each other.

When freemem() is called, the program scans free memory until the location of the specified block is found at that address. It then checks to see if the address of the specified block matches that of two adjacent blocks and determines if the specified block needs to be chained with the adjacent block. If no chaining is required, the block is simply inserted into the list at that point.

When 　 the function getmem() is called with a given amount of memory, it goes through the list from the beginning until it finds a block that is large enough.
The selected block should also have enough memory remaining to be larger than the threshold set by the program. When a block is selected, the user receives the top portion of that block and the remaining memory remains free. If it cannot find a block with enough space, the program receives a block large enough from the lower system. The block is then split in two, the user receives the upper portion, and the remaining lower portion is returned to free memory. The location of the new block depends on the address.

## Additional Features

Current implementations simplify memory management without further optimization. It has the following properties.

- Every time getmem starts at the beginning of the free list when searching for blocks of the appropriate size.
- Each block in the free list contains only a header.

## Summary of Results

Tried with running 10000 trials, CPU TIME needed was around 0.0013 seconds in average. The amount of bytes used was around 19070

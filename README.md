# f22-allocator

## About The Project

Memory allocator created for CMPSC473, Operating Systems, as project junior year using C in Visual Studio Code. For this project, I implement a memory allocator, which supports Buddy Allocation and Slab Allocation schemes, writing my own malloc() and free() functions, which are analogous to the C library's malloc() and free(). The memory allocator allocates memory only within the given starting address and size provided. The input file has a list of memory allocation requests and memory free requests, and the generated output file has the result of the allocations.

## Input File Format

Malloc: `[name] [NumOps] M [Size]`

Free: `[name] [Index] F 0`


## Output File Format

Malloc: `Start of Chunk [name] is: [address]`

Free: `freed object [name] at [address]`


## Built With

* Visual Studio Code: https://code.visualstudio.com/

## Contact

Matthew Kelleher:
* Email: matthew.kelleher10@comcast.net
* LinkedIn: https://www.linkedin.com/in/matthew-k-868178128/

Project Link: https://github.com/matthewlit/f22-mem_allocator

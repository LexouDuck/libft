# libccc
---
A customizable cross-platform standard library for C

<a href="https://github.com/LexouDuck/libccc/actions">
	<img src="https://github.com/LexouDuck/libccc/workflows/CI+CD/badge.svg" />
</a>



### The Project
---
The objective is to have one simple, well-documented, efficient open-source implementation of the C standard library, but with easier to use functions and types than what is offered by the ISO standards - mostly by consistently handling undefined behaviors, and handling edge cases in a consistent, coherent way, so as to have code that is as little machine-specific as possible.

The first step to accomplishing this is to not use the native int/long/short types, which can have different storage size depending on the platform
('int' is defined as the fastest integer type for the given machine, typically this will be the CPU register size - so on a 64-bit machine, that'd be int64, on a 32-bit machine int32, and some old embedded systems you come across might have 16-bit ints as the machine's default storage size). So first of all, using the integer types defined in "stdint.h" ('int32_t', 'uint64_t', etc) is essential for any cross-platform C code, as it ensures consistent cross-platform overflow behaviors (Note that these aren't present on every platform though - sometimes we will have to settle for the 'uint_fastX_t' types).

The following categories/headers include the ISO standard library functions:
* _**memory**_: functions to manipulate memory directly (memcpy,memset,memchr,etc, but also extras like ptrarrlen,memdup,getbits)
* _**string**_: functions to manipulate strings (strcpy,strchr,strsub,etc but also extras such as strformat,strsplit,strmap)
* _**char**_: functions to handle ANSI/ASCII characters (islower,isalpha,toupper,etc)
* _**math**_: common mathematical functions operating on floating-point numbers (the implementations in this lib are fast-approximate versions, though by default LIBCONFIG_FAST_APPROX_MATH is defined as 0, so the functions are actually simple wrappers over the builtin FPU math functions)
* _**time**_: functions for handling times/dates/timezones/timespecs (handling of timezones is different from the stdlib 'global variable' tzset call)
* _**random**_: functions for simple pesudo-random number generation (many more functions than just the ISO rand/srand/rand_r functions)
* _**io**_: functions for reading and writing (from/to terminal, or from/to file(s) - wrapper functions over `unistd.h` and `stdio.h`)

Furthermore, there are other functions here that are not found in the ISO standard, particularly in the following categories:
* _**convert**_: functions to replace the wonky std libc atoi/itoa/atof/strtol functions with more understandable type-specific conversion functions
* _**color**_: functions manipulating several common color encodings, as well as other useful color-related functions (like RGB_to_HSL, etc)
* _**random**_: a set of simple functions for different methods of pseudo-random number generation
* _**stringarray**_: a set of functions used to manipulate string arrays (`char **`), where the top-level pointer array is terminated by a NULL pointer
* _**algebra**_: math functions for 2D/3D/4D vectors and matrices, as well as integrals and more
* _**complex**_: math functions for complex number operations, as well as quaternions
* _**stat**_: statistics & probabilities functions: int/float number array sort functions, median, standard deviation, etc
* _**list**_: linked list functions (using a simple list type which only stores the 'next' pointer)
* _**vlq**_: Variable-Length Quantity math functions, for variable-size "big ints".



### Why does this exist ?
---
What started as a necessary exercise for the 42 school quickly became a much more expansive project: whereas the 42 libft project only requires students to code a certain set of important memory/string/io functions, we decided to take it further. Whereas libft is originally meant as an educational exercise, in which a student learns a lot by "reinventing wheels", here the goal is to have a standard C library which is fully cross-platform, uniformized, which is configurable and customizable, obviously being open source (MIT license) and which offers more than the "bare minimum" of the ISO standard library of functions.



### Building
---
The Makefile simply builds a libccc.a library to link to your project. (eg: something like `gcc main.c -I./libccc/hdr/ -L./libccc/ -lft`)

You can also add this git repo as a "git submodule" to your own if you wish (this allows you to be up to date to the latest version at all times).

In general though, we recommend having the source code and compiling it yourself (as there are important customization flags in `./hdr/libccc.h` which change how the library is compiled). In particular:
- `LIBCONFIG_HANDLE_NULLPOINTERS`
	If 0, then libccc functions will always try to dereference (and usually do a segmentation fault) when given NULL pointer arguments.
	If 1 (this is the default), then all NULL pointer accesses will be avoided, and an appropriate return value (eg:`NULL`, `0`, sometimes `-1`) will be returned by any libccc function when given a NULL pointer.
- `LIBCONFIG_FAST_APPROX_MATH`
	If 0 (this is the default), the builtin FPU-call libc math functions will be used (eg: `__builtin_powf()`, etc)
	If 1, the libccc fast approximate math functions will be used (these can be quite unreliable, their purpose is to be faster in terms of execution time - the general precision error margin they are tested for is 0.0001)



### Testing
---
To ensure proper functionality of all these important functions in every edge case, a big testing program was implemented, featuring segfault handling and execution time comparison among other things.
You can test the libccc by running `make test`: this will compile and run the test suite program from the files found in the 'test' folder.



### Contributing and Continuous Integration
---
Check `CONTRIBUTING.md`, this file serves as a "contributions style guide". We were alone at first so we wrote this code quick and dirty, but it'll do some good for us to have a coherent commit style for the whole team, so we'll follow these guidelines from now on. You should do the same.

// TODO add description of coding style to `CONTRIBUTING.md`


Every time a new commit/push is done, the automated CI testing job is run, as defined in `.github/workflows/ci.yml` (see the [official github documentation for Actions and Workflows to learn more](https://help.github.com/en/actions/reference/workflow-syntax-for-github-actions)).

This CI job builds everything with a `make`, and runs the testing suite with a `make test`. It runs with all of the following configurations:
- Ubuntu (gcc and clang)
- MacOS (gcc and clang)
- Windows (mingw-gcc and clang)



### Build system
---
Cross-platform Makefile: builds a `libccc.a` library file to link against.
The make commands are:


##### Building

##### `make debug` or `make` or `make all`
- Build the `libccc.a` library file, in debug mode (with added flags: `-g -ggdb -D DEBUG=1`)

##### `make release`
- Build the `libccc.a` library file, in release mode (builds both static and dynamic library files for the given OSMODE, with added flags like `-O3`)

##### `make doc`
- Generate documentation from doxygen comments in the code to the ./doc/ folder (in several formats: html, latex, rtf, man)


##### Checking/CI

##### `make libccc_test`
- Builds a test software `libccc_test`, which runs test suites on every libccc function.

##### `make test`
- Builds `libccc_test` and runs it without any arguments (running all tests).

##### `make lint`
- Runs CPPcheck on all source files

##### `make preprocessed`
- Fills the `./obj/` folder with `.c` files in their preprocessed state - this can help debug certain code macros, etc


##### Deleting

##### `make clean`
- Delete all object files in `./obj/` folder.

##### `make rclean`
- Delete all object files in `./obj/` folder, and the folder itself.

##### `make fclean`
- Delete all object files in `./obj/` folder, as well as the executable and library files.

##### `make tclean`
- Delete the test executable file, if present.

##### `make re`
- Does a `make fclean` followed by a `make`

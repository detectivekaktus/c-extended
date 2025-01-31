# c-extended

C-extended is a header only library that contains all the necessary tools and boilerplate code that you would need to use when writing C. Idealy the library inspires the [nobuild](https://github.com/tsoding/nobuild) library and tries to become similar, yet more powerful and useful. The library is always going to be extended as needed. Not only the repository owner but also other people can contribute and add more functionality which they think must be the part of the library.

The current implementation goals for this library are:
- [X] Reading files.
- [X] Manage stack-allocated dynamic arrays.
- [X] Manage heap-allocated dynamic arrays.
- [X] Parse command line arguments.
- [X] Basic logging.
- [X] C-String manipulations.

## How to use it?
In order to use the library, you need to define `EXTEND_C` macro at the top of the file and include the `c-extended.h` header file from the repository and you'll have full access to the power of the library.

## Contribution
If you will to improve the library, you can easily do it after reading the [CONTRIBUTING.md](https://github.com/detectivekaktus/c-extended/blob/main/CONTRIBUTING.md) file which contains all the simple contribution rules you'd need to follow before opening a pull request.

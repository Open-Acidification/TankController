# Style Guide

This project follows [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

With some exceptions listed below
## Exceptions

### Variable Names

#### Constant Names

Variables declared as a constant should be fully capitalized with underscores placed where separation is necessary.

Examples:
- const byte ROWS = 4;
- const int WINDOW_SIZE = 10000;

### Function Names

If the function is meant to overload an exsisting function from a library, keep the same name

Examples:

- bool startsWith(const char *a, const char *b) { ... }
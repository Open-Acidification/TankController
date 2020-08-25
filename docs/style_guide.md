# Style Guide

This project follows Google's C++ style guide

https://google.github.io/styleguide/cppguide.html

With some exceptions and clarifiactions listed below
## Exceptions

### Variable Names

#### Constant Names

Variables declared as constant should be fully capitalized with underscores placed where separation is necessary.

Examples:
- const byte ROWS = 4;
- const int WINDOW_SIZE = 10000;

## Clarifications

### Function Names:

If a name starts with a non-capital character or ends with a capital character, use an underscore for separation if needed.

Examples:
- EEPROM_WriteDouble
- Get_pH_Test
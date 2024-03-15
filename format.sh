#!/bin/bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.ino" | \
  grep -v pybind11 | \
  grep -v "vendor/bundle/ruby" | \
  xargs clang-format -i --Werror

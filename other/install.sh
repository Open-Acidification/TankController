#!/bin/bash

set -x
if [ ${PWD##*/} != "other" ]; then
  cd other
fi
filePath=`bundle exec which arduino_library_location.rb`
export ARDUINO_CI=${filePath%/bin/*}/gems/arduino_ci-1.3.0
cp arduino_ci.rb $ARDUINO_CI/exe/arduino_ci.rb
cp cpp_library.rb $ARDUINO_CI/lib/arduino_ci/cpp_library.rb

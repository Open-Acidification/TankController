#!/bin/bash

if [ ${PWD##*/} != "GUI" ]; then
  cd GUI
fi
../scripts/installLibraries.sh
python3 -m pip install wxPython pybind11
filePath=`bundle exec which arduino_library_location.rb`
export ARDUINO_CI=${filePath%/bin/*}/gems/arduino_ci-1.3.0/cpp/arduino
make clean
(mkdir -p build; cd build; make -f ../Makefile)
python3 TankController.py

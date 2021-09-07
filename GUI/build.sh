#!/bin/bash

if [ ${PWD##*/} != "GUI" ]; then
  cd GUI
fi
../scripts/install_libraries.sh
python3 -m pip install wxPython pybind11
x=$(bundle exec which arduino_library_location.rb)
y=$(echo "${x%/bin/*}/bundler/gems/arduino_ci-*")
export ARDUINO_CI=${y}/cpp/arduino
export TC_PATH=$((cd ..; pwd))/src
make clean
(mkdir -p .build; cd .build; make -f ../Makefile)
python3 ./TankController.py

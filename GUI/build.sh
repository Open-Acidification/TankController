#!/bin/bash

# sudo apt install -y build-essential git ruby python3-dev python3-pip python-is-python3
# sudo gem install bundler
# git clone https://github.com/Open-Acidification/TankControllerLib.git
# bundle config --local path vendor/bundle
# bundle install

if [ ${PWD##*/} != "GUI" ]; then
  cd GUI
fi
../scripts/install_libraries.sh
python -m pip install wxPython pybind11
filePath=`bundle exec which arduino_library_location.rb`
export ARDUINO_CI=${filePath%/bin/*}/gems/arduino_ci-1.3.0/cpp/arduino
export TC_PATH=`(cd ..; pwd)`/src
make clean
(mkdir -p build; cd build; make -f ../Makefile)
python TankController.py

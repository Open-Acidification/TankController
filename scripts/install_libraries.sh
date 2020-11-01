#!/bin/bash

# set up directories
bundle exec ensure_arduino_installation.rb

# get custom version of LiquidCrystal
git clone https://github.com/Arduino-CI/LiquidCrystal.git
mv LiquidCrystal $(bundle exec arduino_library_location.rb)


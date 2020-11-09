#!/bin/bash

# set up directories
bundle exec ensure_arduino_installation.rb

# get custom version of LiquidCrystal
git clone https://github.com/Arduino-CI/LiquidCrystal.git
mv LiquidCrystal $(bundle exec arduino_library_location.rb)

# get custom version of RTClib
git clone https://github.com/Arduino-CI/RTClib.git
mv RTClib $(bundle exec arduino_library_location.rb)

# get custom version of Adafruit_BusIO
git clone https://github.com/Arduino-CI/Adafruit_BusIO.git
mv Adafruit_BusIO $(bundle exec arduino_library_location.rb)

# get custom version of Adafruit_MAX31865
git clone https://github.com/Arduino-CI/Adafruit_MAX31865.git
mv Adafruit_MAX31865 $(bundle exec arduino_library_location.rb)

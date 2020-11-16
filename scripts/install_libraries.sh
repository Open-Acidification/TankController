#!/bin/bash

# set up directories
bundle exec ensure_arduino_installation.rb
cd $(bundle exec arduino_library_location.rb)

# add missing libraries
if [ ! -d "./Adafruit BusIO" ] ; then
  git clone --depth 1 https://github.com/Arduino-CI/Adafruit_BusIO.git --branch "master" --single-branch "./Adafruit BusIO"
fi

if [ ! -d "./Adafruit_MAX31865" ] ; then
  git clone --depth 1 https://github.com/Arduino-CI/Adafruit_MAX31865.git --branch "master" --single-branch
fi

if [ ! -d "./Ethernet" ] ; then
  git clone --depth 1 https://github.com/Arduino-CI/Ethernet.git --branch "master" --single-branch
fi

if [ ! -d "./LiquidCrystal" ] ; then
  git clone --depth 1 https://github.com/Arduino-CI/LiquidCrystal.git --branch "master" --single-branch
fi

if [ ! -d "./RTClib" ] ; then
  git clone --depth 1 https://github.com/Arduino-CI/RTClib.git --branch "master" --single-branch
fi

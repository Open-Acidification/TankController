#!/bin/bash

sudo apt install -y \
  build-essential git ruby libgtk-3-dev \
  python3-dev python3-pip python-is-python3
sudo gem install bundler
git clone https://github.com/Open-Acidification/TankController.git

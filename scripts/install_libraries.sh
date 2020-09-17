# Standard libraries being used include:
#  > EEPROM, Ethernet, LiquidCrystal, SD, SPI, Wire
#
# Note these come pre-installed.

# These libraries are to be installed with the arduino-cli tool
# All libraries should include a version number
arduino-cli lib install "Adafruit MAX31865 library"@1.0.3
arduino-cli lib install ArduinoJson@6.14.1
arduino-cli lib install Ethernet@2.0.0
arduino-cli lib install Keypad@3.1.1
arduino-cli lib install PID@1.2.0
arduino-cli lib install RTClib@1.3.3

# These libraries are to be installed via git clone
# These libraries could not be found in the arduino-cli library list
mkdir -p libraries
cd libraries
git clone https://github.com/maniacbug/MemoryFree.git
git clone https://github.com/br3ttb/Arduino-PID-AutoTune-Library.git
mv Arduino-PID-AutoTune-Library/PID_AutoTune_v0 .
git clone https://github.com/PaulStoffregen/Time.git --branch v1.6
#git clone https://github.com/Glumgad/TimedAction.git
git clone https://github.com/sirleech/TrueRandom.git

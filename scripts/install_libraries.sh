# These libraries are to be installed with the arduino-cli tool
# All libraries should include a version number
arduino-cli lib install "Adafruit Keypad"@1.0.4
arduino-cli lib install "Adafruit MAX31865 library"@1.0.3
arduino-cli lib install ArduinoJson@6.14.1
arduino-cli lib install AsyncTaskLib@1.0.0
arduino-cli lib install Ethernet@2.0.0
arduino-cli lib install Keypad@3.1.1
arduino-cli lib install LiquidCrystal@1.0.7
arduino-cli lib install Marceau@0.0.1
arduino-cli lib install PID@1.2.0
arduino-cli lib install RTClib@1.3.3
arduino-cli lib install SD@1.2.4
arduino-cli lib install TimerOne@1.1.0

# These libraries are to be installed via git clone
# These libraries could not be found in the arduino-cli library list
cd libraries
git clone https://github.com/maniacbug/MemoryFree.git
git clone https://github.com/br3ttb/Arduino-PID-AutoTune-Library.git
mv Arduino-PID-AutoTune-Library/PID_AutoTune_v0 .
git clone https://github.com/PaulStoffregen/Time.git --branch v1.6
git clone https://github.com/Glumgad/TimedAction.git
git clone https://github.com/sirleech/TrueRandom.git

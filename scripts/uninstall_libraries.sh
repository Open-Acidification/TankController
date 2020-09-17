# These libraries are to be uninstalled with the arduino-cli tool
arduino-cli lib uninstall "Adafruit MAX31865 library"
arduino-cli lib uninstall ArduinoJson
arduino-cli lib uninstall Keypad
arduino-cli lib uninstall PID
arduino-cli lib uninstall RTClib

# Libraries outside arduino cli are to be uninstalled
rmdir -p libraries

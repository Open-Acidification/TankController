// https://pybind11.readthedocs.io/en/latest/basics.html

#define NUM_SERIAL_PORTS 1
#define EEPROM_SIZE 4096

#include <limits>
#include <string>
#include <vector>

#include "Arduino.h"
#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "EthernetServer_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "Serial_TC.h"
#include "TankControllerLib.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;
char lcdLine[20];

double eeprom(uint8_t index) {
  switch (index) {
    case 0:
      return EEPROM_TC::instance()->getPH();
    case 1:
      return EEPROM_TC::instance()->getTemp();
    case 2:
      return EEPROM_TC::instance()->getTankID();
    case 3:
      return EEPROM_TC::instance()->getCorrectedTemp();
    case 4:
      return EEPROM_TC::instance()->getKP();
    case 5:
      return EEPROM_TC::instance()->getKI();
    case 6:
      return EEPROM_TC::instance()->getKD();
    case 7:
      return EEPROM_TC::instance()->getMac();  // See issue #57 about this function
    case 8:
      return EEPROM_TC::instance()->getHeat();
    case 9:
      return EEPROM_TC::instance()->getAmplitude();
    case 10:
      return EEPROM_TC::instance()->getFrequency();
    case 11:
      return EEPROM_TC::instance()->getGranularity();
    case 12:
      return EEPROM_TC::instance()->getMaxDataAge();
    case 13:
      return EEPROM_TC::instance()->getPHSeriesSize();
    case 14:
      return EEPROM_TC::instance()->getPHSeriesPointer();
    case 15:
      return EEPROM_TC::instance()->getTempSeriesSize();
    case 16:
      return EEPROM_TC::instance()->getTempSeriesPointer();
    case 17:
      return EEPROM_TC::instance()->getPHInterval();
    case 18:
      return EEPROM_TC::instance()->getPHDelay();
    case 19:
      return EEPROM_TC::instance()->getTempInterval();
    case 20:
      return EEPROM_TC::instance()->getTempDelay();
    default:
      return std::numeric_limits<double>::quiet_NaN();
  }
}

void key(char key) {
  Keypad_TC::instance()->_getPuppet()->push_back(key);
}

const char *lcd(int index) {
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  String line = lines.at(index);
  int size = line.size();
  for (int i = 0; i < size; ++i) {
    if (line.at(i) < 32) {
      line.at(i) = '?';
    }
  }
  strncpy(lcdLine, line.c_str(), size);
  return lcdLine;
}

void loop() {
  TankControllerLib::instance()->loop();
}

string serial() {
  GodmodeState *state = GODMODE();
  string result = string(state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";
  return result;
}

void setup() {
  TankControllerLib::instance()->setup();
}

const char *version() {
  return TankControllerLib::instance()->version();
}

PYBIND11_MODULE(libTC, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring

  m.def("eeprom", &eeprom, "TankController EEPROM");
  m.def("key", &key, "TankController key");
  m.def("lcd", &lcd, "TankController LiquidCrystal");
  m.def("loop", &loop, "TankController loop");
  m.def("serial", &serial, "TankController serial");
  m.def("setup", &setup, "TankController setup");
  m.def("version", &version, "TankController version");
}

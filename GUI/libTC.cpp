// https://pybind11.readthedocs.io/en/latest/basics.html

#define NUM_SERIAL_PORTS 1

#include <string>
#include <vector>

#include "Arduino.h"
#include "Devices/DateTime_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/Serial_TC.h"
#include "TankControllerLib.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;
char lcdLine[20];

void setup() {
  TankControllerLib::instance()->setup();
}
void loop() {
  TankControllerLib::instance()->loop();
}
const char *version() {
  return TankControllerLib::instance()->version();
}
const char *lcd(int index) {
  std::vector<string> lines = LiquidCrystal_TC::instance()->getLines();
  string line = lines.at(index);
  int size = line.size();
  for (int i = 0; i < size; ++i) {
    if (line.at(i) < 32) {
      line.at(i) = '?';
    }
  }
  strncpy(lcdLine, line.c_str(), size);
  return lcdLine;
}
void key(char key) {
  Keypad_TC::instance()->_getPuppet()->push_back(key);
}

PYBIND11_MODULE(libTC, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring

  m.def("setup", &setup, "TankController setup");
  m.def("key", &key, "TankController key");
  m.def("lcd", &lcd, "TankController LiquidCrystal");
  m.def("loop", &loop, "TankController loop");
  m.def("version", &version, "TankController version");
}

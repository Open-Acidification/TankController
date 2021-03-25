// https://pybind11.readthedocs.io/en/latest/basics.html

#define NUM_SERIAL_PORTS 1
#define EEPROM_SIZE 4096

#include <sys/time.h>

// Arduino defines this and thread gets confused with it!
#undef yield

#include <chrono>
#include <ctime>
#include <limits>
#include <string>
#include <thread>
#include <vector>

#include "Arduino.h"
#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "Serial_TC.h"
#include "TankControllerLib.h"
#include "TempProbe_TC.h"
#include "UIState.h"
#include "pybind11/pybind11.h"

#define LED_PIN 13

namespace py = pybind11;
char lcdLine[20];

// function prototypes
void loop();

char *dateTime() {
  return DateTime_TC::now().as16CharacterString();
}

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
    case 21:
      return EEPROM_TC::instance()->getGoogleSheetInterval();
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

double getTemperature() {
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  return tempProbe->getRunningAverage();
}

bool led() {
  return digitalRead(LED_PIN);
}

void loop() {
  unsigned long millisecondsSinceEpoch =
      std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
  int msBehind = millisecondsSinceEpoch - millis();
  if (msBehind) {
    delay(msBehind);
  }
  TankControllerLib::instance()->loop();
}

string serial() {
  GodmodeState *state = GODMODE();
  string result = string(state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";
  return result;
}

void setTemperature(double value) {
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(value);
}

void setTime() {
  int timeDelta = 0;
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  DateTime_TC now(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour,
                  timeinfo->tm_min, timeinfo->tm_sec);
  unsigned long millisecondsSinceEpoch =
      std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
  timeDelta = millisecondsSinceEpoch - millis();
  delay(timeDelta);
  now.setAsCurrent();
}

void setup() {
  setTime();
  TankControllerLib::instance()->setup();
}

const char *version() {
  return TankControllerLib::instance()->version();
}

PYBIND11_MODULE(libTC, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring

  m.def("dateTime", &dateTime, "TankController DateTime");
  m.def("eeprom", &eeprom, "TankController EEPROM");
  m.def("key", &key, "TankController key");
  m.def("lcd", &lcd, "TankController LiquidCrystal");
  m.def("led", &led, "TankController LED pin value");
  m.def("loop", &loop, "TankController loop");
  m.def("serial", &serial, "TankController serial");
  m.def("setTemperature", &setTemperature, "TankController set actual tank temperature");
  m.def("setup", &setup, "TankController setup");
  m.def("version", &version, "TankController version");
}

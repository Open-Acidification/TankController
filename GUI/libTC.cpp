// https://pybind11.readthedocs.io/en/latest/basics.html

#define NUM_SERIAL_PORTS 2
#define EEPROM_SIZE 4096

#include <sys/time.h>

// Arduino defines this and thread gets confused with it!
#undef yield

#include <cassert>
#include <chrono>
#include <ctime>
#include <limits>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "Arduino.h"
#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PhProbe.h"
#include "SD_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"
#include "TempProbe_TC.h"
#include "UIState.h"
#include "pybind11/pybind11.h"

#define LED_PIN 13

namespace py = pybind11;
char lcdLine[17];
uint32_t msOffset = 0;

// function prototypes
void loop();
uint32_t millisecondsSinceEpoch();

char *dateTime() {
  return DateTime_TC::now().as16CharacterString();
}

float eeprom(uint8_t index) {
  switch (index) {
    case 0:
      return EEPROM_TC::instance()->getPh();
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
      return -1.0;  // not really a float!
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
      return EEPROM_TC::instance()->getPhSeriesSize();
    case 14:
      return EEPROM_TC::instance()->getPhSeriesPointer();
    case 15:
      return EEPROM_TC::instance()->getTempSeriesSize();
    case 16:
      return EEPROM_TC::instance()->getTempSeriesPointer();
    case 17:
      return EEPROM_TC::instance()->getPhInterval();
    case 18:
      return EEPROM_TC::instance()->getPhDelay();
    case 19:
      return EEPROM_TC::instance()->getTempInterval();
    case 20:
      return EEPROM_TC::instance()->getTempDelay();
    case 21:
      return EEPROM_TC::instance()->getGoogleSheetInterval();
    default:
      return std::numeric_limits<float>::quiet_NaN();
  }
}

void key(char key) {
  Keypad_TC::instance()->_getPuppet()->push_back(key);
}

const char *lcd(uint16_t index) {
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  String line = lines.at(index);
  uint16_t size = line.size();
  assert(size <= 16);
  for (size_t i = 0; i < size; ++i) {
    if (line.at(i) < 32) {
      line.at(i) = '?';
    }
  }
  strscpy(lcdLine, line.c_str(), size);
  assert(lcdLine[size] == 0);
  return lcdLine;
}

float getTemperature() {
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  return tempProbe->getRunningAverage();
}

void loop() {
  int32_t msBehind = millisecondsSinceEpoch() - millis() - msOffset;
  if (msBehind) {
    delay(msBehind);
  }
  TankController::instance()->loop();
}

uint32_t millisecondsSinceEpoch() {
  return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

float readPh() {
  return PHProbe::instance()->getPh();
}

bool readPin(uint16_t pin) {
  return digitalRead(pin);
}

string readSerial(uint16_t port) {
  GodmodeState *state = GODMODE();
  string result = string(state->serialPort[port].dataOut);
  state->serialPort[port].dataOut = "";
  return result;
}

string readSerial0() {
  return readSerial(0);
}

string readSerial1() {
  return readSerial(1);
}

void sdInit() {
}

string sdNextKey() {
  return string("");
}

string sdNextValue() {
  return string("");
}

void setTemperature(float value) {
  TempProbe_TC *tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(value);
}

void setTime() {
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  DateTime_TC now(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour,
                  timeinfo->tm_min, timeinfo->tm_sec);
  msOffset = millisecondsSinceEpoch() - millis();
  now.setAsCurrent();
}

void setup() {
  setTime();
  TankController::instance()->setup();
}

const char *version() {
  return TankController::instance()->version();
}

void writeSerial1(const char *data) {
  GODMODE()->serialPort[1].dataIn = data;      // the queue of data waiting to be read
  TankController::instance()->serialEvent1();  // fake interrupt
}

PYBIND11_MODULE(libTC, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring

  m.def("dateTime", &dateTime, "TankController DateTime");
  m.def("eeprom", &eeprom, "TankController EEPROM");
  m.def("key", &key, "TankController key");
  m.def("lcd", &lcd, "TankController LiquidCrystal");
  m.def("loop", &loop, "TankController loop");
  m.def("readPin", &readPin, "TankController read pin value");
  m.def("readSerial0", &readSerial0, "From TankController on serial port 0");
  m.def("readSerial1", &readSerial1, "From TankController on serial port 1");
  m.def("sdInit", &sdInit, "Reset the file system scan");
  m.def("sdNextKey", &sdNextKey, "Get the next file's full path");
  m.def("sdNextValue", &sdNextValue, "Get the next file's contents");
  m.def("setTemperature", &setTemperature, "TankController set actual tank temperature");
  m.def("setup", &setup, "TankController setup");
  m.def("version", &version, "TankController version");
  m.def("writeSerial1", &writeSerial1, "To TankController on serial port 1");
}

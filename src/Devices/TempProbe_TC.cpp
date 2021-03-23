#include "Devices/TempProbe_TC.h"

//  class instance variables
/**
 * static variable for singleton
 */
TempProbe_TC* TempProbe_TC::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
TempProbe_TC* TempProbe_TC::instance() {
  if (!_instance) {
    _instance = new TempProbe_TC();
  }
  return _instance;
}

//  instance methods
/**
 * constructor (private so clients use the singleton)
 */
TempProbe_TC::TempProbe_TC() {
  thermo.begin(MAX31865_3WIRE);  // Start pt100 temperature probe with 3 wire configuration
}

/**
 * getRunningAverage()
 *
 * Read the current temperature and return a running average
 */

double TempProbe_TC::getRunningAverage() {
  double temp = this->getRawTemperature() + correction;
  if (firstTime) {
    for (int i = 0; i < HISTORY_SIZE; ++i) {
      history[i] = temp;
    }
    firstTime = false;
  }
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
  history[historyIndex] = temp;
  double sum = 0.0;
  for (int i = 0; i < HISTORY_SIZE; ++i) {
    sum += history[i];
  }
  return sum / HISTORY_SIZE;
}

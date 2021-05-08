#include "Devices/TempProbe_TC.h"

#include "EEPROM_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"

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

void TempProbe_TC::reset() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

//  instance methods
/**
 * constructor (private so clients use the singleton)
 */
TempProbe_TC::TempProbe_TC() {
  // Start pt100 temperature probe with 3 wire configuration
  thermo.begin(MAX31865_3WIRE);

  // load offset from EEPROM
  correction = EEPROM_TC::instance()->getCorrectedTemp();
  if (isnan(correction)) {
    correction = 0;
    EEPROM_TC::instance()->setCorrectedTemp(correction);
  }
  serial("TempProbe_TC::TempProbe_TC() with temperature correction of %6.3f", correction);  // correction);
}

/**
 * getRunningAverage()
 *
 * Read the current temperature and return a running average
 */
double TempProbe_TC::getRunningAverage() {
  double temp = this->getRawTemperature();
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
  return sum / HISTORY_SIZE + correction;
}

/**
 * setCorrection(float value)
 *
 * Set a new value for the correction offset
 */
void TempProbe_TC::setCorrection(float value) {
  COUT("old = " << correction << "; new = " << value);
  if (value != correction) {
    correction = value;
    EEPROM_TC::instance()->setCorrectedTemp(correction);
    serialWithTime("Set temperature correction to %f", correction);
  }
}

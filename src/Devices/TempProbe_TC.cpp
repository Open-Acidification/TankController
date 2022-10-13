#include "Devices/TempProbe_TC.h"

#include "DateTime_TC.h"
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
  char buffer[10];
  dtostrf(correction, 5, 2, buffer);
  serial(F("Temperature probe with correction of %s"), buffer);
}

/**
 * getUncorrectedRunningAverage()
 *
 * Read the current temperature and return a running average.
 * Do this only once per second since device is unreliable beyond that.
 */
float TempProbe_TC::getUncorrectedRunningAverage() {
  uint32_t currentTime = millis();
  if (firstTime || lastTime + 1000 <= currentTime) {
    float temp = this->getRawTemperature();
    if (firstTime) {
      for (size_t i = 0; i < HISTORY_SIZE; ++i) {
        history[i] = temp;
      }
      firstTime = false;
    }
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    history[historyIndex] = temp;
    lastTime = currentTime;
  }
  float sum = 0.0;
  for (size_t i = 0; i < HISTORY_SIZE; ++i) {
    sum += history[i];
  }
  return sum / HISTORY_SIZE;
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
    serial(F("Set temperature correction to %i.%02i"), (int)correction, (int)(correction * 100 + 0.5) % 100);
  }
}

/**
 * clearCorrection(f)
 *
 * Sets the correction offset back to 0
 */
void TempProbe_TC::clearCorrection() {
  COUT("old = " << correction);
  if (correction != 0) {
    correction = 0.0;
    EEPROM_TC::instance()->setCorrectedTemp(correction);
    serial(F("Set temperature correction to %i.%02i"), (int)correction, (int)(correction * 100 + 0.5) % 100);
  }
}

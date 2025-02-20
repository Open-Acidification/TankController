#include "wrappers/ThermalProbe_TC.h"

#include "model/TC_util.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/Serial_TC.h"

//  class instance variables
/**
 * static variable for singleton
 */
ThermalProbe_TC* ThermalProbe_TC::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
ThermalProbe_TC* ThermalProbe_TC::instance() {
  if (!_instance) {
    _instance = new ThermalProbe_TC();
  }
  return _instance;
}

void ThermalProbe_TC::reset() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

//  instance methods
/**
 * constructor (private so clients use the singleton)
 */
ThermalProbe_TC::ThermalProbe_TC() {
  // Start pt100 temperature probe with 3 wire configuration
  thermo.begin(MAX31865_3WIRE);

  // load offset from EEPROM
  correction = EEPROM_TC::instance()->getThermalCorrection();
  if (isnan(correction)) {
    correction = 0;
    EEPROM_TC::instance()->setThermalCorrection(correction);
  }
  char buffer[10];
  floattostrf(correction, 5, 2, buffer, sizeof(buffer));
  serial(F("Temperature probe with correction of %s"), buffer);
}

/**
 * getRunningAverage()
 *
 * Return the corrected running average within the range of 00.00-99.99
 */
float ThermalProbe_TC::getRunningAverage() {
  float temperature = getUncorrectedRunningAverage() + correction;
  if (temperature < 0.0) {
    temperature = 0.0;
  } else if (99.99 < temperature) {
    temperature = 99.99;
  }
  return temperature;
}

/**
 * getUncorrectedRunningAverage()
 *
 * Read the current temperature and return a running average.
 * Do this only once per second since device is unreliable beyond that.
 */
float ThermalProbe_TC::getUncorrectedRunningAverage() {
  uint32_t currentTime = millis();
  if (firstTime || lastTime + 1000 <= currentTime) {
    float temperature = this->getRawTemperature();
    if (firstTime) {
      for (size_t i = 0; i < HISTORY_SIZE; ++i) {
        history[i] = temperature;
      }
      firstTime = false;
    }
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    history[historyIndex] = temperature;
    lastTime = currentTime;
    uncorrectedThermalSample.add(temperature);
  }
  float sum = 0.0;
  for (size_t i = 0; i < HISTORY_SIZE; ++i) {
    sum += history[i];
  }
  return sum / HISTORY_SIZE;
}

/**
 * @brief reset the thermal correction to zero
 *
 */
void ThermalProbe_TC::clearCorrection() {
  if (correction != 0) {
    correction = 0.0;
    EEPROM_TC::instance()->setThermalCorrection(correction);
    serial(F("Set temperature correction to %i.%02i"), (int)correction, (int)(correction * 100 + 0.5) % 100);
  }
}

/**
 * setCorrection(float value)
 *
 * Set a new value for the correction offset
 */
void ThermalProbe_TC::setCorrection(float value) {
  if (value != correction) {
    correction = value;
    EEPROM_TC::instance()->setThermalCorrection(correction);
    serial(F("Set temperature correction to %i.%02i"), (int)correction, (int)(correction * 100 + 0.5) % 100);
  }
}

/**
 * @brief get the recent average temperature
 *
 * @return float
 */
float ThermalProbe_TC::getSampleMean() {
  // This is not used for the Liquid Crystal display, so we don't restrict range to 0 to 99.99
  return uncorrectedThermalSample.average() + correction;
}

/**
 * @brief get the sample standard deviation for recent temperatures
 *
 * @return float
 */
float ThermalProbe_TC::getSampleStandardDeviation() {
  return uncorrectedThermalSample.unbiased_stdev();
}

/**
 * @brief discard the temperature sample to start a new sample
 *
 */
void ThermalProbe_TC::resetSample() {
  uncorrectedThermalSample.clear();
}

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
// set a temperature in the mock
void ThermalProbe_TC::setTemperature(float newTemp, bool clearCorrection, bool setHistory) {
  if (setHistory) {
    for (size_t i = 0; i < HISTORY_SIZE; ++i) {
      history[i] = newTemp;
    }
  }
  if (clearCorrection) {
    this->clearCorrection();
  }
  thermo.setTemperature(newTemp);
}
#endif

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "TC_util.h"
#include "TankController.h"
#include "TempProbe_TC.h"
#include "TemperatureCalibration.h"

unittest(test) {
  // with no correction, we have a temperature of 10.0
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(10.0);
  tempProbe->setCorrection(0.0);
  assertEqual(0.0, EEPROM_TC::instance()->getCorrectedTemp());
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }
  delay(1000);
  float temp = tempProbe->getRunningAverage();
  assertTrue(9.9 <= temp && temp <= 10.1);

  // set UI to TemperatureCalibration state
  TankController* tc = TankController::instance();
  assertFalse(tc->isInCalibration());
  TemperatureCalibration* test = new TemperatureCalibration(tc);
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());

  // UI sets actual temperature to 10.5
  test->setValue(10.5);

  // The new temperature should be 10.5
  temp = tempProbe->getRawTemperature();
  assertTrue(9.9 <= temp && temp <= 10.1);
  temp = tempProbe->getRunningAverage();
  assertTrue(10.49 <= temp && temp <= 10.51);

  // A new measured temperature should also be adjusted
  tempProbe->setTemperature(15.5);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(15.9 <= temp && temp <= 16.1);
  temp = EEPROM_TC::instance()->getCorrectedTemp();
  assertTrue(0.49 < temp && temp < 0.51);

  // test for issue #174
  tc->loop();
  tc->loop();
  test = new TemperatureCalibration(tc);
  tc->setNextState(test, true);
  test->setValue(16.0);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertEqual(16.0, temp);
}

unittest_main()

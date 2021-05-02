#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "TankControllerLib.h"
#include "TempProbe_TC.h"
#include "TemperatureCalibration.h"

unittest(foo) {
  // with no correction, we have a temperature of 10.0
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(10.0);
  tempProbe->setCorrection(0.0);
  assertEqual(0.0, EEPROM_TC::instance()->getCorrectedTemp());
  for (int i = 0; i < 100; ++i) {
    tempProbe->getRunningAverage();
  }
  double temp = tempProbe->getRunningAverage();
  assertTrue(9.9 <= temp && temp <= 10.1);

  // UI sets actual temperature to 10.5
  TankControllerLib* tc = TankControllerLib::instance();
  TemperatureCalibration* test = new TemperatureCalibration(tc);
  tc->setNextState(test, true);
  test->setValue(10.5);

  // The new temperature should be 10.5
  double temp = tempProbe->getRunningAverage();
  assertTrue(10.49 <= temp && temp <= 10.51);

  // A new measured temperature should also be adjusted
  tempProbe->setTemperature(15.5);
  for (int i = 0; i < 100; ++i) {
    tempProbe->getRunningAverage();
  }
  double temp = tempProbe->getRunningAverage();
  assertTrue(15.9 <= temp && temp <= 16.1);

  assertEqual(0.5, EEPROM_TC::instance()->getCorrectedTemp());
}

unittest_main()

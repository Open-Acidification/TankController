#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "TC_util.h"
#include "TankController.h"
#include "ThermalProbe_TC.h"
#include "TemperatureCalibration.h"

unittest(test) {
  GodmodeState* state = GODMODE();
  // with no correction, we have a temperature of 10.0
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->setTemperature(10.0, true, false);
  assertEqual(0.0, EEPROM_TC::instance()->getCorrectedTemp());
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  float temp = thermalProbe->getRunningAverage();
  assertTrue(9.9 <= temp && temp <= 10.1);

  // set UI to TemperatureCalibration state
  TankController* tc = TankController::instance();
  assertFalse(tc->isInCalibration());
  TemperatureCalibration* test = new TemperatureCalibration();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());

  // UI sets actual temperature to 10.5
  state->serialPort[0].dataOut = "";  // the history of data written
  test->setValue(10.5);
  assertEqual("Set temperature correction to 0.50\r\n", state->serialPort[0].dataOut);

  // The new temperature should be 10.5
  temp = thermalProbe->getRawTemperature();
  assertTrue(9.9 <= temp && temp <= 10.1);
  temp = thermalProbe->getRunningAverage();
  assertTrue(10.49 <= temp && temp <= 10.51);

  // A new measured temperature should also be adjusted
  thermalProbe->setTemperature(15.5, false, false);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  temp = thermalProbe->getRunningAverage();
  assertTrue(15.9 <= temp && temp <= 16.1);
  temp = EEPROM_TC::instance()->getCorrectedTemp();
  assertTrue(0.49 < temp && temp < 0.51);

  // test for https://github.com/Open-Acidification/TankController/issues/174
  tc->loop(false);
  tc->loop(false);  // second loop needed to set the next state
  test = new TemperatureCalibration();
  tc->setNextState(test, true);
  test->setValue(16.0);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  temp = thermalProbe->getRunningAverage();
  assertEqual(16.0, temp);
}

unittest_main()

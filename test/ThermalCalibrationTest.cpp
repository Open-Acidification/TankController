#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "TC_util.h"
#include "TankController.h"
#include "ThermalCalibration.h"
#include "ThermalProbe_TC.h"

unittest(test) {
  GodmodeState* state = GODMODE();
  // with no correction, we have a temperature of 10.0
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->setTemperature(10.0, true, false);
  assertEqual(0.0, EEPROM_TC::instance()->getThermalCorrection());
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  float temperature = thermalProbe->getRunningAverage();
  assertTrue(9.9 <= temperature && temperature <= 10.1);

  // set UI to ThermalCalibration state
  TankController* tc = TankController::instance();
  assertFalse(tc->isInCalibration());
  ThermalCalibration* test = new ThermalCalibration();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());

  // UI sets actual temperature to 10.5
  state->serialPort[0].dataOut = "";  // the history of data written
  test->setValue(10.5);
  assertEqual("Set temperature correction to 0.50\r\n", state->serialPort[0].dataOut);

  // The new temperature should be 10.5
  temperature = thermalProbe->getRawTemperature();
  assertTrue(9.9 <= temperature && temperature <= 10.1);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(10.49 <= temperature && temperature <= 10.51);

  // A new measured temperature should also be adjusted
  thermalProbe->setTemperature(15.5, false, false);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(15.9 <= temperature && temperature <= 16.1);
  temperature = EEPROM_TC::instance()->getThermalCorrection();
  assertTrue(0.49 < temperature && temperature < 0.51);

  // test for https://github.com/Open-Acidification/TankController/issues/174
  tc->loop(false);
  tc->loop(false);  // second loop needed to set the next state
  test = new ThermalCalibration();
  tc->setNextState(test, true);
  test->setValue(16.0);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertEqual(16.0, temperature);
}

unittest_main()

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TC_util.h"
#include "ThermalProbe_TC.h"

unittest_setup() {
  ThermalProbe_TC::reset();
}

unittest(readFromEepromOnStartup) {
  EEPROM_TC::instance()->setThermalCorrection(-1);
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->setTemperature(11.0, false);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  float temperature = thermalProbe->getRunningAverage();
  assertTrue(9.9 <= temperature && temperature <= 10.1);
}

unittest(writeToEepromWhenCorrectionChanged) {
  ThermalProbe_TC::instance()->setCorrection(0.5);
  assertEqual(0.5, EEPROM_TC::instance()->getThermalCorrection());
}

unittest(TempProbe_Test) {
  // Instance
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();

  // Test getResistance()
  uint16_t testRTD = thermalProbe->getResistance();
  assertEqual(0, testRTD);

  // Test getRawTemperature()
  float testTemp = thermalProbe->getRawTemperature();
  assertEqual(-242, (int16_t)testTemp);

  // Test readFault()
  uint8_t testFault = thermalProbe->readFault();
  assertEqual(0, testFault);

  // Test clearFault()
  thermalProbe->clearFault();
  testFault = thermalProbe->readFault();
  assertEqual(0, testFault);

  // Test setTemperature()
  float temperature = 0.0;
  thermalProbe->setTemperature(0, true);
  assertEqual(7621, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(-0.1 < temperature && temperature < 0.1);

  thermalProbe->setTemperature(10, true);
  assertEqual(7918, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(9.9 < temperature && temperature < 10.1);

  thermalProbe->setTemperature(90, true);
  assertEqual(10266, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(89.9 < temperature && temperature < 90.1);

  thermalProbe->setTemperature(100, true);
  assertEqual(10554, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(99.9 < temperature && temperature < 100.1);
}

unittest(adjustment) {
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->setTemperature(10.0, false);
  thermalProbe->setCorrection(0.5);
  delay(1000);
  float temperature = thermalProbe->getRunningAverage();
  assertTrue(10.49 <= temperature && temperature <= 10.51);
}

unittest(runningAverage) {
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->setTemperature(10.0, true);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  delay(1000);
  float temperature = thermalProbe->getRunningAverage();
  assertTrue(9.9 <= temperature && temperature <= 10.1);
  thermalProbe->setTemperature(20.0, true, false);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(10.9 <= temperature && temperature <= 11.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(11.9 <= temperature && temperature <= 12.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(12.9 <= temperature && temperature <= 13.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(13.9 <= temperature && temperature <= 14.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(14.9 <= temperature && temperature <= 15.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(15.9 <= temperature && temperature <= 16.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(16.9 <= temperature && temperature <= 17.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(17.9 <= temperature && temperature <= 18.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(18.9 <= temperature && temperature <= 19.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(19.9 <= temperature && temperature <= 20.1);
}

unittest(meanAndStdDev1) {
  // a big jump from 10 to 20
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->resetSample();
  assertEqual(0.0, thermalProbe->getSampleMean());
  assertEqual(0.0, thermalProbe->getSampleStandardDeviation());
  thermalProbe->setTemperature(10.0, true);  // raw temp will be about 10.0047
  for (uint8_t i = 0; i < 10; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue((9.99 < thermalProbe->getSampleMean()) && (thermalProbe->getSampleMean() < 10.01));
  assertTrue((-0.01 < thermalProbe->getSampleStandardDeviation()) &&
             (thermalProbe->getSampleStandardDeviation() < 0.01));
  thermalProbe->setTemperature(20.0, true);  // raw temp will be about 20.0212
  for (uint8_t i = 10; i < 60; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue(abs(thermalProbe->getSampleMean() - 18.3517833) < 0.00005);
  assertTrue(abs(thermalProbe->getSampleStandardDeviation() - 3.76443122) < 0.00005);

  thermalProbe->resetSample();
  assertEqual(0.0, thermalProbe->getSampleMean());
  assertEqual(0.0, thermalProbe->getSampleStandardDeviation());
}

unittest(meanAndStdDev2) {
  // a small jump from 10 to 10.1
  ThermalProbe_TC* thermalProbe = ThermalProbe_TC::instance();
  thermalProbe->resetSample();
  assertEqual(0.0, thermalProbe->getSampleMean());
  assertEqual(0.0, thermalProbe->getSampleStandardDeviation());
  thermalProbe->setTemperature(10.0, true);  // raw temp will be about 10.0047
  for (uint8_t i = 0; i < 10; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue((9.99 < thermalProbe->getSampleMean()) && (thermalProbe->getSampleMean() < 10.01));
  assertTrue((-0.01 < thermalProbe->getSampleStandardDeviation()) &&
             (thermalProbe->getSampleStandardDeviation() < 0.01));
  thermalProbe->setTemperature(10.1, true);  // raw temp will be about 10.072
  for (uint8_t i = 10; i < 60; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue((10.060771 - 0.00005 < thermalProbe->getSampleMean()) &&
             (thermalProbe->getSampleMean() < 10.060771 + 0.00005));
  assertTrue((0.025 - 0.003 < thermalProbe->getSampleStandardDeviation()) &&
             (thermalProbe->getSampleStandardDeviation() < 0.025 + 0.003));
}

unittest_main()

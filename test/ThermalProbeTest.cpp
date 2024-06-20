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
  assertTrue(abs(0.0 - temperature) < 0.1);

  thermalProbe->setTemperature(10, true);
  assertEqual(7918, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(abs(10.0 - temperature) < 0.1);

  thermalProbe->setTemperature(90, true);
  assertEqual(10266, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(abs(90.0 - temperature) < 0.1);

  thermalProbe->setTemperature(100, true);
  assertEqual(10554, thermalProbe->getResistance());
  temperature = thermalProbe->getRawTemperature();
  assertTrue(abs(100.0 - temperature) < 0.1);
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
  assertTrue(abs(10.0 - temperature) < 0.1);
  thermalProbe->setTemperature(20.0, true, false);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(11.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(12.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(13.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(14.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(15.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(16.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(17.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(18.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(19.0 - temperature) < 0.1);
  delay(1000);
  temperature = thermalProbe->getRunningAverage();
  assertTrue(abs(20.0 - temperature) < 0.1);
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
  assertTrue(abs(10.00 - thermalProbe->getSampleMean()) < 0.01);
  assertTrue(abs(0.00 - thermalProbe->getSampleStandardDeviation()) < 0.01);
  thermalProbe->setTemperature(20.0, true);  // raw temp will be about 20.0212
  for (uint8_t i = 10; i < 60; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue(abs(18.3517833 - thermalProbe->getSampleMean()) < 0.00005);
  assertTrue(abs(3.76443122 - thermalProbe->getSampleStandardDeviation()) < 0.00005);

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
  assertTrue(abs(10.00 - thermalProbe->getSampleMean()) < 0.01);
  assertTrue(abs(0.00 - thermalProbe->getSampleStandardDeviation()) < 0.01);
  thermalProbe->setTemperature(10.1, true);  // raw temp will be about 10.072
  for (uint8_t i = 10; i < 60; ++i) {
    delay(1000);
    thermalProbe->getRunningAverage();
  }
  assertTrue(abs(10.060771 - thermalProbe->getSampleMean()) < 0.00005);
  assertTrue(abs(0.025 - thermalProbe->getSampleStandardDeviation()) < 0.003);

  // Test getSampleMean()
  float testMean = thermalProbe->getSampleMean();
  assertTrue(abs(10.907 - testMean) < 0.01);
  // Test getSampleStandardDeviation()
  float testStdDev = thermalProbe->getSampleStandardDeviation();
  assertTrue(abs(2.8809 - testStdDev) < 0.01);
  // Test resetSample()
  thermalProbe->resetSample();
  testMean = thermalProbe->getSampleMean();
  assertEqual(testMean, 0.0);
  testStdDev = thermalProbe->getSampleStandardDeviation();
  assertEqual(testStdDev, 0.0);
}

unittest_main()

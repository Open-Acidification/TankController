#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "TempProbe_TC.h"

unittest_setup() {
  TempProbe_TC::reset();
}

unittest(readFromEepromOnStartup) {
  EEPROM_TC::instance()->setCorrectedTemp(-1);
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(11.0);
  for (int i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }
  delay(1000);
  float temp = tempProbe->getRunningAverage();
  assertTrue(9.9 <= temp && temp <= 10.1);
}

unittest(writeToEepromWhenCorrectionChanged) {
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setCorrection(0.5);
  assertEqual(0.5, EEPROM_TC::instance()->getCorrectedTemp());
}

unittest(TempProbe_Test) {
  // Instance
  TempProbe_TC* tempProbe = TempProbe_TC::instance();

  // Test getResistance()
  uint16_t testRTD = tempProbe->getResistance();
  assertEqual(0, testRTD);

  // Test getRawTemperature()
  float testTemp = tempProbe->getRawTemperature();
  assertEqual(-242, (int)testTemp);

  // Test readFault()
  uint8_t testFault = tempProbe->readFault();
  assertEqual(0, testFault);

  // Test clearFault()
  tempProbe->clearFault();
  testFault = tempProbe->readFault();
  assertEqual(0, testFault);

  // Test setTemperature()
  float temp = 0.0;
  tempProbe->setTemperature(0);
  assertEqual(7621, tempProbe->getResistance());
  temp = tempProbe->getRawTemperature();
  assertTrue(-0.1 < temp && temp < 0.1);

  tempProbe->setTemperature(10);
  assertEqual(7918, tempProbe->getResistance());
  temp = tempProbe->getRawTemperature();
  assertTrue(9.9 < temp && temp < 10.1);

  tempProbe->setTemperature(90);
  assertEqual(10266, tempProbe->getResistance());
  temp = tempProbe->getRawTemperature();
  assertTrue(89.9 < temp && temp < 90.1);

  tempProbe->setTemperature(100);
  assertEqual(10554, tempProbe->getResistance());
  temp = tempProbe->getRawTemperature();
  assertTrue(99.9 < temp && temp < 100.1);
}

unittest(adjustment) {
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(10.0);
  tempProbe->setCorrection(0.5);
  delay(1000);
  float temp = tempProbe->getRunningAverage();
  assertTrue(10.49 <= temp && temp <= 10.51);
}

unittest(runningAverage) {
  TempProbe_TC* tempProbe = TempProbe_TC::instance();
  tempProbe->setTemperature(10.0);
  tempProbe->setCorrection(0.0);
  for (int i = 0; i < 100; ++i) {
    delay(1000);
    tempProbe->getRunningAverage();
  }
  delay(1000);
  float temp = tempProbe->getRunningAverage();
  assertTrue(9.9 <= temp && temp <= 10.1);
  tempProbe->setTemperature(20.0);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(10.9 <= temp && temp <= 11.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(11.9 <= temp && temp <= 12.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(12.9 <= temp && temp <= 13.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(13.9 <= temp && temp <= 14.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(14.9 <= temp && temp <= 15.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(15.9 <= temp && temp <= 16.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(16.9 <= temp && temp <= 17.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(17.9 <= temp && temp <= 18.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(18.9 <= temp && temp <= 19.1);
  delay(1000);
  temp = tempProbe->getRunningAverage();
  assertTrue(19.9 <= temp && temp <= 20.1);
}

unittest_main()

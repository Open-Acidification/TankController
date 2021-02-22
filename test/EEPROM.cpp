#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"

unittest(Main) {
  // Test singleton
  EEPROM_TC* singleton1 = nullptr;
  singleton1 = EEPROM_TC::instance();
  assertNotNull(singleton1);
  EEPROM_TC* singleton2 = nullptr;
  singleton2 = EEPROM_TC::instance();
  assertNotNull(singleton2);
  assertEqual(singleton1, singleton2);
}

unittest(PH) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getPH(), 255);
  singleton->setPH(3);
  assertEqual(singleton->getPH(), 3);
}

unittest(Temp) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTemp(), 255);
  singleton->setTemp(4);
  assertEqual(singleton->getTemp(), 4);
}

unittest(TankID) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTankID(), 255);
  singleton->setTankID(5);
  assertEqual(singleton->getTankID(), 5);
}

unittest(CorrectedTemp) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getCorrectedTemp(), 255);
  singleton->setCorrectedTemp(6);
  assertEqual(singleton->getCorrectedTemp(), 6);
}

unittest(KP) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getKP(), 255);
  singleton->setKP(7);
  assertEqual(singleton->getKP(), 7);
}

unittest(KI) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getKI(), 255);
  singleton->setKI(8);
  assertEqual(singleton->getKI(), 8);
}

unittest(KD) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getKD(), 255);
  singleton->setKD(9);
  assertEqual(singleton->getKD(), 9);
}

unittest(Mac) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getMac(), 255);
  singleton->setMac(10);
  assertEqual(singleton->getMac(), 10);
}

unittest(Heat) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getHeat(), 255);
  singleton->setHeat(11);
  assertEqual(singleton->getHeat(), 11);
}

unittest(Amplitude) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getAmplitude(), 255);
  singleton->setAmplitude(12);
  assertEqual(singleton->getAmplitude(), 12);
}

unittest(Frequency) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getFrequency(), 255);
  singleton->setFrequency(13);
  assertEqual(singleton->getFrequency(), 13);
}

unittest(Granularity) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getGranularity(), 255);
  singleton->setGranularity(14);
  assertEqual(singleton->getGranularity(), 14);
}

unittest(MaxDataAge) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getMaxDataAge(), 255);
  singleton->setMaxDataAge(15);
  assertEqual(singleton->getMaxDataAge(), 15);
}

unittest(PHSeriesSize) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getPHSeriesSize(), 255);
  singleton->setPHSeriesSize(16);
  assertEqual(singleton->getPHSeriesSize(), 16);
}

unittest(PHSeriesPointer) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getPHSeriesPointer(), 255);
  singleton->setPHSeriesPointer(17);
  assertEqual(singleton->getPHSeriesPointer(), 17);
}

unittest(TempSeriesSize) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTempSeriesSize(), 255);
  singleton->setTempSeriesSize(18);
  assertEqual(singleton->getTempSeriesSize(), 18);
}

unittest(TempSeriesPointer) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTempSeriesPointer(), 255);
  singleton->setTempSeriesPointer(19);
  assertEqual(singleton->getTempSeriesPointer(), 19);
}

unittest(PHInterval) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getPHInterval(), 255);
  singleton->setPHInterval(20);
  assertEqual(singleton->getPHInterval(), 20);
}

unittest(PHDelay) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getPHDelay(), 255);
  singleton->setPHDelay(21);
  assertEqual(singleton->getPHDelay(), 21);
}

unittest(TempInterval) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTempInterval(), 255);
  singleton->setTempInterval(22);
  assertEqual(singleton->getTempInterval(), 22);
}

unittest(TempDelay) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(singleton->getTempDelay(), 255);
  singleton->setTempDelay(23);
  assertEqual(singleton->getTempDelay(), 23);
}

unittest_main()

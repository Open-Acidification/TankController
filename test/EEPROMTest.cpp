#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"

unittest_setup() {
  GODMODE()->resetEEPROM();
}

unittest(singleton) {
  // Test singleton
  EEPROM_TC* singleton1 = nullptr;
  singleton1 = EEPROM_TC::instance();
  assertNotNull(singleton1);
  EEPROM_TC* singleton2 = nullptr;
  singleton2 = EEPROM_TC::instance();
  assertNotNull(singleton2);
  assertEqual(singleton1, singleton2);
}

unittest(eeprom_Read_and_Write_Double) {
  EEPROM_TC* test = EEPROM_TC::instance();
  const uint16_t TEST_ADDRESS = 4000;  // beyond the end of our use

  // integer
  test->eepromWriteFloat(TEST_ADDRESS, 10);
  assertEqual(10, test->eepromReadFloat(TEST_ADDRESS));

  // float
  test->eepromWriteFloat(TEST_ADDRESS, 12.25);
  assertEqual(12.25, test->eepromReadFloat(TEST_ADDRESS));
}

unittest(PH) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPH());
  singleton->setPH(3.125);
  assertEqual(3.125, singleton->getPH());
}

unittest(Temp) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getTemp());
  singleton->setTemp(4);
  assertEqual(4, singleton->getTemp());
}

unittest(TankID) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0, singleton->getTankID());
  singleton->setTankID(5);
  assertEqual(5, singleton->getTankID());
}

unittest(CorrectedTemp) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getCorrectedTemp());
  singleton->setCorrectedTemp(6);
  assertEqual(6, singleton->getCorrectedTemp());
}

unittest(KP) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getKP());
  singleton->setKP(7);
  assertEqual(7, singleton->getKP());
}

unittest(KI) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getKI());
  singleton->setKI(8);
  assertEqual(8, singleton->getKI());
}

unittest(KD) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getKD());
  singleton->setKD(9);
  assertEqual(9, singleton->getKD());
}

unittest(Mac) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getMac());
  singleton->setMac(10);
  assertEqual(10, singleton->getMac());
}

unittest(Heat) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertTrue(singleton->getHeat());  // (bool)(-1)
  singleton->setHeat(false);
  assertFalse(singleton->getHeat());
  singleton->setHeat(true);
  assertTrue(singleton->getHeat());
}

unittest(Amplitude) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getAmplitude());
  singleton->setAmplitude(12);
  assertEqual(12, singleton->getAmplitude());
}

unittest(Frequency) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getFrequency());
  singleton->setFrequency(13);
  assertEqual(13, singleton->getFrequency());
}

unittest(Granularity) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getGranularity());
  singleton->setGranularity(14);
  assertEqual(14, singleton->getGranularity());
}

unittest(MaxDataAge) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getMaxDataAge());
  singleton->setMaxDataAge(15);
  assertEqual(15, singleton->getMaxDataAge());
}

unittest(PHSeriesSize) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPHSeriesSize());
  singleton->setPHSeriesSize(16);
  assertEqual(16, singleton->getPHSeriesSize());
}

unittest(PHSeriesPointer) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPHSeriesPointer());
  singleton->setPHSeriesPointer(17);
  assertEqual(17, singleton->getPHSeriesPointer());
}

unittest(TempSeriesSize) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getTempSeriesSize());
  singleton->setTempSeriesSize(18);
  assertEqual(18, singleton->getTempSeriesSize());
}

unittest(TempSeriesPointer) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getTempSeriesPointer());
  singleton->setTempSeriesPointer(19);
  assertEqual(19, singleton->getTempSeriesPointer());
}

unittest(PHInterval) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPHInterval());
  singleton->setPHInterval(20);
  assertEqual(20, singleton->getPHInterval());
}

unittest(PHDelay) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPHDelay());
  singleton->setPHDelay(21);
  assertEqual(21, singleton->getPHDelay());
}

unittest(TempInterval) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getTempInterval());
  singleton->setTempInterval(22);
  assertEqual(22, singleton->getTempInterval());
}

unittest(TempDelay) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getTempDelay());
  singleton->setTempDelay(23);
  assertEqual(23, singleton->getTempDelay());
}

unittest(GoogleSheetIntervalMinutes) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(65535, singleton->getGoogleSheetInterval());
  singleton->setGoogleSheetInterval(20);
  assertEqual(20, singleton->getGoogleSheetInterval());
}

unittest_main()

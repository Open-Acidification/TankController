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

unittest(Ph) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPh());
  singleton->setPh(3.125);
  assertEqual(3.125, singleton->getPh());
}

unittest(RampStartingPh) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getRampStartingPh());
  singleton->setRampStartingPh(4.125);
  assertEqual(4.125, singleton->getRampStartingPh());
  assertNAN(singleton->getThermalRampInitialValue());
  singleton->setThermalRampInitialValue(4.125);
  assertEqual(4.125, singleton->getThermalRampInitialValue());
}

unittest(RampTimeStart) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0xFFFFFFFF, singleton->getPhRampTimeStart());
  singleton->setPhRampTimeStart(123456789);
  assertEqual(123456789, singleton->getPhRampTimeStart());
  assertEqual(0xFFFFFFFF, singleton->getThermalRampTimeStart());
  singleton->setThermalRampTimeStart(123456789);
  assertEqual(123456789, singleton->getThermalRampTimeStart());
}

unittest(RampTimeEnd) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0xFFFFFFFF, singleton->getPhRampTimeEnd());
  singleton->setPhRampTimeEnd(987654321);
  assertEqual(987654321, singleton->getPhRampTimeEnd());
  assertEqual(0xFFFFFFFF, singleton->getThermalRampTimeEnd());
  singleton->setThermalRampTimeEnd(987654321);
  assertEqual(987654321, singleton->getThermalRampTimeEnd());
}

unittest(Temp) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getThermalTarget());
  singleton->setTemp(4);
  assertEqual(4, singleton->getThermalTarget());
}

unittest(PHFunctionType) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0, singleton->getPHFunctionType());
  singleton->setPHFunctionType(5);
  assertEqual(5, singleton->getPHFunctionType());
}
unittest(ThermalFunctionType) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0, singleton->getThermalFunctionType());
  singleton->setThermalFunctionType(5);
  assertEqual(5, singleton->getThermalFunctionType());
}
unittest(SineAmplitude) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPhSineAmplitude());
  singleton->setPhSineAmplitude(4);
  assertEqual(4, singleton->getPhSineAmplitude());
  assertNAN(singleton->getThermalSineAmplitude());
  singleton->setThermalSineAmplitude(4);
  assertEqual(4, singleton->getThermalSineAmplitude());
}
unittest(SineStartTime) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0xFFFFFFFF, singleton->getPhSineStartTime());
  singleton->setPhSineStartTime(123456789);
  assertEqual(123456789, singleton->getPhSineStartTime());
  assertEqual(0xFFFFFFFF, singleton->getThermalSineStartTime());
  singleton->setThermalSineStartTime(123456789);
  assertEqual(123456789, singleton->getThermalSineStartTime());
}

unittest(SinePeriod) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0xFFFFFFFF, singleton->getPhSinePeriod());
  singleton->setPhSinePeriod(987654321);
  assertEqual(987654321, singleton->getPhSinePeriod());
  assertEqual(0xFFFFFFFF, singleton->getThermalSinePeriod());
  singleton->setThermalSinePeriod(987654321);
  assertEqual(987654321, singleton->getThermalSinePeriod());
}
unittest(TankID) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(0, singleton->getTankID());
  singleton->setTankID(5);
  assertEqual(5, singleton->getTankID());
}

unittest(ThermalCorrection) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getThermalCorrection());
  singleton->setThermalCorrection(6);
  assertEqual(6, singleton->getThermalCorrection());
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
  uint8_t bytes[6];
  EEPROM_TC* singleton = EEPROM_TC::instance();
  singleton->getMac(bytes);
  for (int i = 0; i < 6; ++i) {
    assertEqual(255, bytes[i]);
  }
  for (int i = 0; i < 6; ++i) {
    bytes[i] = i + 10;
  }
  singleton->setMac(bytes);
  for (int i = 0; i < 6; ++i) {
    bytes[i] = 0;
  }
  singleton->getMac(bytes);
  for (int i = 0; i < 6; ++i) {
    assertEqual(i + 10, bytes[i]);
  }
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

unittest(PhSeriesSize) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPhSeriesSize());
  singleton->setPhSeriesSize(16);
  assertEqual(16, singleton->getPhSeriesSize());
}

unittest(PhSeriesPointer) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPhSeriesPointer());
  singleton->setPhSeriesPointer(17);
  assertEqual(17, singleton->getPhSeriesPointer());
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

unittest(PhInterval) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPhInterval());
  singleton->setPhInterval(20);
  assertEqual(20, singleton->getPhInterval());
}

unittest(PhDelay) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertNAN(singleton->getPhDelay());
  singleton->setPhDelay(21);
  assertEqual(21, singleton->getPhDelay());
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

unittest(IgnoreBadPHSlope) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertTrue(singleton->getIgnoreBadPHSlope());  // (bool)(-1)
  singleton->setIgnoreBadPHSlope(false);
  assertFalse(singleton->getIgnoreBadPHSlope());
  singleton->setIgnoreBadPHSlope(true);
  assertTrue(singleton->getIgnoreBadPHSlope());
}

unittest(GoogleSheetIntervalMinutes) {
  EEPROM_TC* singleton = EEPROM_TC::instance();
  assertEqual(65535, singleton->getGoogleSheetInterval());
  singleton->setGoogleSheetInterval(20);
  assertEqual(20, singleton->getGoogleSheetInterval());
}

unittest_main()

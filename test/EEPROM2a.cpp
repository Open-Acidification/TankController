#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <EEPROM.h>

#include "EEPROM_TC.h"

/*
 * If the first four bytes aren't a valid version,
 * then we use the legacy layout.
 *
 * This is in a separate test because the other
 * tests are based on a boot/reset contents.
 */
unittest(getVersion2) {
  union {
    double d;
    uint8_t b[8];
  } u;
  u.d = 7.2;
  for (int i = 0; i < 8; ++i) {
    EEPROM.update(i, u.b[i]);
  }
  EEPROM_TC* eeprom = EEPROM_TC::instance();
  assertEqual(2, eeprom->getVersion());
}

unittest_main()

#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <EEPROM.h>

#include "EEPROM_TC.h"

/*
 * If the first four bytes are a valid version,
 * then we use that version (in this case, v3).
 *
 * This is in a separate test because the other
 * tests are based on a boot/reset contents.
 */
unittest(getVersion3) {
  union {
    int i;
    uint8_t b[4];
  } u;
  u.i = 3;
  for (int i = 0; i < 4; ++i) {
    EEPROM.update(i, u.b[i]);
  }
  EEPROM_TC* eeprom = EEPROM_TC::instance();
  assertEqual(3, eeprom->getVersion());
}

unittest_main()

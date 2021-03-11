#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tank = TankControllerLib::instance();
  SDClass_TC* sd = SDClass_TC::instance();
  assertTrue(tank != nullptr);
  assertTrue(sd != nullptr);
}

unittest_main()

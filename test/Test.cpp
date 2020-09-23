#define TankControllerClass TankControllerTest
#include "Common.cpp"

unittest(className) {
  TankControllerClass tank;
  assertEqual("TankControllerTest", tank.className());
  assertTrue(false);  // do errors get caught by GitHub actions?
}

unittest_main()

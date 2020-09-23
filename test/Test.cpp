#define TankControllerClass TankControllerTest
#include "Common.cpp"

unittest(className) {
  TankControllerClass tank;
  assertEqual("TankControllerTest", tank.className());
}

unittest_main()

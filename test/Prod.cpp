#define TankControllerClass TankControllerProd
#include "Common.cpp"

unittest(className) {
  TankControllerClass tank;
  assertEqual("TankControllerProd", tank.className());
}

unittest_main()

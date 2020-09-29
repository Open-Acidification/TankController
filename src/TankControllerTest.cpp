#include "TankControllerTest.h"
#ifdef ARDUINO_CI

TankControllerTest::TankControllerTest() : TankControllerProd() {
}

TankControllerTest::~TankControllerTest() {
}

void TankControllerTest::setup() {
  TankControllerProd::setup();
}

void TankControllerTest::loop() {
  TankControllerProd::loop();
}

#endif

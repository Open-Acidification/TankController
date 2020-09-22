#include "TankControllerTest.h"
#ifdef MOCK_PINS_COUNT

TankControllerTest::TankControllerTest() : TankControllerProd() {}

TankControllerTest::~TankControllerTest() {}

void TankControllerTest::setup() {
    TankControllerProd::setup();
}

void TankControllerTest::loop() {
    TankControllerProd::loop();
}

#endif

/**
 * This Arduino Sketch is all that is needed to use TankControllerLib.
 * Everything else should go in the library so it can be tested and
 * used in the mock environment.
 */

#include "TankControllerLib.h"

TankControllerLib *tank;

void setup() {
  tank = TankControllerLib::instance();
  tank->setup();
}

void loop() {
  tank->loop();
}

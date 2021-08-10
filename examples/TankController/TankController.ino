/**
 * This Arduino Sketch is all that is needed to use TankControllerLib.
 * Everything else should go in the library so it can be tested and
 * used in the mock environment.
 */

const char pushingBoxID[] = "PushingBoxIdentifier";  // <<== REPLACE THIS!

#include "TankControllerLib.h"

TankControllerLib *tank = nullptr;

void serialEvent() {  // if the hardware serial port_0 receives a char
  tank->serialEvent();
}

void serialEvent1() {  // if the hardware serial port_1 receives a char
  tank->serialEvent1();
}

void setup() {
  // the install process is followed by a reset and we get two startups
  delay(500);
  tank = TankControllerLib::instance(pushingBoxID);
  tank->setup();
}

void loop() {
  tank->loop();
}

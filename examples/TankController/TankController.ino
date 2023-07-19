/**
 * This Arduino Sketch is all that is needed to use TankController.
 * Everything else should go in the library so it can be tested and
 * used in the mock environment.
 */

// For PushingBox to work you need to provide an ID.
// If it remains empty, then no data will be sent.
const char pushingBoxID[] = "";

// We query a web server for GMT time and then adjust for local time
const int tzOffsetHrs = -7;

#include "TankController.h"

TankController *tank = nullptr;

void serialEvent() {  // if the hardware serial port_0 receives a char
  tank->serialEvent();
}

void serialEvent1() {  // if the hardware serial port_1 receives a char
  tank->serialEvent1();
}

void setup() {
  // the install process is followed by a reset and we get two startups
  delay(500);
  tank = TankController::instance(pushingBoxID, tzOffsetHrs);
  tank->setup();
}

void loop() {
  tank->loop();
}

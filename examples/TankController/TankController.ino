
#include "TankControllerLib.h"
TankControllerLib *tank = TankControllerLib::instance();

void setup() {
  tank->setup();
}
void loop() {
  tank->loop();
}

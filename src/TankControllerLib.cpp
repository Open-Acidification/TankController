#include "TankControllerLib.h"

TankControllerProd::TankControllerProd() {
}

TankControllerProd::~TankControllerProd() {
}

void TankControllerProd::setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void TankControllerProd::loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for half a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                       // wait for half a second
}

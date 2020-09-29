#pragma once

#include <Arduino.h>
#ifdef ARDUINO_CI
#define TankControllerTest TankController
#else
#define TankControllerProd TankController
#endif

class TankControllerProd {
public:
  TankControllerProd();
  ~TankControllerProd();
  virtual String className() const {
    return "TankControllerProd";
  }
  void setup();
  void loop();

private:
  int foo;
};

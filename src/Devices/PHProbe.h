#pragma once
#include "Arduino.h"

class PHProbe {
public:
  static PHProbe* instance();
  double getPH() {
    return value;
  }
  void probeData(String string);

private:
  // Class variable
  static PHProbe* _instance;
  // instance variable
  double value = 0;
  // Methods
  PHProbe();
};

#pragma once
#include <Arduino.h>

class UIState;
class TankControllerLib {
public:
  static TankControllerLib* singleton() {
    if (!instance) {
      instance = new TankControllerLib;
    }
    return instance;
  }
  TankControllerLib();
  ~TankControllerLib();
  void setup();
  void loop();

private:
  friend class UIState;
  void changeState(UIState* newState);
  UIState* _state;
  static TankControllerLib* instance;
};

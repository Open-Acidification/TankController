#pragma once
#include <Arduino.h>

class UIState;
class TankControllerLib {
public:
  static TankControllerLib* instance();
  void setup();
  void loop();

protected:
  TankControllerLib();
  ~TankControllerLib();

private:
  friend class UIState;
  void changeState(UIState* newState);
  UIState* _state;
  static TankControllerLib* _instance;
};

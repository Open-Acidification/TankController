/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "UIState.h"

class SeePIDConstants : public UIState {
public:
  SeePIDConstants(TankControllerLib* tc) : UIState(tc) {
  }
  void loop();
  const char* name() {
    return "SeePIDConstants";
  }
  void start() {
    startTime = millis();
  }

private:
  void loadKp(int line);
  void loadKi(int line);
  void loadKd(int line);
  void loadSlope(int line);

  int long startTime;
};

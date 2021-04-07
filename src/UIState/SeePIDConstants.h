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
    msEnd = millis() + (1000 * secondsLeft);
    // Subtract 1 because seconds left needs to be at the end of our current second boundary
    secondsLeft--;
  }

private:
  void rotateDisplay();
  void loadKp(int line);
  void loadKi(int line);
  void loadKd(int line);
  void loadSlope(int line);

  int msEnd;
  int secondsLeft = 5;
};

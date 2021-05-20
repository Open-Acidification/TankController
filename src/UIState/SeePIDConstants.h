/**
 * SeePIDConstants.h
 *
 * See PID Constants
 */
#pragma once
#include "TC_util.h"
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
  void loadKp(uint16_t line);
  void loadKi(uint16_t line);
  void loadKd(uint16_t line);
  void loadSlope(uint16_t line);

  uint32_t startTime;
};

/**
 * SeePHSlope.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeePHSlope : public UIState {
public:
  SeePHSlope(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  void loop();
  const char* name() {
    return "SeePHSlope";
  }
  const char* prompt() {
    return "PH Slope:";
  };
};

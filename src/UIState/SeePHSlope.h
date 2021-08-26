/**
 * SeePHSlope.h
 *
 * Display the tank ID
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeePHSlope : public UIState {
public:
  SeePHSlope(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  void loop();
  const char* name() {
    return CSTR("SeePHSlope");
  }
  const char* prompt() {
    return CSTR("PH Slope:");
  };
};

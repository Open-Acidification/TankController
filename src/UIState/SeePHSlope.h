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
  const __FlashStringHelper* name() {
    return F("SeePHSlope");
  }
  const __FlashStringHelper* prompt() {
    return F("PH Slope:");
  };
};

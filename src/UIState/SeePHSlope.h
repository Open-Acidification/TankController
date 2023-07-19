/**
 * SeePHSlope.h
 *
 * Display the pH slope
 */
#pragma once
#include "UIState.h"

class SeePHSlope : public UIState {
public:
  SeePHSlope(TankController* tc) : UIState(tc) {
  }
  void start() override;
  void loop() override;
  const __FlashStringHelper* name() override {
    return F("SeePHSlope");
  }
  const __FlashStringHelper* prompt() override {
    return F("PH Slope:");
  };
};

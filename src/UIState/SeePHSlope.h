/**
 * SeePHSlope.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeePHSlope : public UIState {
public:
  SeePHSlope(TankController* tc) : UIState(tc) {
  }
  void start() override;
  void loop() override;
  const __FlashStringHelper* name() {
    return F("SeePHSlope");
  }
  const __FlashStringHelper* prompt() {
    return F("PH Slope:");
  };
  int writeToBuffer(char*, int, int);
};

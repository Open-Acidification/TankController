/**
 * SeeTempCalOffset.h
 *
 * Display the calibration offset for temperature
 */
#pragma once
#include "UIState.h"

class SeeTempCalOffset : public UIState {
public:
  SeeTempCalOffset(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() {
    return F("SeeTempCalOffset");
  }
  const __FlashStringHelper* prompt() {
    return F("Temp Cal Offset:");
  };
};

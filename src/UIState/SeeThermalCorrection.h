/**
 * SeeThermalCorrection.h
 *
 * Display the calibration offset for temperature
 */
#pragma once
#include "UIState.h"

class SeeThermalCorrection : public UIState {
public:
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeThermalCorrection");
  }
  const __FlashStringHelper* prompt() override {
    return F("Temp Cal Offset:");
  };
};

/**
 * SeeLogFile.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeeLogFile : public UIState {
public:
  SeeLogFile(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeLogFile");
  }
  const __FlashStringHelper* prompt() override {
    return F("Current Log File");
  };
};

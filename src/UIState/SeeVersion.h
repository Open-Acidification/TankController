/**
 * SeeVersion.h
 *
 * Display the tankController Version
 */
#pragma once
#include "UIState.h"

class SeeVersion : public UIState {
public:
  SeeVersion(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() {
    return F("SeeVersion");
  }
  const __FlashStringHelper* prompt() {
    return F("Software Version");
  };
  int writeToBuffer(char*, int, int) override;
};

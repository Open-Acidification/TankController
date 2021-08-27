/**
 * SeeVersion.h
 *
 * Display the tankController Version
 */
#pragma once
#include "UIState.h"

class SeeVersion : public UIState {
public:
  SeeVersion(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const __FlashStringHelper* name() {
    return F("SeeVersion");
  }
  const char* prompt() {
    return "Software Version";
  };
};

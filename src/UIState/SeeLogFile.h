/**
 * SeeLogFile.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeeLogFile : public UIState {
public:
  SeeLogFile(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return (const char*)F("SeeLogFile");
  }
  const char* prompt() {
    return (const char*)F("Current Log File");
  };
};

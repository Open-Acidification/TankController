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
    return "SeeLogFile";
  }
  const char* prompt() {
    return "Current Log File";
  };
};

/**
 * SeeLogFile.h
 *
 * Display the tank ID
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeLogFile : public UIState {
public:
  SeeLogFile(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return CSTR("SeeLogFile");
  }
  const char* prompt() {
    return CSTR("Current Log File");
  };
};

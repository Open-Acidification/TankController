/**
 * SeeVersion.h
 *
 * Display the tankController Version
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeVersion : public UIState {
public:
  SeeVersion(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return CSTR("SeeVersion");
  }
  const char* prompt() {
    return CSTR("Software Version");
  };
};

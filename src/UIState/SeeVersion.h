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
  const char* name() {
    return "SeeVersion";
  }
  const char* prompt() {
    return "Software Version";
  };
};

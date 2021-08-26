#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return CSTR("SeeFreeMemory");
  }
  const char* prompt() {
    return CSTR("Free Memory:");
  };
};

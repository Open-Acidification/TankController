#pragma once
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return "SeeFreeMemory";
  }
  const char* prompt() {
    return "Free Memory:";
  };
};

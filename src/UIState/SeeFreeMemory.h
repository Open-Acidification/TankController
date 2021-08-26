#pragma once
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return (const char*)F("SeeFreeMemory");
  }
  const char* prompt() {
    return (const char*)F("Free Memory:");
  };
};

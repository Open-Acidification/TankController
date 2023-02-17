#pragma once
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeFreeMemory");
  }
  const __FlashStringHelper* prompt() override {
    return F("Free Memory:");
  };
};

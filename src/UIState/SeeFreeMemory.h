#pragma once
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() {
    return F("SeeFreeMemory");
  }
  const __FlashStringHelper* prompt() {
    return F("Free Memory:");
  };
  int writeToBuffer(char*, int, int) override;
};

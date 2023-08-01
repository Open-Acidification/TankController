#pragma once
#include "UIState.h"

class SeeFreeMemory : public UIState {
public:
  SeeFreeMemory() : UIState() {
  }
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeFreeMemory");
  }
  const __FlashStringHelper* prompt() override {
    return F("Free Memory:");
  };
};

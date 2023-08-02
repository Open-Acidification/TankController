/**
 * SeeVersion.h
 *
 * Display the tankController Version
 */
#pragma once
#include "UIState.h"

class SeeVersion : public UIState {
public:
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeVersion");
  }
  const __FlashStringHelper* prompt() override {
    return F("Software Version");
  };
};

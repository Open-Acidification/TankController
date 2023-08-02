/**
 * SeeTankID.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeeTankID : public UIState {
public:
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeTankID");
  }
  const __FlashStringHelper* prompt() override {
    return F("Tank ID:");
  };
};

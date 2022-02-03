/**
 * SeeTankID.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeeTankID : public UIState {
public:
  SeeTankID(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() {
    return F("SeeTankID");
  }
  const __FlashStringHelper* prompt() {
    return F("Tank ID:");
  };
  int writeToBuffer(char*, int, int) override;
};

/**
 * SeeTankID.h
 *
 * Display the tank ID
 */
#pragma once
#include "UIState.h"

class SeeTankID : public UIState {
public:
  SeeTankID(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const __FlashStringHelper* name() {
    return F("SeeTankID");
  }
  const char* prompt() {
    return "Tank ID:";
  };
};

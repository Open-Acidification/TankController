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
  const char* name() {
    return (const char*)F("SeeTankID");
  }
  const char* prompt() {
    return (const char*)F("Tank ID:");
  };
};

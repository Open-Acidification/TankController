/**
 * SeeTankID.h
 *
 * Display the tank ID
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeTankID : public UIState {
public:
  SeeTankID(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return CSTR("SeeTankID");
  }
  const char* prompt() {
    return CSTR("Tank ID:");
  };
};

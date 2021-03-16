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
  void handleKey(char key);
  const char* prompt() {
    return "Tank ID=        ";
  };
};

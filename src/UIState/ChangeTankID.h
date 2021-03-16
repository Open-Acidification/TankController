/**
 * ChangeTankID.h
 *
 * Change Tank ID
 */
#pragma once
#include "UIState.h"

class ChangeTankID : public UIState {
public:
  ChangeTankID(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  const char* prompt() {
    return "Set Tank ID#    ";
  };
};

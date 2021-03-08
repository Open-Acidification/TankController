/**
 * SetChillOrHeat.h
 *
 * Set if the chiller or heater is on
 */
#pragma once
#include "UIState.h"

class SetChillOrHeat : public UIState {
public:
  void handleKey(char key);
  const char *prompt() {
    return "Chill or Heat?";
  };
};

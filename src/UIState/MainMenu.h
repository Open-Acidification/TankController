/**
 * MainMenu.h
 *
 * Branch to other states to handle various menu options
 */

#pragma once

#include "../TankControllerLib.h"
#include "UIState.h"

class MainMenu : public UIState {
public:
  MainMenu() {
  }
  ~MainMenu() {
  }
  virtual void handleKey(char key);
  const char *prompt() {
    return "Main Menu";
  };
};

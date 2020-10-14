/*
 * MainMenu.h
 *
 * Branch to other states to handle various menu options
 */

#pragma once

#include "../TankControllerLib.h"
#include "UIState.h"

class MainMenu : UIState {
public:
  MainMenu() {
  }
  ~MainMenu() {
  }
  virtual void handleKey(char key);
};

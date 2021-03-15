/**
 * MainMenu.h
 *
 * Branch to other states to handle various menu options
 */

#pragma once

#include "UIState.h"

class TankControllerLib;  // forward reference
class MainMenu : public UIState {
public:
  MainMenu(TankControllerLib* tc) : UIState(tc) {
  }
  virtual void handleKey(char key);
  const char* prompt() {
    return "Main Menu";
  };
  virtual bool isMainMenu() {
    return true;
  }
};

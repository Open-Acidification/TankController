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
  void handleKey(char key);
  String name() {
    return "MainMenu";
  }
  const char* prompt() {
    return "Main Menu       ";
  };
  bool isMainMenu() {
    return true;
  }
  void loop();
};

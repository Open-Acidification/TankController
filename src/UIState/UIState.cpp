#include "UIState.h"

#include "../TankControllerLib.h"
#include "Devices/LiquidCrystal_TC.h"
#include "UIState/MainMenu.h"
#include "UIState/Wait.h"

// instance methods

void UIState::handleKey(char key) {
  // default is to just return to main menu
  // this is useful for display-only actions
  this->returnToMainMenu();
}

void UIState::returnToMainMenu(int msDelay) {
  if (msDelay) {
    this->setNextState((UIState*)new Wait(tc, msDelay));
  } else {
    this->setNextState((UIState*)new MainMenu(tc));
  }
}

void UIState::setNextState(UIState* state) {
  tc->setNextState(state);
}

void UIState::start() {
  // default is to print the current prompt on the first line of the display
  const char* prompt = this->prompt();
  LiquidCrystal_TC::instance()->writeLine(prompt, 0);
  LiquidCrystal_TC::instance()->writeLine("                ", 1);
}

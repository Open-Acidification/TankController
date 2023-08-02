#include "UIState.h"

#include "../TankController.h"
#include "Devices/LiquidCrystal_TC.h"
#include "UIState/MainMenu.h"
#include "UIState/Wait.h"

// instance methods

void UIState::handleKey(char key) {
  // use value just to avoid compiler warning
  (void)key;
  // default is to just return to main menu
  // this is useful for display-only actions
  this->returnToMainMenu();
}

void UIState::returnToMainMenu(uint16_t msDelay) {
  if (msDelay) {
    this->setNextState(new Wait(msDelay));
  } else {
    this->setNextState(new MainMenu());
  }
}

void UIState::setNextState(UIState* state) {
  TankController::instance()->setNextState(state);
}

void UIState::start() {
  // default is to print the current prompt on the first line of the display
  LiquidCrystal_TC::instance()->writeLine(this->prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(F(""), 1);
}

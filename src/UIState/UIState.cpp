#include "UIState.h"

#include "../TankControllerLib.h"
#include "Devices/LiquidCrystal_TC.h"
#include "UIState/MainMenu.h"

/**
 * SleepHandler
 *
 * When running on the real Arduino, the delay() function will actually pause.
 * When running on the mock Arduino, the delay() function increments the clock
 * and then returns immediately. While this generally the right thing to do,
 * this presents a couple issues. First, if we want to look at the state during
 * the delay, this is difficult since the state could change after the delay
 * (e.g., show a message on the LCD for one second, then clear it). Second, if
 * we are using the mock Arduino library to emulate the application in a GUI,
 * we want to have a real delay. To solve this, we will allow code (such as tests
 * and the GUI) to register to be notified of delays. Then we can take whatever
 * action is appropriate.
 *
 * typedef void (*SleepHandler)(int ms);
 *
 */

SleepHandler UIState::sleepHandlers[SLEEP_HANDLERS_COUNT];

bool UIState::addSleepHandler(SleepHandler pFunction) {
  for (int i = 0; i < SLEEP_HANDLERS_COUNT; ++i) {
    if (!sleepHandlers[i]) {
      sleepHandlers[i] = pFunction;
      return true;
    }
  }
  return false;
}

bool UIState::removeSleepHandler(SleepHandler pFunction) {
  for (int i = 0; i < SLEEP_HANDLERS_COUNT; ++i) {
    if (sleepHandlers[i] == pFunction) {
      sleepHandlers[i] = nullptr;
      return true;
    }
  }
  return false;
}

void UIState::sleep(int ms) {
  for (int i = 0; i < SLEEP_HANDLERS_COUNT; ++i) {
    SleepHandler pFunction = sleepHandlers[i];
    if (pFunction) {
      pFunction(ms);
    }
  }
  delay(ms);
}

// instance methods

void UIState::handleKey(char key) {
  // default is to just return to main menu
  // this is useful for display-only actions
  this->returnToMainMenu();
}

void UIState::returnToMainMenu(int msDelay) {
  if (msDelay) {
    sleep(msDelay);
  }
  this->setNextState((UIState*)new MainMenu(tc));
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

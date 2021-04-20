#include "Wait.h"
#include "MainMenu.h"

Wait::Wait(TankControllerLib *tc, int msDelay, UIState *nextState) : UIState(tc) {
  endTime = millis() + msDelay;
  this->nextState = nextState;
  if (this->nextState == nullptr) {
    this->nextState = (UIState*)new MainMenu(tc);
  }
}

void Wait::loop() {
  if (endTime <= millis()) {
    this->setNextState(nextState);
  }
}

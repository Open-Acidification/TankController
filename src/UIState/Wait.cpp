#include "Wait.h"

#include "MainMenu.h"
#include "TC_util.h"


Wait::Wait(TankControllerLib *tc, int msDelay, UIState *nextState) : UIState(tc) {
  endTime = millis() + msDelay;
  if (nextState) {
    COUT(nextState->name());
  }
  this->nextState = nextState;
  if (this->nextState == nullptr) {
    this->nextState = (UIState *)new MainMenu(tc);
  }
}

void Wait::loop() {
  if (endTime <= millis()) {
    this->setNextState(nextState);
  }
}

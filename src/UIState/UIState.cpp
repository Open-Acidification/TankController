#include "UIState.h"

#include "../TankControllerLib.h"
#include "UIState/MainMenu.h"

void UIState::setNextState(UIState* state) {
  tc->setNextState(state);
}

void UIState::returnToMainMenu() {
  setNextState((UIState*)new MainMenu(tc));
}

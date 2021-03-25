#include "Wait.h"

void Wait::loop() {
  if (endTime <= millis()) {
    this->returnToMainMenu();
  }
}

#include "TankControllerLib.h"

#ifdef ARDUINO_CI
#include <cassert>
#else
#define assert(p) (void)0
#endif

#include "UIState/MainMenu.h"
#include "UIState/UIState.h"

TankControllerLib* TankControllerLib::_instance = nullptr;

TankControllerLib* TankControllerLib::instance() {
  if (!_instance) {
    _instance = new TankControllerLib;
  }
  return _instance;
}

TankControllerLib::TankControllerLib() {
  assert(!_instance);
  _state = nullptr;
}

TankControllerLib::~TankControllerLib() {
  changeState(nullptr);
  assert(this == _instance);
  _instance = nullptr;
}

void TankControllerLib::setup() {
  changeState((UIState*)new MainMenu);
  pinMode(LED_BUILTIN, OUTPUT);
}

void blink() {
  if (millis() / 500 % 2) {
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
}

#define NO_KEY '\0'
void TankControllerLib::loop() {
  blink();
  char key = NO_KEY;  // custom_keypad.getKey();
  if (key != NO_KEY) {
    Serial.print(F("To start key: "));
    Serial.println(key);
    _state->handleKey(key);
  }
}

void TankControllerLib::changeState(UIState* newState) {
  if (_state) {
    delete _state;
  }
  _state = newState;
}

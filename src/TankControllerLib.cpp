#include "TankControllerLib.h"

#ifdef MOCK_PINS_COUNT
#include <cassert>
#else
#define assert(p) (void)0
#endif

#include "UIState/MainMenu.h"
#include "UIState/UIState.h"

const char NO_KEY = '\0';  // Move to LiquidCrystal?

// ------------ Class Methods ------------
/**
 * static variable to hold singleton
 */
TankControllerLib* TankControllerLib::_instance = nullptr;

/**
 * static function to return singleton
 */
TankControllerLib* TankControllerLib::instance() {
  if (!_instance) {
    _instance = new TankControllerLib;
  }
  return _instance;
}

// ------------ Instance Methods ------------
/**
 * Constructor
 */
TankControllerLib::TankControllerLib() {
  assert(!_instance);
  state = nullptr;
  lcd = LiquidCrystal_TC::instance();
  log = Serial_TC::instance();
  log->print(F("TankControllerLib::TankControllerLib() - version "), TANK_CONTROLLER_VERSION);
}

/**
 * Destructor
 */
TankControllerLib::~TankControllerLib() {
  changeState(nullptr);
  assert(this == _instance);
  _instance = nullptr;
}

/**
 * Blink the on-board LED to let us know that loop() is being called
 *
 */
void TankControllerLib::blink() {
  if (millis() / 500 % 2) {
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
}

/**
 * Private member function called by UIState subclasses
 */
void TankControllerLib::changeState(UIState* newState) {
  if (state) {
    delete state;
  }
  state = newState;
}

/**
 * This is one of two public instance functions.
 * It is called repeatedly while the board is on.
 */
void TankControllerLib::loop() {
  blink();            //  blink the on-board LED to show that we are running
  char key = NO_KEY;  // custom_keypad.getKey();
  if (key != NO_KEY) {
    log->print(F("Keypad input: "), key);
    state->handleKey(key);
  }
}

/**
 * This is one of two public instance functions.
 * Here we do any one-time startup initialization.
 */
void TankControllerLib::setup() {
  log->print(F("TankControllerLib::setup()"));
  changeState((UIState*)new MainMenu);
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * What is the current version?
 */
const char* TankControllerLib::version() {
  log->print(F("TankControllerLib::version() = "), TANK_CONTROLLER_VERSION);
  return TANK_CONTROLLER_VERSION;
}

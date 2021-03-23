#include "TankControllerLib.h"

#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/SD_TC.h"
#include "Devices/Serial_TC.h"
#include "UIState/MainMenu.h"
#include "UIState/UIState.h"

// ------------ Class Methods ------------
/**
 * static variable to hold singleton
 */
TankControllerLib *TankControllerLib::_instance = nullptr;

/**
 * static function to return singleton
 */
TankControllerLib *TankControllerLib::instance() {
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
  state = new MainMenu(this);
  lcd = LiquidCrystal_TC::instance();
  log = Serial_TC::instance();
  log->print((const char *)F("TankControllerLib::TankControllerLib() - version "),
             (const char *)TANK_CONTROLLER_VERSION);
  SDClass_TC::instance()->printRootDirectory();
}

/**
 * Destructor
 */
TankControllerLib::~TankControllerLib() {
  delete state;
  delete nextState;
}

/**
 * Blink the on-board LED to let us know that loop() is being called
 *
 */
void TankControllerLib::blink() {
  if (millis() / 1000 % 2) {
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
}

/**
 * Private member function called by loop
 * Handles keypresses
 */
void TankControllerLib::handleUI() {
  // std::cout << "TankControllerLib::handleUI() - " << state->name() << std::endl;
  char key = Keypad_TC::instance()->getKey();
  if (key != NO_KEY) {
    log->print(F("Keypad input: "), key);
    // std::cout << "TankControllerLib::handleUI() - " << state->name() << "::handleKey(" << key << ")" << std::endl;
    state->handleKey(key);
  }
  updateState();
  // std::cout << "TankControllerLib::handleUI() - " << state->name() << "::loop()" << std::endl;
  state->loop();
}

/**
 * This is one of two public instance functions.
 * It is called repeatedly while the board is on.
 */
void TankControllerLib::loop() {
  // std::cout << "TankControllerLib::loop() for " << newState->name() << std::endl;
  blink();  //  blink the on-board LED to show that we are running
  handleUI();
}

/**
 * Set the next state
 */
void TankControllerLib::setNextState(UIState *newState, bool update) {
  // std::cout << "TankControllerLib::setNextState() to " << newState->name() << std::endl;
  assert(nextState == nullptr);
  nextState = newState;
  if (update) {
    this->updateState();
  }
}

/**
 * This is one of two public instance functions.
 * Here we do any one-time startup initialization.
 */
void TankControllerLib::setup() {
  log->print(F("TankControllerLib::setup()"));
  setNextState(((UIState *)new MainMenu(this)));
  updateState();
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * Private member function called by UIState subclasses
 * Only updates if a new state is available to switch to
 */
void TankControllerLib::updateState() {
  if (nextState) {
    // std::cout << "TankControllerLib::updateState() to " << nextState->name() << std::endl;
    assert(state != nextState);
    delete state;
    state = nextState;
    nextState = nullptr;
    state->start();
  }
}

/**
 * Public member function used to get the current state name.
 * This is primarily used by testing.
 */

String TankControllerLib::stateName() {
  return state->name();
}

/**
 * What is the current version?
 */
const char *TankControllerLib::version() {
  log->print((const char *)F("TankControllerLib::version() = "), (const char *)TANK_CONTROLLER_VERSION);
  return TANK_CONTROLLER_VERSION;
}

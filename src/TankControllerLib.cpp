#include "TankControllerLib.h"

#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/SD_TC.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"
#include "UIState/MainMenu.h"
#include "UIState/UIState.h"

const char TANK_CONTROLLER_VERSION[] = "0.3.0";

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
  log->printf((const char *)F("TankControllerLib::TankControllerLib() - version %s"),
              (const char *)TANK_CONTROLLER_VERSION);
  SD_TC::instance()->printRootDirectory();
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
  COUT("TankControllerLib::handleUI() - " << state->name());
  char key = Keypad_TC::instance()->getKey();
  if (key == NO_KEY) {
    // check for idle timeout and return to main menu
    if (lastKeypadTime && !nextState && (millis() - lastKeypadTime > IDLE_TIMEOUT)) {
      setNextState((UIState *)new MainMenu(this));
      lastKeypadTime = 0;  // so we don't do this until another keypress!
    }
  } else {
    log->printf((const char *)F("Keypad input: %c"), key);
    COUT("TankControllerLib::handleUI() - " << state->name() << "::handleKey(" << key << ")");
    state->handleKey(key);
    lastKeypadTime = millis();
  }
  updateState();
  COUT("TankControllerLib::handleUI() - " << state->name() << "::loop()");
  state->loop();
}

/**
 * This is one of two public instance functions.
 * It is called repeatedly while the board is on.
 */
void TankControllerLib::loop() {
  COUT("TankControllerLib::loop() for " << state->name());
  blink();  //  blink the on-board LED to show that we are running
  handleUI();
  // update TemperatureControl
  // update PHControl
  // write data to SD
  // write data to Google Sheets
}

/**
 * This public instance function is called when there is data on the serial port(0).
 */
void TankControllerLib::serialEvent() {
}

/**
 * This public instance function is called when there is data on the serial port(1).
 * This the Atlas EZO pH circuit probe.
 */
void TankControllerLib::serialEvent1() {
  PHProbe::instance()->serialEvent1();
}

/**
 * Set the next state
 */
void TankControllerLib::setNextState(UIState *newState, bool update) {
  COUT("TankControllerLib::setNextState() to " << newState->name());
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
  log->printf((const char *)F("TankControllerLib::setup()"));
  setNextState(((UIState *)new MainMenu(this)));
  updateState();
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * Public member function used to get the current state name.
 * This is primarily used by testing.
 */
const char *TankControllerLib::stateName() {
  return state->name();
}

/**
 * Private member function called by UIState subclasses
 * Only updates if a new state is available to switch to
 */
void TankControllerLib::updateState() {
  if (nextState) {
    COUT("TankControllerLib::updateState() to " << nextState->name());
    assert(state != nextState);
    delete state;
    state = nextState;
    nextState = nullptr;
    state->start();
  }
}

/**
 * What is the current version?
 */
const char *TankControllerLib::version() {
  log->printf((const char *)F("TankControllerLib::version() = %s"), (const char *)TANK_CONTROLLER_VERSION);
  return TANK_CONTROLLER_VERSION;
}

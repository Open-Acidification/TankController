#include "TankController.h"

#include <avr/wdt.h>
#include <stdlib.h>

#include "Devices/DataLogger_TC.h"
#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/EthernetServer_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/PushingBox.h"
#include "Devices/SD_TC.h"
#include "Devices/Serial_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TC_util.h"
#include "UIState/MainMenu.h"
#include "UIState/UIState.h"

const char TANK_CONTROLLER_VERSION[] = "23.06.0";

// ------------ Class Methods ------------
/**
 * static variable to hold singleton
 */
TankController *TankController::_instance = nullptr;

/**
 * static function to return singleton
 */
TankController *TankController::instance(const char *pushingBoxID) {
  if (!_instance) {
    _instance = new TankController;
    PushingBox::instance(pushingBoxID);
  }
  return _instance;
}

// ------------ Instance Methods ------------
/**
 * Constructor
 */
TankController::TankController() {
  serial(F("\r\n#################\r\nTankController::TankController() - version %s"), TANK_CONTROLLER_VERSION);
  assert(!_instance);
  // ensure we have instances
  SD_TC::instance();
  EEPROM_TC::instance();
  Keypad_TC::instance();
  LiquidCrystal_TC::instance(TANK_CONTROLLER_VERSION);
  DataLogger_TC::instance();
  DateTime_TC::rtc();
  Ethernet_TC::instance();
  TempProbe_TC::instance();
  TemperatureControl::instance();
  PHProbe::instance();
  PHControl::instance();
  state = new MainMenu(this);
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * Destructor
 */
TankController::~TankController() {
  if (state) {
    delete state;
    state = nullptr;
  }
  if (nextState) {
    delete nextState;
    nextState = nullptr;
  }
}

/**
 * Blink the on-board LED to let us know that loop() is being called
 *
 */
void TankController::blink() {
  if (millis() / 1000 % 2) {
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
}

// https://github.com/maniacbug/MemoryFree/blob/master/MemoryFree.cpp
int TankController::freeMemory() {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  return 1024;
#else
  extern char *__brkval;
  int topOfStack;

  return (int)((size_t)&topOfStack) - ((size_t)__brkval);
#endif
}

/**
 * Is the current UIState one that should disable controls?
 * We don't want to turn on the heat/chill if the temperature probe is out of the tank!
 */
bool TankController::isInCalibration() {
  return state->isInCalibration();
}

/**
 * Private member function called by loop
 * Handles keypresses
 */
void TankController::handleUI() {
  // Get server key, otherwise check for Keypad key
  char key = nextKey ? nextKey : Keypad_TC::instance()->getKey();
  nextKey = 0;
  if (key == NO_KEY) {
    if (!lastKeypadTime) {
      // we have already reached an idle state, so don't do other checks
    } else if (isInCalibration()) {
      // we are in calibration, so don't return to main menu
    } else if (nextState) {
      // we already have a next state teed-up, do don't try to return to main menu
    } else if (millis() - lastKeypadTime > IDLE_TIMEOUT) {
      // time since last keypress exceeds the idle timeout, so return to main menu
      setNextState((UIState *)new MainMenu(this));
      lastKeypadTime = 0;  // so we don't do this until another keypress!
    }
  } else {
    serial(F("Keypad input: %c"), key);
    COUT("TankController::handleUI() - " << state->name() << "::handleKey(" << key << ")");
    state->handleKey(key);
    lastKeypadTime = millis();
  }
  updateState();
  state->loop();
}

/**
 * This is one of two public instance functions.
 * It is called repeatedly while the board is on.
 * (It appears to be called about once every 15 ms.)
 */
void TankController::loop(bool report_loop_delay) {
  static unsigned long lastTime = 0;
  unsigned long thisTime = millis();
  if (report_loop_delay && lastTime && thisTime - lastTime > 500) {
    // report unusual delay
    serial(F("unexpected delay of %i ms"), thisTime - lastTime);
  }
  lastTime = thisTime;
  wdt_reset();
  blink();                                // blink the on-board LED to show that we are running
  updateControls();                       // turn CO2 and temperature controls on or off
  handleUI();                             // look at keypad, update LCD (~90ms)
  DataLogger_TC::instance()->loop();      // record current data to SD and serial
  PushingBox::instance()->loop();         // write data to Google Sheets (~1130ms every report)
  Ethernet_TC::instance()->loop();        // renew DHCP lease
  EthernetServer_TC::instance()->loop();  // handle any HTTP requests
}

/**
 * This public instance function is called when there is data on the serial port(0).
 */
void TankController::serialEvent() {
}

/**
 * This public instance function is called when there is data on the serial port(1).
 * This the Atlas EZO pH circuit probe.
 */
void TankController::serialEvent1() {
  PHProbe::instance()->serialEvent1();
}

/**
 * Set the next state
 */
void TankController::setNextState(UIState *newState, bool update) {
  COUT("TankController::setNextState() from " << (nextState ? (PGM_P)nextState->name() : "nullptr") << " to "
                                              << newState->name());
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
void TankController::setup() {
  serial(F("TankController::setup()"));
  serial(F("Free memory = %i"), freeMemory());
  wdt_enable(WDTO_8S);
}

/**
 * Public member function used to get the current state name.
 * This is primarily used by testing.
 */
const __FlashStringHelper *TankController::stateName() {
  return state->name();
}

/**
 * Private member function called by loop to update solonoids
 */
void TankController::updateControls() {
  // update TemperatureControl
  TemperatureControl::instance()->updateControl(TempProbe_TC::instance()->getRunningAverage());
  // update PHControl
  PHControl::instance()->updateControl(PHProbe::instance()->getPh());
}

/**
 * Private member function called by UIState subclasses
 * Only updates if a new state is available to switch to
 */
void TankController::updateState() {
  if (nextState) {
    COUT("TankController::updateState() to " << nextState->name());
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
const char *TankController::version() {
  return TANK_CONTROLLER_VERSION;
}

#if defined(__CYGWIN__)
size_t strnlen(const char *s, size_t n) {
  void *found = memchr(s, '\0', n);
  return found ? (size_t)((char *)found - s) : n;
}
#endif

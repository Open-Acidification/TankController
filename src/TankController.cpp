#include "TankController.h"

#include <avr/wdt.h>
#include <stdlib.h>

#include "UIState/MainMenu.h"
#include "UIState/UIState.h"
#include "Version.h"
#include "model/DataLogger.h"
#include "model/GetTime.h"
#include "model/PHControl.h"
#include "model/PHProbe.h"
#include "model/PushingBox.h"
#include "model/RemoteLogPusher.h"
#include "model/TC_util.h"
#include "model/ThermalControl.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/EthernetServer_TC.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/Keypad_TC.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/PID_TC.h"
#include "wrappers/SD_TC.h"
#include "wrappers/Serial_TC.h"
#include "wrappers/ThermalProbe_TC.h"

const char TANK_CONTROLLER_VERSION[] = VERSION;

// ------------ Class Methods ------------
/**
 * static variable to hold singleton
 */
TankController *TankController::_instance = nullptr;

/**
 * static function to return singleton
 */
TankController *TankController::instance(const char *remoteLogName, const char *pushingBoxID, int tzOffsetHrs) {
  if (!_instance) {
    serial(F("\r\n##############\r\nTankController %s"), TANK_CONTROLLER_VERSION);
    _instance = new TankController();
    unsigned long start = millis();
    SD_TC::instance()->setRemoteLogName(remoteLogName);
    EEPROM_TC::instance();
    Keypad_TC::instance();
    LiquidCrystal_TC::instance(TANK_CONTROLLER_VERSION);
    DataLogger::instance();
    DateTime_TC::rtc();
    Ethernet_TC::instance();
    EthernetServer_TC::instance();
    ThermalProbe_TC::instance();
    ThermalControl::instance();
    PHProbe::instance();
    PHControl::instance();
    PID_TC::instance();
    pinMode(LED_BUILTIN, OUTPUT);
    _instance->state = new MainMenu();
    PushingBox::instance(pushingBoxID);
    GetTime::instance(tzOffsetHrs);
    serial(F("Free memory = %i"), _instance->freeMemory());
    wdt_enable(WDTO_8S);
    serial(F("TankController::instance() - took %lu ms"), millis() - start);
  }
  return _instance;
}

/**
 * static function to delete singleton
 */
void TankController::deleteInstance() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

// ------------ Instance Methods ------------
/**
 * Constructor
 */
TankController::TankController() {
  assert(!_instance);
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
      setNextState((UIState *)new MainMenu());
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
 * This is one of two public instance functions (the other is setup()).
 * It is called repeatedly while the board is on.
 * We monitor two elapsed times:
 *   1. the time it takes to run the loop() function (our code); and,
 *   2. the time from one loop start to the next (including the board delay).
 */
void TankController::loop(bool report_loop_delay) {
  static unsigned long previousLoopStart = 0;
  unsigned long currentLoopStart = millis();
  if (report_loop_delay && previousLoopStart && currentLoopStart - previousLoopStart > 500) {
    serial(F("unexpected overall delay of %i ms (at %lu sec uptime)"), currentLoopStart - previousLoopStart,
           millis() / 1000);
  }
  wdt_reset();
  blink();                                // blink the on-board LED to show that we are running (0ms)
  updateControls();                       // turn CO2 and temperature controls on or off (~90ms)
  handleUI();                             // look at keypad, update LCD (~10ms)
  DataLogger::instance()->loop();         // record current data to SD and serial (~80ms)
  RemoteLogPusher::instance()->loop();    // write data to remote log
  GetTime::instance()->loop();            // update the time (~0ms)
  PushingBox::instance()->loop();         // write data to Google Sheets (~0ms; ~1130ms every report)
  Ethernet_TC::instance()->loop();        // renew DHCP lease (~0ms)
  EthernetServer_TC::instance()->loop();  // handle any HTTP requests (~0ms)
  if (report_loop_delay) {
    static long int count = 0;
    unsigned long currentLoopTime = millis() - currentLoopStart;
    if (++count % 100000 == 1 || currentLoopTime > 500) {  // first time through and periodically thereafter
      serial(F("TankController::loop() - took %lu ms (at %lu sec uptime)"), currentLoopTime, millis() / 1000);
    }
  }
  previousLoopStart = currentLoopStart;
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
                                              << (PGM_P)newState->name());
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
  // all the setup happens in the instance() function
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
  // update ThermalControl
  ThermalControl::instance()->updateControl(ThermalProbe_TC::instance()->getRunningAverage());
  // update PHControl
  PHControl::instance()->loop();
}

/**
 * Private member function called by UIState subclasses
 * Only updates if a new state is available to switch to
 */
void TankController::updateState() {
  if (nextState) {
    COUT("TankController::updateState() to " << (PGM_P)nextState->name());
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

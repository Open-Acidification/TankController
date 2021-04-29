#include "TankControllerLib.h"

#include "Devices/DateTime_TC.h"
#include "Devices/Keypad_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PID_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/SD_TC.h"
#include "Devices/Serial_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
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
    if (!calibrationMode && lastKeypadTime && !nextState && (millis() - lastKeypadTime > IDLE_TIMEOUT)) {
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
  blink();           // blink the on-board LED to show that we are running
  handleUI();        // look at keypad, update LCD
  updateControls();  // turn CO2 and temperature controls on or off
  writeDataToSD();   // record current state to data log
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
 * When in calibration mode we don't return to the idle screen and
 * we don't do any tank control actions.
 */
void TankControllerLib::setCalibrationMode(bool flag) {
  calibrationMode = flag;
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
 * Private member function called by loop to update solonoids
 */
void TankControllerLib::updateControls() {
  if (calibrationMode) {
    return;
  }
  // update TemperatureControl
  TemperatureControl::instance()->updateControl(TempProbe_TC::instance()->getRunningAverage());
  // update PHControl
  PHControl::instance()->updateControl(PHProbe::instance()->getPh());
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

/**
 * once per second write the current data to the SD card
 */
void TankControllerLib::writeDataToSD() {
  static unsigned long nextWriteTime = 0;
  static const char header[] = "time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd";
  static const char format[] =
      "%02i/%02i/%4i %02i:%02i:%02i, %3i, %2.3f, %2.3f, %1.4f, %1.4f, %4i, %5.1f, %5.1f, %5.1f";
  unsigned long msNow = millis();
  COUT("nextWriteTime: " << nextWriteTime << "; now = " << msNow);
  if (nextWriteTime <= msNow) {
    char buffer[128];
    DateTime_TC dtNow = DateTime_TC::now();
    PID_TC *pPID = PID_TC::instance();
    int tankId = 0;
    snprintf(buffer, sizeof(buffer), format, (int)dtNow.month(), (int)dtNow.day(), (int)dtNow.year(), (int)dtNow.hour(),
             (int)dtNow.minute(), (int)dtNow.second(), (int)tankId,
             (double)TempProbe_TC::instance()->getRunningAverage(),
             (double)TemperatureControl::instance()->getTargetTemperature(), (double)PHProbe::instance()->getPh(),
             (double)PHControl::instance()->getTargetPh(), (int)0, (double)pPID->getKp(), (double)pPID->getKi(),
             (double)pPID->getKd());  // still missing onTime
    SD_TC::instance()->appendToDataLog(header, buffer);
    nextWriteTime = msNow / 1000 * 1000 + 1000;  // round up to next second
    COUT(buffer);
  }
}

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "PHControl.h"
#include "ThermalControl.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "PHCalibrationPrompt.h"
#include "TankController.h"
#include "ThermalProbe_TC.h"

// globals for the singletons used in every test
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop(false);  // recognize and apply the key entry
}

unittest_setup() {
  PHControl::instance()->setBaseTargetPh(8.100);
  EEPROM_TC::instance()->setPh(8.100);
  tc->setNextState(new MainMenu(), true);
  ThermalControl::enableHeater(true);
  ThermalControl::instance()->setThermalTarget(15.75);
  ThermalProbe_TC::instance()->setTemperature(12.25, true);
  tc->loop(false);  // recognize and apply the targets
  enterKey('D');
}

unittest_teardown() {
  enterKey('D');
}

/**
 * 2 goes up in the menu
 * 8 goes down
 * 6 goes into the current menu option
 * 4 backs out of the current option
 */

unittest(MainMenu) {
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.23 H 15.75 ", lc->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("pH 0.000   8.100", lc->getLines().at(0));
  assertEqual("T 12.23 H 15.75 ", lc->getLines().at(1));
  delay(1000);
  tc->loop(false);
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.23 H 15.75 ", lc->getLines().at(1));
}

unittest(ChangeSettings) {
  enterKey('2');
  assertEqual("Change settings ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.25 H 15.75 ", lc->getLines().at(1));
  enterKey('8');
  enterKey('8');
  assertEqual("Change settings ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('4');
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.25 H 15.75 ", lc->getLines().at(1));
}

unittest(ViewSettings) {
  enterKey('8');
  assertEqual("View settings   ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.25 H 15.75 ", lc->getLines().at(1));
  enterKey('2');
  enterKey('2');
  assertEqual("View settings   ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('D');
  assertEqual("pH=0.000   8.100", lc->getLines().at(0));
  assertEqual("T=12.25 H 15.75 ", lc->getLines().at(1));
}

unittest(SetPHTarget) {
  enterKey('A');
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
  enterKey('2');
  enterKey('6');
  assertEqual("Set pH target   ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('6');
  assertEqual("Set pH Set Point", lc->getLines().at(0));
  enterKey('D');
  assertEqual("MainMenu", tc->stateName());
}

unittest(ViewTime) {
  enterKey('8');
  enterKey('6');
  assertEqual("View time       ", lc->getLines().at(0));
  assertEqual("<4   ^2  8v   6>", lc->getLines().at(1));
  enterKey('6');
  assertEqual(DateTime_TC::now().as16CharacterString(), lc->getLines().at(0).c_str());
  delay(6000);
  tc->loop(false);
  assertEqual("SeeDeviceUptime", tc->stateName());
  delay(55000);  // idle timeout should return to main menu
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest(DisableTimeout) {
  enterKey('8');
  enterKey('8');
  enterKey('6');
  enterKey('8');
  enterKey('8');
  enterKey('6');
  assertEqual("PHCalibrationPrompt", tc->stateName());
  delay(65000);  // wait for over 60 seconds to verify that it does not return to main menu
  tc->loop(false);
  assertEqual("PHCalibrationPrompt", tc->stateName());
}

unittest_main()

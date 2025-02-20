#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHControl.h"
#include "PHProbe.h"
#include "SeePh.h"
#include "TankController.h"
#include "UIState/UIState.h"

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();
PHControl *controlSolenoid = PHControl::instance();
PHProbe *pHProbe = PHProbe::instance();
LiquidCrystal_TC *lc = LiquidCrystal_TC::instance();

unittest_setup() {
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
}

unittest(TestVerticalScrollWithFlatSet) {
  pHProbe->setPh(7.062);
  controlSolenoid->setBaseTargetPh(7.062);
  SeePh *test = new SeePh();

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(1000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(2000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithRampSet) {
  pHProbe->setPh(8.50);
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setRampDurationHours(0.005);  // 18 seconds
  SeePh *test = new SeePh();

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.50 8.500 7.000", lc->getLines().at(1));
  delay(1000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.50 8.417 7.000", lc->getLines().at(1));
  delay(2000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:15    ", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.25 8.000 7.000", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:9     ", lc->getLines().at(1));
  delay(1000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:8     ", lc->getLines().at(1));
  delay(8000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.67 7.000 7.000", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:0     ", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.000 7.000", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithSineSet) {
  pHProbe->setPh(8.50);
  controlSolenoid->setBaseTargetPh(7.00);
  controlSolenoid->setSineAmplitudeAndHours(1.5, 0.125);
  SeePh *test = new SeePh();

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.000 7.000", lc->getLines().at(1));
  delay(1000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.021 7.000", lc->getLines().at(1));
  delay(2000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.126 7.000", lc->getLines().at(1));
  delay(3000);
  pHProbe->setPh(controlSolenoid->getCurrentTargetPh());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

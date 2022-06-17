#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "MainMenu.h"
#include "SeeTemperature.h"
#include "TankController.h"
#include "TempProbe_TC.h"
#include "TemperatureControl.h"
#include "UIState/UIState.h"

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();
TemperatureControl *control = TemperatureControl::instance();
LiquidCrystal_TC *lc = LiquidCrystal_TC::instance();

void reset() {
  tc->setNextState(new MainMenu(tc), true);
  state->resetClock();
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
  control->setTargetTemperature(7.00);
}

unittest_setup() {
  reset();
  TempProbe_TC::instance()->setTemperature(10.00);
  TempProbe_TC::instance()->setCorrection(0.0);
  for (size_t i = 0; i < 100; ++i) {
    delay(1000);
    TempProbe_TC::instance()->getRunningAverage();
  }
  delay(1000);
}

unittest_teardown() {
  reset();
}

unittest(TestVerticalScrollWithFlatSet) {
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  tc->loop();

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(2000);
  tc->loop();
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
}

unittest(TestVerticalScrollWithRampSet) {
  control->setRampDuration(0.005);  // 18 seconds
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  tc->loop();

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(2000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:15    ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 9.50 7.00  ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:9     ", lc->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:8     ", lc->getLines().at(1));
  delay(8000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 8.34 7.00  ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:0     ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
}

unittest(TestVerticalScrollWithSineSet) {
  control->setSine(1.5, 0.125);
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  tc->loop();

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(1000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.00 7.00  ", lc->getLines().at(1));
  delay(2000);
  tc->loop();
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 7.06 7.00  ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));
}

unittest_main()

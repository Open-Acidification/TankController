#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeeTemperature.h"
#include "TankController.h"
#include "TempProbe_TC.h"
#include "TemperatureControl.h"
#include "UIState/UIState.h"

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();
TemperatureControl *control = TemperatureControl::instance();
LiquidCrystal_TC *lc = LiquidCrystal_TC::instance();

unittest_setup() {
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
}

void setTempMeasurementTo(float value) {
  TempProbe_TC::instance()->setTemperature(value);
  tc->loop();
}

unittest(TestVerticalScrollWithFlatSet) {
  setTempMeasurementTo(10.06);
  control->setTargetTemperature(10.06);
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  // Set up
  setTempMeasurementTo(control->getCurrentTemperatureTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.1 10.06 10.06", lc->getLines().at(1));
  delay(1000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.1 10.06 10.06", lc->getLines().at(1));
  delay(2000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("10.1 10.06 10.06", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.1 10.06 10.06", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("10.1 10.06 10.06", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithRampSet) {
  setTempMeasurementTo(10.00);
  control->setTargetTemperature(7.00);
  control->setRampDuration(0.005);  // 18 seconds
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  // Set up
  setTempMeasurementTo(control->getCurrentTemperatureTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(1000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00", lc->getLines().at(1));
  delay(2000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:15    ", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:9     ", lc->getLines().at(1));
  delay(1000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:8     ", lc->getLines().at(1));
  delay(8000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(3000);
  tc->loop();
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:0     ", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithSineSet) {
  setTempMeasurementTo(10);
  control->setTargetTemperature(7.00);
  control->setSine(1.5, 0.125);
  SeeTemperature *test = new SeeTemperature(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeeTemperature nextState: Wait
  tc->loop();
  assertEqual("SeeTemperature", tc->stateName());

  // Set up
  setTempMeasurementTo(control->getCurrentTemperatureTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(1000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(2000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("10.0 10.00 7.00 ", lc->getLines().at(1));
  delay(3000);
  setTempMeasurementTo(control->getCurrentTemperatureTarget());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

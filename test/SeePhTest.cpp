#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/DateTime_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHControl.h"
#include "SeePh.h"
#include "TankController.h"
#include "UIState/UIState.h"

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();
PHControl *controlSolenoid = PHControl::instance();
LiquidCrystal_TC *lc = LiquidCrystal_TC::instance();

unittest_setup() {
  DateTime_TC january(2021, 1, 15, 1, 48, 24);
  january.setAsCurrent();
}

void setPhMeasurementTo(float value) {
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%03i\r"), (int)value, (int)(value * 1000 + 0.5) % 1000);
  state->serialPort[1].dataIn = buffer;  // the queue of data waiting to be read
  tc->serialEvent1();                    // fake interrupt to update the current pH reading
  tc->loop(false);                       // update the controls based on the current readings
}

unittest(TestVerticalScrollWithFlatSet) {
  setPhMeasurementTo(7.062);
  controlSolenoid->setTargetPh(7.062);
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop(false);
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: flat      ", lc->getLines().at(0));
  assertEqual("7.06 7.062 7.062", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithRampSet) {
  setPhMeasurementTo(8.50);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->setRampDuration(0.005);  // 18 seconds
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop(false);
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.50 8.500 7.000", lc->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.50 8.417 7.000", lc->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:15    ", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("8.25 8.000 7.000", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:9     ", lc->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:8     ", lc->getLines().at(1));
  delay(8000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.67 7.000 7.000", lc->getLines().at(1));
  delay(3000);
  tc->loop(false);
  assertEqual("type: ramp      ", lc->getLines().at(0));
  assertEqual("left: 0:0:0     ", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.000 7.000", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithSineSet) {
  setPhMeasurementTo(8.50);
  controlSolenoid->setTargetPh(7.00);
  controlSolenoid->setSine(1.5, 0.125);
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop(false);
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.000 7.000", lc->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.00 7.021 7.000", lc->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Now  Next  Goal ", lc->getLines().at(0));
  assertEqual("7.06 7.126 7.000", lc->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: sine      ", lc->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", lc->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

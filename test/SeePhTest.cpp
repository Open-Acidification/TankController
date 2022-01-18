#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHControl.h"
#include "SeePh.h"
#include "TankController.h"
#include "UIState/UIState.h"

GodmodeState *state = GODMODE();
TankController *tc = TankController::instance();

void setPhMeasurementTo(float value) {
  char buffer[10];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%i\r"), (int)value, (int)(value * 1000 + 0.5) % 1000);
  state->serialPort[1].dataIn = buffer;  // the queue of data waiting to be read
  tc->serialEvent1();                    // fake interrupt to update the current pH reading
  tc->loop();                            // update the controls based on the current readings
}

unittest(TestVerticalScrollWithFlatSet) {
  EEPROM_TC::instance()->setPh(7.00);  // targetPh
  // mock arduino restarting so values get read from eeprom
  PHControl::clearInstance();
  PHControl *controlSolenoid = PHControl::instance();
  LiquidCrystal_TC *display = LiquidCrystal_TC::instance();
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: flat      ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(3000);
  float fakePh = controlSolenoid->getCurrentPhTarget();
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: flat      ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithRampSet) {
  EEPROM_TC::instance()->setPh(7.00);             // targetPh
  PHControl::instance()->setRampDuration(0.125);  // 7.5 min.
  EEPROM_TC::instance()->setRampStartingPh(8.5);
  // mock arduino restarting so values get read from eeprom
  PHControl::clearInstance();
  PHControl *controlSolenoid = PHControl::instance();
  LiquidCrystal_TC *display = LiquidCrystal_TC::instance();
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=8.500 ", display->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=8.500 ", display->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: ramp      ", display->getLines().at(0));
  assertEqual("hrs left: 0.120 ", display->getLines().at(1));
  delay(3000);
  float fakePh = controlSolenoid->getCurrentPhTarget();
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=8.490 ", display->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: ramp      ", display->getLines().at(0));
  assertEqual("hrs left: 0.118 ", display->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(TestVerticalScrollWithSineSet) {
  EEPROM_TC::instance()->setPh(7.00);  // targetPh
  PHControl::instance()->setSine(1.5, 0.125);
  // mock arduino restarting so values get read from eeprom
  PHControl::clearInstance();
  PHControl *controlSolenoid = PHControl::instance();
  LiquidCrystal_TC *display = LiquidCrystal_TC::instance();
  SeePh *test = new SeePh(tc);

  // Transition states
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);  // MainMenu -> SeePh nextState: Wait
  tc->loop();
  assertEqual("SeePh", tc->stateName());

  // Set up
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());

  // during the delay we cycle through displays
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(1000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.0   ", display->getLines().at(1));
  delay(2000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: sine      ", display->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", display->getLines().at(1));
  delay(3000);
  float fakePh = controlSolenoid->getCurrentPhTarget();
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("Target pH=7.0   ", display->getLines().at(0));
  assertEqual("Actual pH=7.630 ", display->getLines().at(1));
  delay(3000);
  setPhMeasurementTo(controlSolenoid->getCurrentPhTarget());
  assertEqual("type: sine      ", display->getLines().at(0));
  assertEqual("p=0.125 a=1.500 ", display->getLines().at(1));

  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

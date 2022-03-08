#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"
#include "EEPROM_TC.h"
#include "SetTempWithSine.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->resetEEPROM();
}

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  EEPROM_TC::instance()->setTemp(20.00);
  TankController* tc = TankController::instance();  // instantiate after setting eeprom stuff
  assertEqual(20.00, EEPROM_TC::instance()->getTemp());
  assertEqual(20.00, TemperatureControl::instance()->getTargetTemperature());
  SetTempWithSine* test = new SetTempWithSine(tc);
  tc->setNextState(test, true);

  assertEqual(EEPROM_TC::instance()->getTempSetType(), TemperatureControl::instance()->tempSetTypeTypes::FLAT_TYPE);
  assertEqual(20, TemperatureControl::instance()->getTargetTemperature());

  // get currently displayed lines
  std::vector<String> lines = lcd->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set T Set Point ", lines.at(0));
  assertEqual(" 20.00->     0  ", lines.at(1));
  assertEqual(20, test->getCurrentValue());

  // setValues
  test->setValue(25.00);

  lines = lcd->getLines();
  assertEqual("Set Amplitude:  ", lines.at(0));
  assertEqual("    0.0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(2.12);

  lines = lcd->getLines();
  assertEqual("Set Period hrs: ", lines.at(0));
  assertEqual("      0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(1.5);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual(25, TemperatureControl::instance()->getTargetTemperature());
  assertEqual(25, EEPROM_TC::instance()->getTemp());
  assertEqual(TemperatureControl::instance()->tempSetTypeTypes::SINE_TYPE, EEPROM_TC::instance()->getTempSetType());

  assertEqual("New Temp=25.00  ", lines[0]);
  assertEqual("A=2.12 P=1.500  ", lines[1]);
  assertEqual("SetTempWithSine", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

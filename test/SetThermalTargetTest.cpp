#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "LiquidCrystal_TC.h"
#include "SetThermalTarget.h"
#include "TankController.h"
#include "ThermalControl.h"

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  EEPROM_TC::instance()->setTemp(20.00);
  EEPROM_TC::instance()->setThermalRampTimeEnd(0);
  EEPROM_TC::instance()->setThermalRampTimeStart(0);
  TankController* tc = TankController::instance();  // instantiate after setting eeprom stuff
  assertEqual(ThermalControl::instance()->thermalFunctionTypes::FLAT_TYPE,
              ThermalControl::instance()->getThermalFunctionType());
  assertEqual(20.00, EEPROM_TC::instance()->getThermalTarget());
  assertEqual(0, 20.00 - EEPROM_TC::instance()->getThermalTarget());
  assertEqual(20.00, ThermalControl::instance()->getBaseThermalTarget());
  assertEqual(0, EEPROM_TC::instance()->getThermalRampTimeEnd());
  assertEqual(0, EEPROM_TC::instance()->getThermalRampTimeStart());
  assertEqual("MainMenu", tc->stateName());
  SetThermalTarget* test = new SetThermalTarget();
  tc->setNextState(test, true);

  // get currently displayed lines
  std::vector<String> lines = lcd->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set Temperature ", lines.at(0));
  assertEqual("  20.00->     0 ", lines.at(1));

  // getCurrentValue
  assertEqual(20, test->getCurrentValue());

  // setValue
  test->setValue(50.25);

  lines = lcd->getLines();
  assertEqual("Set ramp hours: ", lines.at(0));
  assertEqual("    0.0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(4.125);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual(50.25, ThermalControl::instance()->getBaseThermalTarget());
  assertEqual(ThermalControl::instance()->thermalFunctionTypes::RAMP_TYPE,
              ThermalControl::instance()->getThermalFunctionType());
  assertEqual(50.25, EEPROM_TC::instance()->getThermalTarget());

  // during the delay we showed the new value
  assertEqual("New Temp=50.25  ", lines[0]);
  assertEqual("New ramp=4.125  ", lines[1]);
  assertEqual("SetThermalTarget", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

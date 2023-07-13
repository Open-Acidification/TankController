#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TemperatureControl.h"
#include "EEPROM_TC.h"
#include "SetTempSetPoint.h"
#include "TankController.h"

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  EEPROM_TC::instance()->setTemp(20.00);
  EEPROM_TC::instance()->setTempRampTimeEnd(0);
  EEPROM_TC::instance()->setTempRampTimeStart(0);
  TankController* tc = TankController::instance();  // instantiate after setting eeprom stuff
  assertEqual(TemperatureControl::instance()->tempSetTypeTypes::FLAT_TYPE,
              TemperatureControl::instance()->getTempSetType());
  assertEqual(20.00, EEPROM_TC::instance()->getTemp());
  assertEqual(0, 20.00 - EEPROM_TC::instance()->getTemp());
  assertEqual(20.00, TemperatureControl::instance()->getBaseTargetTemperature());
  assertEqual(0, EEPROM_TC::instance()->getTempRampTimeEnd());
  assertEqual(0, EEPROM_TC::instance()->getTempRampTimeStart());
  assertEqual("MainMenu", tc->stateName());
  SetTempSetPoint* test = new SetTempSetPoint(tc);
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
  assertEqual(50.25, TemperatureControl::instance()->getBaseTargetTemperature());
  assertEqual(TemperatureControl::instance()->tempSetTypeTypes::RAMP_TYPE,
              TemperatureControl::instance()->getTempSetType());
  assertEqual(50.25, EEPROM_TC::instance()->getTemp());

  // during the delay we showed the new value
  assertEqual("New Temp=50.25  ", lines[0]);
  assertEqual("New ramp=4.125  ", lines[1]);
  assertEqual("SetTempSetPoint", tc->stateName());
  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // queue MainMenu to be next
  tc->loop(false);  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

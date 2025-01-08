#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHCalibrationHigh.h"
#include "TankController.h"

unittest(test) {
  TankController* tc = TankController::instance();
  PHCalibrationHigh* test = new PHCalibrationHigh();
  tc->setNextState(test, true);
  assertTrue(tc->isInCalibration());
  // setValue
  test->setValue(12.345);
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("High = 12.345   ", lines[1]);
  assertEqual("PHCalibrationHigh", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(2000);
  assertTrue(tc->isInCalibration());
  delay(1000);
  tc->loop();  // after the delay, Wait will call setNextState to prepare to go to PHCalibrationLow
  tc->loop();  // updateState to PHCalibrationLow
  assertEqual("PHCalibrationLow", tc->stateName());
  assertTrue(tc->isInCalibration());
}

unittest_main()

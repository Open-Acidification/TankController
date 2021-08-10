#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "EnablePID.h"
#include "TankControllerLib.h"

unittest(IgnoreInvalidCharacter) {
  TankControllerLib* tc = TankControllerLib::instance();
  EnablePID* test = new EnablePID(tc);
  tc->setNextState(test, true);
  assertEqual("EnablePID", tc->stateName());
  assertTrue(PHControl::instance()->getUsePID());
  test->setValue(2.0);
  assertEqual("EnablePID", tc->stateName());
  assertTrue(PHControl::instance()->getUsePID());
}

unittest(DisablePID) {
  TankControllerLib* tc = TankControllerLib::instance();
  EnablePID* test = new EnablePID(tc);
  tc->setNextState(test, true);
  assertEqual("EnablePID", tc->stateName());
  test->setValue(9.0);
  assertFalse(PHControl::instance()->getUsePID());
  assertEqual("EnablePID", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // after the delay, Wait will call setNextState
  tc->loop();  // now transition back to main
  assertEqual("MainMenu", tc->stateName());
}

unittest(EnablePID) {
  TankControllerLib* tc = TankControllerLib::instance();
  EnablePID* test = new EnablePID(tc);
  tc->setNextState(test, true);
  assertEqual("EnablePID", tc->stateName());
  test->setValue(1.0);
  assertTrue(PHControl::instance()->getUsePID());
  assertEqual("EnablePID", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // after the delay, Wait will call setNextState
  tc->loop();  // now transition back to main
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

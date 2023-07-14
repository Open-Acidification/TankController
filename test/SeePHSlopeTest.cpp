#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/PHProbe.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SeePHSlope.h"
#include "TankController.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  tc->serialEvent1();  // fake interrupt

  assertEqual("MainMenu", tc->stateName());
  SeePHSlope* test = new SeePHSlope(tc);
  tc->setNextState(test, true);
  assertEqual("SeePHSlope", tc->stateName());

  // Test the output
  assertEqual("PH Slope:       ", display->getLines().at(0));
  assertEqual("requesting slope", display->getLines().at(1));
  tc->loop(false);
  assertEqual("Slope requested!", display->getLines().at(1));
  PHProbe::instance()->setPhSlope();
  // GODMODE()->serialPort[1].dataIn = "?SLOPE,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  // tc->serialEvent1();                                             // fake interrupt
  // tc->loop(false);
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop(false);
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

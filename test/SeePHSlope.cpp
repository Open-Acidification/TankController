#include "SeePHSlope.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "TankControllerLib.h"

unittest(testOutput) {
  // Set up
  TankControllerLib* tc = TankControllerLib::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  GODMODE()->serialPort[1].dataIn = "?Slope,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  tc->serialEvent1();                                             // fake interrupt

  assertEqual("MainMenu", tc->stateName());
  SeePHSlope* test = new SeePHSlope(tc);
  tc->setNextState(test, true);
  assertEqual("SeePHSlope", tc->stateName());

  // Test the output
  assertEqual("PH Slope:       ", display->getLines().at(0));
  assertEqual("requesting slope", display->getLines().at(1));
  tc->loop();
  assertEqual("Slope requested!", display->getLines().at(1));
  GODMODE()->serialPort[1].dataIn = "?Slope,99.7,100.3,-0.89\r";  // the queue of data waiting to be read
  tc->serialEvent1();                                             // fake interrupt
  tc->loop();
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()

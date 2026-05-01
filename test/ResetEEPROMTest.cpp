/**
 * ResetEEPROMTest.cpp
 *
 * Tests for the ResetEEPROM UIState. These tests cover the behavior reachable
 * from outside the 'A'/'D' branches: prompt rendering, state identity, and
 * the no-op handling of every other key.
 *
 * The 'A' and 'D' branches are intentionally NOT exercised here: both end in
 *   wdt_enable(WDTO_15MS);
 *   do { } while (true);
 * which the arduino_ci mocks cannot break out of -- entering them would hang
 * the test process. Those branches are validated by manual hardware testing.
 */
#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "ResetEEPROM.h"
#include "TankController.h"

// globals for the singletons used in every test
TankController* tc = TankController::instance();
LiquidCrystal_TC* lc = LiquidCrystal_TC::instance();
Keypad* keypad = Keypad_TC::instance()->_getPuppet();

// reduce duplicate code and make it more explicit
void enterKey(char key) {
  keypad->push_back(key);
  tc->loop();  // recognize and apply the key entry
}

unittest_setup() {
  tc->setNextState(new ResetEEPROM(), true);
}

// 1) Prompt is rendered on entry: line 0 is the question, line 1 is the cancel hint.
unittest(promptIsRenderedOnEntry) {
  assertEqual("A: Erase EEPROM ", lc->getLines().at(0));
  assertEqual("D: Cancel       ", lc->getLines().at(1));
}

// 2) State identity: stateName() reports "ResetEEPROM" while in this state.
unittest(stateNameIsResetEEPROM) {
  assertEqual("ResetEEPROM", tc->stateName());
}

// 3) isInCalibration() returns true so background controllers/writers stay
//    quiet while the user is deciding whether to wipe the EEPROM.
unittest(isInCalibrationIsTrue) {
  assertTrue(tc->isInCalibration());
}

// 4) Keys other than 'A' and 'D' are no-ops: the state and the on-screen
//    prompt are both unchanged. This pins down the current `default: break;`
//    behavior so a future edit can't silently add (e.g.) a returnToMainMenu()
//    that would let an accidental keypress drop the user out of the prompt.
unittest(unhandledKeysAreNoOps) {
  const char unhandledKeys[] = {'0', '1', '2', '3', '4', '5', '6',
                                '7', '8', '9', 'B', 'C', '*', '#'};
  for (size_t i = 0; i < sizeof(unhandledKeys); ++i) {
    enterKey(unhandledKeys[i]);
    assertEqual("ResetEEPROM", tc->stateName());
    assertEqual("A: Erase EEPROM ", lc->getLines().at(0));
    assertEqual("D: Cancel       ", lc->getLines().at(1));
  }
}

unittest_main()

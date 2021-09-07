#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "NumberCollectorState.h"
#include "TankController.h"

unittest(Edge_Case) {
  TestNumCollectorState test(TankController::instance());
  test.handleKey('9');
  test.handleKey('0');
  test.handleKey('*');
  test.handleKey('2');
  test.handleKey('5');
  assertEqual(90.25, test.getValue());
}

unittest(A_Digit_NoDecimal) {
  TestNumCollectorState testNoDecimal(TankController::instance());
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('3');
  assertEqual(123, testNoDecimal.getValue());
}

unittest(A_Digit_WithDecimal) {
  TestNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('7');
  testDecimal.handleKey('5');
  assertEqual(12.75, testDecimal.getValue());
}

unittest(A_Digit_MultipleDecimals) {
  TestNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('*');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('5');
  assertEqual(12.25, testDecimal.getValue());
}

unittest(backSpace) {
  TestNumCollectorState testNoDecimal(TankController::instance());
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('B');
  assertEqual(1, testNoDecimal.getValue());

  TestNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('*');
  testDecimal.handleKey('B');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('5');
  testDecimal.handleKey('3');
  testDecimal.handleKey('B');
  assertEqual(12.5, testDecimal.getValue());
}

unittest(clear) {
  TestNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('C');
  assertEqual(0, testDecimal.getValue());
}

unittest(accept) {
  TestNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('3');
  assertEqual(0, testDecimal.getStoredValue());
  testDecimal.handleKey('A');
  assertEqual(123, testDecimal.getStoredValue());
}

unittest(printing) {
  LiquidCrystal_TC* testLcd = LiquidCrystal_TC::instance();
  std::vector<String> lines;
  TestNumCollectorState test(TankController::instance());
  test.start();
  lines = testLcd->getLines();
  assertEqual("      0->     0 ", lines.at(1));

  test.setPriorValue(34.125);
  test.setPriorValuePrecision(1);
  test.handleKey('2');
  lines = testLcd->getLines();
  assertEqual("   34.1->     2 ", lines.at(1));

  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual("   34.1->    21 ", lines.at(1));

  test.handleKey('*');
  lines = testLcd->getLines();
  assertEqual("   34.1->    21.", lines.at(1));

  test.handleKey('3');
  lines = testLcd->getLines();
  assertEqual("   34.1->   21.3", lines.at(1));

  test.setPriorValuePrecision(2);
  test.handleKey('4');
  lines = testLcd->getLines();
  assertEqual("  34.12->  21.34", lines.at(1));

  test.setPriorValuePrecision(3);
  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual(" 34.125-> 21.341", lines.at(1));
}

unittest(integer) {
  TestIntNumCollectorState testDecimal(TankController::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('3');
  testDecimal.handleKey('4');
  assertEqual(1234, testDecimal.getValue());
}

unittest_main()

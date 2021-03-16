#include "NumberCollectorState.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "TankControllerLib.h"

unittest(Edge_Case) {
  TestNumCollectorState test(TankControllerLib::instance());
  test.handleKey('9');
  test.handleKey('0');
  test.handleKey('*');
  test.handleKey('0');
  test.handleKey('9');
  assertEqual(90.09, test.getValue());
}

unittest(A_Digit_NoDecimal) {
  TestNumCollectorState testNoDecimal(TankControllerLib::instance());
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('3');
  assertEqual(123, testNoDecimal.getValue());
}

unittest(A_Digit_WithDecimal) {
  TestNumCollectorState testDecimal(TankControllerLib::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('3');
  testDecimal.handleKey('4');
  assertEqual(12.34, testDecimal.getValue());
}

unittest(A_Digit_MultipleDecimals) {
  TestNumCollectorState testDecimal(TankControllerLib::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('*');
  testDecimal.handleKey('3');
  testDecimal.handleKey('*');
  testDecimal.handleKey('4');
  assertEqual(12.34, testDecimal.getValue());
}

unittest(backSpace) {
  TestNumCollectorState testNoDecimal(TankControllerLib::instance());
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('B');
  assertEqual(1, testNoDecimal.getValue());

  TestNumCollectorState testDecimal(TankControllerLib::instance());
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
  TestNumCollectorState testDecimal(TankControllerLib::instance());
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('C');
  assertEqual(0, testDecimal.getValue());
}

unittest(AllDone) {
  TestNumCollectorState testDecimal(TankControllerLib::instance());
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
  TestNumCollectorState test(TankControllerLib::instance());
  lines = testLcd->getLines();
  assertEqual("0               ", lines.at(1));

  test.handleKey('2');
  lines = testLcd->getLines();
  assertEqual("2               ", lines.at(1));

  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual("21              ", lines.at(1));

  test.handleKey('*');
  lines = testLcd->getLines();
  assertEqual("21.             ", lines.at(1));

  test.handleKey('3');
  lines = testLcd->getLines();
  assertEqual("21.3            ", lines.at(1));

  test.handleKey('4');
  lines = testLcd->getLines();
  assertEqual("21.34           ", lines.at(1));

  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual("21.341          ", lines.at(1));
}

unittest_main()

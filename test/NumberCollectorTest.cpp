#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "LiquidCrystal_TC.h"
#include "NumberCollector.h"
#include "TankController.h"

unittest(Edge_Case) {
  TestNumberCollector test;
  test.handleKey('9');
  test.handleKey('0');
  test.handleKey('*');
  test.handleKey('2');
  test.handleKey('5');
  assertEqual(90.25, test.getValue());
}

unittest(Clear) {
  TestNumberCollector test;
  test.handleKey('9');
  test.handleKey('0');
  test.handleKey('*');
  test.handleKey('2');
  test.handleKey('C');
  assertEqual(0, test.getValue());
  test.handleKey('9');
  test.handleKey('0');
  test.handleKey('*');
  test.handleKey('2');
  test.handleKey('B');
  test.handleKey('5');
  assertEqual(90.5, test.getValue());
}

unittest(A_Digit_NoDecimal) {
  TestNumberCollector testNoDecimal;
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('3');
  assertEqual(123, testNoDecimal.getValue());
}

unittest(A_Digit_WithDecimal) {
  TestNumberCollector testDecimal;
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('7');
  testDecimal.handleKey('5');
  assertEqual(12.75, testDecimal.getValue());
}

unittest(A_Digit_MultipleDecimals) {
  TestNumberCollector testDecimal;
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
  TestNumberCollector testNoDecimal;
  testNoDecimal.handleKey('1');
  testNoDecimal.handleKey('2');
  testNoDecimal.handleKey('B');
  assertEqual(1, testNoDecimal.getValue());

  TestNumberCollector testDecimal;
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
  TestNumberCollector testDecimal;
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('C');
  assertEqual(0, testDecimal.getValue());
}

unittest(accept) {
  TestNumberCollector testDecimal;
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
  TestNumberCollector test;
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

unittest(printingNoCurrent) {
  LiquidCrystal_TC* testLcd = LiquidCrystal_TC::instance();
  std::vector<String> lines;
  TestNumberCollectorWithNoCurrentValue test;
  test.start();
  lines = testLcd->getLines();
  assertEqual("              0 ", lines.at(1));

  test.setPriorValue(34.125);
  test.setPriorValuePrecision(1);
  test.handleKey('2');
  lines = testLcd->getLines();
  assertEqual("              2 ", lines.at(1));

  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual("             21 ", lines.at(1));

  test.handleKey('*');
  lines = testLcd->getLines();
  assertEqual("             21.", lines.at(1));

  test.handleKey('3');
  lines = testLcd->getLines();
  assertEqual("            21.3", lines.at(1));

  test.setPriorValuePrecision(2);
  test.handleKey('4');
  lines = testLcd->getLines();
  assertEqual("           21.34", lines.at(1));

  test.setPriorValuePrecision(3);
  test.handleKey('1');
  lines = testLcd->getLines();
  assertEqual("          21.341", lines.at(1));
}

unittest(integer) {
  TestIntNumberCollector testDecimal;
  testDecimal.handleKey('1');
  testDecimal.handleKey('2');
  testDecimal.handleKey('*');
  testDecimal.handleKey('3');
  testDecimal.handleKey('4');
  assertEqual(1234, testDecimal.getValue());
}

unittest_main()

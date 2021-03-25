#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "SD_TC.h"

unittest_setup() {
  SD.removeAll();
}

unittest_teardown() {
  SD.removeAll();
}

unittest(singleton) {
  SDClass_TC* thing1 = SDClass_TC::instance();
  SDClass_TC* thing2 = SDClass_TC::instance();
  assertTrue(thing1 != nullptr);
  assertEqual(thing1, thing2);
}

unittest(printRootDirectory) {
  GodmodeState* state = GODMODE();

  SD.open("c", FILE_WRITE).close();
  SD.open("e", FILE_WRITE).close();
  SD.mkdir("d");
  SD.open("d/d1", FILE_WRITE).close();
  SD.open("d/d2", FILE_WRITE).close();

  SDClass_TC::instance()->printRootDirectory();
  String expect = String("c>>0\r\nd/\r\n>d1>>0\r\n>d2>>0\r\ne>>0\r\n");
  String output = String(state->serialPort[0].dataOut);
  std::replace(output.begin(), output.end(), '\t', '>');
  assertEqual(expect, output);
}

unittest_main()

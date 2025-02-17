#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <Serial_TC.h>

#include "TC_util.h"

GodmodeState* state = GODMODE();
Serial_TC* serialPort = Serial_TC::instance();

unittest(stringCopy) {
  String serialOutput;
  serial("This line of code initializes SD_TC so it won't pollute future logs.");

  static const char source[11] = "stringtest";
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringte\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringtes\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringte\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(0, strscpy(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
  assertEqual("", serialPort->getBuffer());
}

unittest(stringCopy_P) {
  String serialOutput;
  serial("This line of code initializes SD_TC so it won't pollute future logs.");

  const __FlashStringHelper* source = F("stringtest");
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy_P(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringte\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy_P(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringtes\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(1, strscpy_P(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringte\"", serialPort->getBuffer());

  Serial_TC::instance()->clearBuffer();  // the history of data written
  assertEqual(0, strscpy_P(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
  assertEqual("", serialPort->getBuffer());
}

unittest(floatToString) {
  String serialOutput;
  serial("This line of code initializes SD_TC so it won't pollute future logs.");

  // Float fills the buffer
  double num = 1000.5;
  char buffer[7];
  int error_code;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("1000.5", buffer);

  // Padding on the left
  num = 10.4;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  // Padding on the left, truncation on the right
  num = 10.444;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  // Padding and truncation with negative number
  num = -1.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual(" -1.44", buffer);

  // Truncation of negative number
  num = -10.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(0, error_code);
  assertEqual("-10.44", buffer);

  // Error because number is too large
  Serial_TC::instance()->clearBuffer();  // the history of data written
  num = -1000.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("-1000.", buffer);
  assertEqual("WARNING! String \"-1000.44\" was truncated to \"-1000.\"", serialPort->getBuffer());

  // Error because too many decimal places are added
  Serial_TC::instance()->clearBuffer();  // the history of data written
  num = 1.3;
  error_code = floattostrf(num, 6, 5, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("1.3000", buffer);
  assertEqual("WARNING! String \"1.30000\" was truncated to \"1.3000\"", serialPort->getBuffer());

  // Dangerously large number; potential overflow
  Serial_TC::instance()->clearBuffer();  // the history of data written
  num = 10000000000000.0;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(2, error_code);
  assertEqual("100000", buffer);
  assertEqual("WARNING! String \"10000000000000.0\" was truncated to \"100000\"", serialPort->getBuffer());
}
unittest(strtofloat) {
  assertEqual(0.0, strtofloat(" "));
  assertEqual(0.0, strtofloat("0 "));
  assertEqual(0.0, strtofloat(".0 "));
  assertEqual(0.0, strtofloat("0.0 "));
  assertEqual(0.0, strtofloat("00.00 "));
  assertEqual(5.0, strtofloat("5 "));
  assertEqual(0.5, strtofloat(".5 "));
  assertEqual(5.5, strtofloat("5.5 "));
  assertEqual(12.25, strtofloat("12.25 "));
  assertEqual(3.125, strtofloat("3.125 "));
}

unittest_main()

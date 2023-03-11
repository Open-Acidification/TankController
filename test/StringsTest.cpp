#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <Serial_TC.h>

#include "TC_util.h"

GodmodeState* state = GODMODE();

unittest(stringCopy) {
  String serialOutput;
  serial("This line of code initializes SD_TC so it won't pollute future logs.");

  static const char source[11] = "stringtest";
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringte\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringtes\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"stringtest\" was truncated to \"stringte\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(0, strscpy(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("", serialOutput.c_str());
}

unittest(stringCopy_P) {
  String serialOutput;
  serial("This line of code initializes SD_TC so it won't pollute future logs.");

  const __FlashStringHelper* source = F("stringtest");
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy_P(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual('\0', dest1[8]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringte\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy_P(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual('\0', dest2[9]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringtes\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(1, strscpy_P(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('\0', dest3[8]);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String (P) \"stringtest\" was truncated to \"stringte\"\r\n", serialOutput.c_str());

  state->serialPort[0].dataOut = "";  // the history of data written
  assertEqual(0, strscpy_P(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("", serialOutput.c_str());
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
  state->serialPort[0].dataOut = "";  // the history of data written
  num = -1000.444;
  error_code = floattostrf(num, 6, 2, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("-1000.", buffer);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"-1000.44\" was truncated to \"-1000.\"\r\n", serialOutput.c_str());

  // Error because too many decimal places are added
  state->serialPort[0].dataOut = "";  // the history of data written
  num = 1.3;
  error_code = floattostrf(num, 6, 5, buffer, sizeof(buffer));
  assertEqual(1, error_code);
  assertEqual("1.3000", buffer);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual("WARNING! String \"1.30000\" was truncated to \"1.3000\"\r\n", serialOutput.c_str());

  // Dangerously large number; potential overflow
  state->serialPort[0].dataOut = "";  // the history of data written
  num = 10000000000000.0;
  error_code = floattostrf(num, 6, 1, buffer, sizeof(buffer));
  assertEqual(2, error_code);
  assertEqual("100000", buffer);
  serialOutput = state->serialPort[0].dataOut;
  assertEqual(
      "WARNING! Memory overflow may have occurred\r\nWARNING! String \"10000000000000.0\" was truncated to "
      "\"100000\"\r\n",
      serialOutput.c_str());
}

unittest_main()

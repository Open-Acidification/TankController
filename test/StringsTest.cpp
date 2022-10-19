#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "TC_util.h"

unittest(stringCopy) {
  static const char source[11] = "stringtest";
  char dest1[9];
  char dest2[] = "012345678";
  char dest3[] = "0123456789abc";
  char dest4[] = "0123456789abc";

  assertEqual(1, strscpy(dest1, source, sizeof(dest1)));
  assertEqual("stringte", dest1);
  assertEqual(1, strscpy(dest2, source, sizeof(dest2)));
  assertEqual("stringtes", dest2);
  assertEqual(1, strscpy(dest3, source, 9));
  assertEqual("stringte", dest3);
  assertEqual('9', dest3[9]);
  assertEqual('a', dest3[10]);
  assertEqual('\0', dest3[13]);
  assertEqual(0, strscpy(dest4, source, sizeof(dest4)));
  assertEqual("stringtest", dest4);
  assertEqual('\0', dest4[10]);
  assertEqual('b', dest4[11]);
  assertEqual('\0', dest4[13]);
}

unittest(floatToString) {
  double num = 1000.5;
  char buffer[7];
  int error_code;
  error_code = floattostrf(num, 6, 1, buffer);
  assertEqual(0, error_code);
  assertEqual("1000.5", buffer);

  num = 10.4;
  error_code = floattostrf(num, 6, 1, buffer);
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  num = 10.444;
  error_code = floattostrf(num, 6, 1, buffer);
  assertEqual(0, error_code);
  assertEqual("  10.4", buffer);

  num = -1.444;
  error_code = floattostrf(num, 6, 2, buffer);
  assertEqual(0, error_code);
  assertEqual(" -1.44", buffer);

  num = -10.444;
  error_code = floattostrf(num, 6, 2, buffer);
  assertEqual(0, error_code);
  assertEqual("-10.44", buffer);

  num = -1000.444;
  error_code = floattostrf(num, 6, 2, buffer);
  assertEqual(1, error_code);
  assertEqual("-1000.", buffer);

  num = 1.3;
  error_code = floattostrf(num, 6, 5, buffer);
  assertEqual(1, error_code);
  assertEqual("1.3000", buffer);

  num = 10000000000000000;
  error_code = floattostrf(num, 6, 0, buffer);
  assertEqual(2, error_code);
  assertEqual("100000", buffer);
}

unittest_main()

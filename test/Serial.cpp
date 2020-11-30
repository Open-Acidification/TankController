#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"

unittest(SerialTest) {
  GodmodeState* state = GODMODE();
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  Serial_TC* mySerial = Serial_TC::instance();

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print('a');
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("a", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print((uint16_t)1234);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("1234", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->println();
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print_two_digits(9);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("09", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print_two_digits(10);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("10", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->write('b');
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("b", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  String test = "test:";
  mySerial->print(test);
  assertEqual("test:\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print(test, 2);
  assertEqual("test:2\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print("Label: ", "value");
  assertEqual("Label: value\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print("Test Hex:", 78, HEX);
  assertEqual("Test Hex:4E\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  byte mac[6] = {0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00};
  mySerial->print_mac(mac);
  assertEqual("MAC Address: 144:162:218:0:0:0\r\n", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  char temp[] = {'a', 'b', 'c', '\0'};
  mySerial->write(temp, strlen(temp));
  assertEqual("abc", state->serialPort[0].dataOut);

  state->serialPort[0].dataOut = "";  // the history of data written
  mySerial->print_PID(1.1, 2.2, 3.3, 1234.5);
  assertEqual("Kp:1.1000000000 Ki:2.2000000000 Kd:3.3000000000\r\nPID output (s): 1.2\r\n",
              state->serialPort[0].dataOut);
}

unittest_main()

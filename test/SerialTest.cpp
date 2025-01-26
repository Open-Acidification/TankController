#include <RTClib.h>
#include <Serial_TC.h>

#include "ArduinoUnitTests.h"
#include "DateTime_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"
#include "TankController.h"

GodmodeState* state = GODMODE();
TankController* tc = TankController::instance();
Serial_TC* serialPort = Serial_TC::instance();

unittest_setup() {
  state->resetClock();
  DateTime_TC d1(2023, 8, 15);
  d1.setAsCurrent();
  serialPort->clearBuffer();
}

unittest(SerialTest) {
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  serial(F("foo"));                   // "Unable to create directory"
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // the history of data written

  // basic serial output
  serial(F("abc %s %i %.4f"), "XYZ", 42, 1.375);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("abc XYZ 42 1.3750\r\n", state->serialPort[0].dataOut);
}

unittest(report_loop_delay) {
  DateTime_TC::now();                 // this puts stuff on the serial port that we want to ignore
  serial(F("foo"));                   // "Unable to create directory"
  state->serialPort[0].dataIn = "";   // the queue of data waiting to be read
  state->serialPort[0].dataOut = "";  // clear serial output

  tc->loop();  // for data log
  tc->loop();  // for serial log
  tc->loop();  // for remote log
  assertEqual("heater turned on at 0 after 0 ms\r\n", state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";  // clear serial output
  assertEqual(0, millis() / 100);
  for (int i = 0; i < 595; i++) {
    delay(100);
    tc->loop();
  }
  assertEqual(599, millis() / 100);
  assertEqual("", state->serialPort[0].dataOut);
  delay(100);
  assertEqual(600, millis() / 100);
  tc->loop();  // for data log
  tc->loop();  // for serial log
  assertEqual("00:01 pH=0.000 temp= 0.00\r\n", state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";  // clear serial output
  tc->loop();                         // for remote log

  tc->loop(true);  // to get the first loop delay message
  assertEqual("TankController::loop() - took 0 ms (at 60 sec uptime)\r\n", state->serialPort[0].dataOut);
  state->serialPort[0].dataOut = "";  // clear serial output

  delay(505);
  assertEqual(605, millis() / 100);
  tc->loop(true);
  assertEqual("", state->serialPort[0].dataIn);
  assertEqual("unexpected overall delay of 505 ms (at 60 sec uptime)\r\n", state->serialPort[0].dataOut);
}

unittest_main()

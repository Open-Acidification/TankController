#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include <iostream>

#include "TankControllerLib.h"

GodmodeState* state = GODMODE();
const int LOG_SIZE = 10;
struct {
  long time;
  bool value;
} pinLog[LOG_SIZE];
int logIndex = 0;
bool overflowFlag = false;
const int LED_PIN = 13;

class BitCollector : public DataStreamObserver {
public:
  BitCollector() : DataStreamObserver(false, false) {
  }
  virtual void onBit(bool aBit) {
    if (logIndex < LOG_SIZE) {
      pinLog[logIndex].time = micros();
      pinLog[logIndex].value = state->digitalPin[LED_PIN];
      // std::cout << logIndex << ":" << pinLog[logIndex].time << ":" << pinLog[logIndex].value << endl;
      logIndex++;
    } else {
      overflowFlag = true;
    }
  }

  virtual String observerName() const {
    return "BitCollector";
  }
};

unittest(loop) {
  BitCollector led;
  logIndex = 0;
  TankControllerLib* tank = TankControllerLib::instance();
  state->digitalPin[LED_PIN].addObserver("led", &led);
  tank->setup();
  state->resetClock();
  assertEqual(0, micros());
  tank->loop();
  assertTrue(0 <= micros() && micros() <= 10000);
  delay(1000);
  assertTrue(1000000 <= micros() && micros() <= 1100000);
  tank->loop();
  delay(1000);
  tank->loop();
  delay(1000);
  tank->loop();
  delay(1000);
  state->digitalPin[LED_PIN].removeObserver("led");
  assertFalse(overflowFlag);
  assertEqual(4, logIndex);
  assertEqual(0, pinLog[0].time);
  assertEqual(HIGH, pinLog[0].value);
  assertTrue(1000000 <= pinLog[1].time && pinLog[1].time <= 1100000);
  assertEqual(LOW, pinLog[1].value);
  assertTrue(2000000 <= pinLog[2].time && pinLog[2].time <= 2100000);
  assertEqual(HIGH, pinLog[2].value);
  assertTrue(3000000 <= pinLog[3].time && pinLog[3].time <= 3100000);
  assertEqual(LOW, pinLog[3].value);
}

unittest_main()

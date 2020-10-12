#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ci/ObservableDataStream.h>

#include <iostream>

#include "TankControllerLib.h"

#define LED_PIN 13

GodmodeState* state = GODMODE();
const int logSize = 10;
struct {
  long time;
  bool value;
} pinLog[logSize];
int logIndex = 0;
bool overflowFlag = false;

class BitCollector : public DataStreamObserver {
public:
  BitCollector() : DataStreamObserver(false, false) {
  }
  virtual void onBit(bool aBit) {
    if (logIndex < logSize) {
      pinLog[logIndex].time = micros();
      pinLog[logIndex].value = state->digitalPin[LED_BUILTIN];
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
  state->reset();
  BitCollector led;
  logIndex = 0;
  TankControllerLib tank;
  state->digitalPin[LED_BUILTIN].addObserver("led", &led);
  tank.setup();
  tank.loop();
  delay(500);
  tank.loop();
  delay(500);
  tank.loop();
  delay(500);
  tank.loop();
  delay(500);
  state->digitalPin[LED_BUILTIN].removeObserver("led");
  assertFalse(overflowFlag);
  assertEqual(4, logIndex);
  assertEqual(0, pinLog[0].time);
  assertEqual(HIGH, pinLog[0].value);
  assertEqual(500000, pinLog[1].time);
  assertEqual(LOW, pinLog[1].value);
  assertEqual(1000000, pinLog[2].time);
  assertEqual(HIGH, pinLog[2].value);
  assertEqual(1500000, pinLog[3].time);
  assertEqual(LOW, pinLog[3].value);
}

unittest_main()

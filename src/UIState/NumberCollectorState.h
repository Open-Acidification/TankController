/**
 * Abstract superclass for the NumberCollector UI states
 */

#pragma once
#include "../Devices/LiquidCrystal_TC.h"
#include "UIState.h"

class NumCollectorState : public UIState {
public:
  NumCollectorState(TankControllerLib* tc) : UIState(tc) {
    printValue();
  }
  void handleKey(char key);
  virtual void setValue(double value) = 0;

protected:
  // Helper Functions
  void backSpace();
  void clear();
  void handleDigit(int digit);
  void printValue();
  virtual bool isInteger() {
    return false;
  }

  double value = 0.0;
  int numDigits = 0;
  int factor = 10;
  bool hasDecimal = false;
};

#ifdef MOCK_PINS_COUNT
class TestNumCollectorState : public NumCollectorState {
public:
  TestNumCollectorState(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  // Implementation
  void setValue(double value) {
    storedValue = value;
  }
  String name() {
    return "TestNumCollectorState";
  }
  const char* prompt() {
    return "Test:";
  }

  // Testing
  double getValue() {
    return value;
  }
  double getStoredValue() {
    return storedValue;
  };

private:
  double storedValue = 0.0;
};

class TestIntNumCollectorState : public TestNumCollectorState {
public:
  TestIntNumCollectorState(TankControllerLib* tc) : TestNumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
};

#endif

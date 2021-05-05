/**
 * Abstract superclass for the NumberCollector UI states
 */

#pragma once
#include "UIState.h"

class NumCollectorState : public UIState {
public:
  NumCollectorState(TankControllerLib* tc) : UIState(tc) {
  }
  void handleKey(char key);
  virtual void setValue(double value) = 0;
  virtual void start();

protected:
  // Helper Functions
  void backSpace();
  void clear();
  void handleDigit(int digit);
  virtual bool isInteger() {
    return false;
  }
  void printValue();
  virtual float getCurrentValue() = 0;
  virtual int getCurrentValuePrecision() {
    return 0;
  };

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
  const char* name() {
    return "TestNumCollectorState";
  }
  float getCurrentValue() {
    return priorValue;
  }
  int getCurrentValuePrecision() {
    return priorValuePrecision;
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
  void setPriorValue(double aValue) {
    priorValue = aValue;
  }
  void setPriorValuePrecision(int aValue) {
    priorValuePrecision = aValue;
  }

private:
  double storedValue = 0.0;
  double priorValue = 0.0;
  int priorValuePrecision = 0;
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

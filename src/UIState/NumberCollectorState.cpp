#include "NumberCollectorState.h"

#include <math.h>

#include "../Devices/LiquidCrystal_TC.h"
#include "MainMenu.h"

void NumCollectorState::handleKey(char key) {
  if (key > '0' && key < '9') {  // a digit
    numDigits++;
    handleDigit(key - '0');
    if (collectedEnoughDigits()) {
      setValue(value);
    }
  } else if (key == 'B') {  // Backspace
    backSpace();
  } else if (key == 'C') {  // Clear the value
    numDigits = 0;
    value = 0;
  } else if (key == 'A') {  // All done
    setValue(value);
  } else if (key == '*') {  // Decimal place (if we already have a decimal nothing happens)
    hasDecimal = true;
  } else if (key == 'D') {  // Don't finish
    changeState((UIState*)new MainMenu);
  }
  printValue();
}

void NumCollectorState::handleDigit(int digit) {
  if (hasDecimal) {
    value = value + (double)digit / factor;
    factor = factor * 10;
  } else {
    value = value * 10 + digit;
  }
}

void NumCollectorState::backSpace() {
  if (hasDecimal && factor == 10) {
    hasDecimal = false;
  } else if (hasDecimal) {
    numDigits--;
    factor = factor / 10;
    // we use factor/10 because factor is set to the next decimal not the current entered one
    value = floor(value * factor / 10) / (factor / 10);
  } else {
    numDigits--;
    value = floor(value / 10);
  }
}

void NumCollectorState::printValue() {
  char strValue[16];
  if (!hasDecimal) {
    sprintf(strValue, "%.*f", 0, value);
  } else if (factor == 10) {
    sprintf(strValue, "%.*f.", 0, value);
  } else {
    int precision = log10(factor / 10);
    sprintf(strValue, "%.*f", precision, value);
  }
  LiquidCrystal_TC::instance()->writeLine(strValue, 1);
}

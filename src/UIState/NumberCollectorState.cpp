#include "NumberCollectorState.h"

#include <math.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/Serial_TC.h"
#include "MainMenu.h"
// useful for future debugging
#include "TC_util.h"

void NumCollectorState::clear() {
  numDigits = 0;
  value = 0;
}

void NumCollectorState::handleKey(char key) {
  if (key >= '0' && key <= '9') {  // a digit
    numDigits++;
    handleDigit(key - '0');
  } else if (key == '*') {  // Decimal place (if we already have a decimal nothing happens)
    hasDecimal = isInteger() ? false : true;
  } else if (key == 'A') {  // Accept
    setValue(value);
  } else if (key == 'B') {  // Backspace
    backSpace();
  } else if (key == 'C') {  // Clear
    clear();
  } else if (key == 'D') {  // Disregard (escape/cancel)
    returnToMainMenu();
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
  char strValue[20];
  double current = getCurrentValue();
  int precision = getCurrentValuePrecision();
  int intWidth = 6 - precision;
  Serial.print("<A");
  Serial.print(current);
  Serial.print(" ");
  Serial.print(precision);
  Serial.print(" ");
  Serial.print(intWidth);
  snprintf(strValue, sizeof(strValue), "%*i->", 3, 12);
  // snprintf(strValue, 10, "%*i.%*i->", intWidth, (int)current, FLOAT((current - (int)current) + precision,
  // precision));
  Serial.print(" \"");
  Serial.print(strValue);
  Serial.print("\" ");

  int size = strlen(strValue);
  Serial.print(size);
  char* ptr = &strValue[size];
  size = sizeof(strValue) - size;
  Serial.print(" ");
  Serial.print(size);
  Serial.println("</A>");
  if (!hasDecimal) {
    // show user entry as an integer (no decimal yet)
    snprintf(ptr, size, "%i ", (int)value);
  } else if (factor == 10) {
    // show user entry with a decimal but nothing beyond
    snprintf(ptr, size, "%i.", (int)value);
  } else {
    // show user entry with appropriate precision (based on digits user has entered)
    precision = log10(factor / 10);
    snprintf(ptr, size, "%*i.%*i", intWidth, FLOAT(value, precision));
  }
  Serial.print("<B>");
  Serial.print(strValue);
  Serial.println("</B>");
  LiquidCrystal_TC::instance()->writeLine(strValue, 1);
}

void NumCollectorState::start() {
  UIState::start();
  printValue();
}

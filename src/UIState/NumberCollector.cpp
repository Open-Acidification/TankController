#include "NumberCollector.h"

#include <math.h>

#include "MainMenu.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/Serial_TC.h"
// useful for future debugging
#include "model/TC_util.h"

void NumberCollector::clear() {
  hasDecimal = false;
  numDigits = 0;
  value = 0;
  factor = 10;
}

void NumberCollector::handleKey(char key) {
  if (key >= '0' && key <= '9') {  // a digit
    numDigits++;
    handleDigit(key - '0');
  } else if (key == '*') {  // Decimal place (if we already have a decimal nothing happens)
    hasDecimal = !isInteger();
  } else if (key == 'A') {  // Accept
    setValue(value);
    return;                 // don't print the value again
  } else if (key == 'B') {  // Backspace
    backSpace();
  } else if (key == 'C') {  // Clear
    clear();
  } else if (key == 'D') {  // Disregard (escape/cancel)
    returnToMainMenu();
  }
  printValue();
}

void NumberCollector::handleDigit(uint16_t digit) {
  if (hasDecimal) {
    value = value + (float)digit / factor;
    factor = factor * 10;
  } else {
    value = value * 10 + digit;
  }
}

void NumberCollector::backSpace() {
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

void NumberCollector::printValue() {
  char format[20], strValue[20];
  // The Arduino does not support variable widths, so we construct the format string at runtime!
  char buffer[8];
  if (this->showCurrentValue()) {
    floattostrf(getCurrentValue(), 7, getCurrentValuePrecision(), buffer, sizeof(buffer));
    memcpy(strValue, buffer, sizeof(buffer));
    snprintf_P(strValue + 7, 3, (PGM_P)F("->"));
  } else {
    strscpy_P(strValue, F("         "), sizeof(strValue));
  }

  if (!hasDecimal) {
    // show user entry as an integer (no decimal yet)
    snprintf_P(format, sizeof(format), (PGM_P)F("%s%%6i"), strValue);
    snprintf(strValue, sizeof(strValue), format, (uint32_t)value);
  } else if (factor == 10) {
    // show user entry with a decimal but nothing beyond
    snprintf_P(format, sizeof(format), (PGM_P)F("%s%%6i."), strValue);
    snprintf(strValue, sizeof(strValue), format, (uint32_t)value);
  } else {
    // show user entry with appropriate precision (based on digits user has entered)
    uint16_t precision = log10(factor / 10);
    floattostrf(value, 7, precision, buffer, sizeof(buffer));
    memcpy(strValue + 9, buffer, sizeof(buffer));
    // TODO: Is there a test of this case?
  }
  LiquidCrystal_TC::instance()->writeLine(strValue, 1);
}

void NumberCollector::start() {
  UIState::start();
  printValue();
}

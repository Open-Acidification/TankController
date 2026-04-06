/**
 * ResetEEPROM.h
 *
 * Clear EEPROM
 */
#pragma once
#include "UIState.h"

class ResetEEPROM : public UIState {
public:
  void handleKey(char key);
  const __FlashStringHelper* name() {
    return F("ResetEEPROM");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Erase EEPROM");
  };
};

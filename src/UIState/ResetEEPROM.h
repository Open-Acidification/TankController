/**
 * ResetEEPROM.h
 *
 * Asks whether to reset EEPROM to factory default, which is useful if the data is corrupted and causing crashes. This
 * is accessed by holding the 'C' key on the keypad during startup. The device is rebooted after the selection is made.
 */
#pragma once
#include "UIState.h"

class ResetEEPROM : public UIState {
public:
  void handleKey(char key);
  bool isInCalibration() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("Erase EEPROM?");
  }
  const __FlashStringHelper* prompt() {
    return F("A: Erase  D: Cancel");
  };
};

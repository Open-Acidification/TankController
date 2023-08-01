/**
 * SeeDeviceAddress.h
 *
 * Display the device address
 */
#pragma once
#include "UIState.h"

class SeeDeviceAddress : public UIState {
public:
  SeeDeviceAddress() : UIState() {
  }
  void start() override;
  void handleKey(char key) override;
  const __FlashStringHelper* name() override {
    return F("SeeDeviceAddress");
  }
  const __FlashStringHelper* prompt() override {
    return F("Device address");
  };
};

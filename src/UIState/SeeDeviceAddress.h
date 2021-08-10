/**
 * SeeDeviceAddress.h
 *
 * Display the device address
 */
#pragma once
#include "UIState.h"

class SeeDeviceAddress : public UIState {
public:
  SeeDeviceAddress(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  void handleKey(char key);
  const char* name() {
    return "SeeDeviceAddress";
  }
  const char* prompt() {
    return "Device address";
  };
};

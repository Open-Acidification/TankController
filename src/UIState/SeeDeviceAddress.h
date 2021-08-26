/**
 * SeeDeviceAddress.h
 *
 * Display the device address
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeDeviceAddress : public UIState {
public:
  SeeDeviceAddress(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  void handleKey(char key);
  const char* name() {
    return CSTR("SeeDeviceAddress");
  }
  const char* prompt() {
    return CSTR("Device address");
  };
};

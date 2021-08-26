/**
 * SeeGoogleMins.h
 *
 * Display the Google Sheet Interval
 */
#pragma once
#include "TC_util.h"
#include "UIState.h"

class SeeGoogleMins : public UIState {
public:
  SeeGoogleMins(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const char* name() {
    return CSTR("SeeGoogleMins");
  }
  const char* prompt() {
    return CSTR("Google Mins:");
  };
};

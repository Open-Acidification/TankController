/**
 * SeeGoogleMins.h
 *
 * Display the Google Sheet Interval
 */
#pragma once
#include "UIState.h"

class SeeGoogleMins : public UIState {
public:
  SeeGoogleMins(TankControllerLib* tc) : UIState(tc) {
  }
  void start();
  const __FlashStringHelper* name() {
    return F("SeeGoogleMins");
  }
  const __FlashStringHelper* prompt() {
    return F("Google Mins:");
  };
};

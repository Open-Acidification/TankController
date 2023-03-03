/**
 * SeeGoogleMins.h
 *
 * Display the Google Sheet Interval
 */
#pragma once
#include "UIState.h"

class SeeGoogleMins : public UIState {
public:
  SeeGoogleMins(TankController* tc) : UIState(tc) {
  }
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeeGoogleMins");
  }
  const __FlashStringHelper* prompt() override {
    return F("Google Mins:");
  };
};

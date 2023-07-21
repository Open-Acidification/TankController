/**
 * SeePHCalibration.cpp
 */

#include "SeePHCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "MainMenu.h"

SeePHCalibration::SeePHCalibration(TankController* tc, bool inCalibration) : UIState(tc) {
  endTime = millis() + 60000;
  this->inCalibration = inCalibration;
}

void SeePHCalibration::loop() {
  char buffer[20];
  PHProbe::instance()->getCalibration(buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  if (endTime <= millis()) {
    this->setNextState(new MainMenu(tc));
  }
}

void SeePHCalibration::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(F("requesting calib"), 1);
  PHProbe::instance()->sendCalibrationRequest();
}

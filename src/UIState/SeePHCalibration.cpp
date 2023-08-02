/**
 * SeePHCalibration.cpp
 */

#include "SeePHCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "TC_util.h"
#include "UIState/BadPHCalibration.h"
#include "UIState/MainMenu.h"

SeePHCalibration::SeePHCalibration(bool inCalibration) : UIState() {
  endTime = millis() + 60000;
  this->inCalibration = inCalibration;
}

void SeePHCalibration::loop() {
  char pointsBuffer[20];
  char slopeBuffer[20];
  PHProbe::instance()->getCalibration(pointsBuffer, sizeof(pointsBuffer));
  PHProbe::instance()->getSlope(slopeBuffer, sizeof(slopeBuffer));
  LiquidCrystal_TC::instance()->writeLine(pointsBuffer, 0);
  LiquidCrystal_TC::instance()->writeLine(slopeBuffer, 1);
  if (PHProbe::instance()->slopeIsBad()) {
    this->setNextState(new BadPHCalibration());
  } else if (endTime <= millis()) {
    this->setNextState(new MainMenu());
  }
}

void SeePHCalibration::start() {
  PHProbe::instance()->sendCalibrationRequest();
  PHProbe::instance()->sendSlopeRequest();
}

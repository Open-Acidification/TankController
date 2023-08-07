/**
 * SeePHCalibration.cpp
 */

#include "SeePHCalibration.h"

#include "UIState/MainMenu.h"
#include "UIState/PHCalibrationWarning.h"
#include "model/PHProbe.h"
#include "model/TC_util.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/Serial_TC.h"

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
    this->setNextState(new PHCalibrationWarning());
  } else if (endTime <= millis()) {
    this->setNextState(new MainMenu());
  }
}

void SeePHCalibration::start() {
  PHProbe::instance()->sendCalibrationRequest();
  PHProbe::instance()->sendSlopeRequest();
}

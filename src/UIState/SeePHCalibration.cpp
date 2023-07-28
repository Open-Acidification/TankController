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

SeePHCalibration::SeePHCalibration(TankController* tc, bool inCalibration) : UIState(tc) {
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
  if (endTime <= millis()) {
    this->setNextState(new MainMenu(tc));
  }
}

void SeePHCalibration::start() {
  PHProbe::instance()->sendCalibrationRequest();
  PHProbe::instance()->sendSlopeRequest();
}

void SeePHCalibration::checkPhSlope() {
  char buffer[20];
  PHProbe::instance()->getSlope(buffer, sizeof(buffer));
  char acidSlopePercent[20];
  char baseSlopePercent[20];
  char millivoltOffset[20];
  if (memcmp_P(buffer, F("Requesting"), 10) == 0) {
    serial(F("SeePHCalibration::checkPhSlope() failed to parse slope from PHProbe::instance()"));
  } else {
    sscanf_P(buffer, PSTR(" %[^,] , %[^,] , %s"), acidSlopePercent, baseSlopePercent, millivoltOffset);
    if ((95.0 <= strtofloat(acidSlopePercent)) && (strtofloat(acidSlopePercent) <= 105.0) &&
        (95.0 <= strtofloat(baseSlopePercent)) && (strtofloat(baseSlopePercent) <= 105.0)) {
      serial(F("pH slopes are within 5%% of ideal"));
    } else {
      serial(F("BAD CALIBRATION? pH slopes are more than 5%% from ideal"));
      this->setNextState(new BadPHCalibration(tc));
    }
  }
}

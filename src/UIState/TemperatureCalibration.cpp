/**
 * TemperatureCalibration.cpp
 */

#include "TemperatureCalibration.h"

#include "Devices/TempProbe_TC.h"

void TemperatureCalibration::setValue(float value) {
  TempProbe_TC *pProbe = TempProbe_TC::instance();
  float average = pProbe->getRunningAverage();
  pProbe->setCorrection(value - average);
}

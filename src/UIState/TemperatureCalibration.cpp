/**
 * TemperatureCalibration.cpp
 */

#include "TemperatureCalibration.h"

#include "TempProbe_TC.h"

void TemperatureCalibration::setValue(double value) {
  TempProbe_TC *pProbe = TempProbe_TC::instance();
  double average = pProbe->getRunningAverage();
  pProbe->setCorrection(value - average);
}

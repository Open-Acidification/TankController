/**
 * TemperatureCalibration.cpp
 */

#include "TemperatureCalibration.h"

#include "Devices/TempProbe_TC.h"

void TemperatureCalibration::setValue(double value) {
  TempProbe_TC *pProbe = TempProbe_TC::instance();
  double average = pProbe->getRunningAverage();
  pProbe->setCorrection(value - average);
}

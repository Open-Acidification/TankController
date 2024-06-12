#include "model/JSONBuilder.h"

// Include all relevant classes
#include "TankController.h"
#include "model/PHControl.h"
#include "model/PHProbe.h"
#include "model/TC_util.h"
#include "model/ThermalControl.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/PID_TC.h"
#include "wrappers/SD_TC.h"
#include "wrappers/ThermalProbe_TC.h"

int JSONBuilder::buildCurrentValues() {
  // Grab all necessary pieces
  float pH = PHProbe::instance()->getPh();
  int pH_f = (int)(pH * 1000 + 0.5) % 1000;
  while (pH_f && pH_f % 10 == 0) {
    pH_f /= 10;
  }
  float target_pH = PHControl::instance()->getBaseTargetPh();
  int target_pH_f = (int)(target_pH * 1000 + 0.5) % 1000;
  while (target_pH_f && target_pH_f % 10 == 0) {
    target_pH_f /= 10;
  }
  float temperature = ThermalProbe_TC::instance()->getRunningAverage();
  // int target_therm_type = ThermalControl::instance()->getThermalFunctionType();  // Flat 0, Ramp 1, or Sine 2
  // https://github.com/Open-Acidification/TankController/issues/331
  int temperature_f = (temperature - (int)temperature) * 1000 + 0.5;
  while (temperature_f && temperature_f % 10 == 0) {
    temperature_f /= 10;
  }
  float thermal_target = ThermalControl::instance()->getBaseThermalTarget();
  int thermal_target_f = (thermal_target - (int)thermal_target) * 1000 + 0.5;
  while (thermal_target_f && thermal_target_f % 10 == 0) {
    thermal_target_f /= 10;
  }
  IPAddress IP = Ethernet_TC::instance()->getIP();
  byte* mac = Ethernet_TC::instance()->getMac();
  char logFilePath[30];
  SD_TC::instance()->todaysDataFileName(logFilePath, sizeof(logFilePath));
  char pHSlope[20];
  float pHSineAmplitude = 0.0;
  if ((EEPROM_TC::instance()->getPhSinePeriod() / 3600.0) > 0) {
    pHSineAmplitude = (PHControl::instance()->getAmplitude() ? PHControl::instance()->getAmplitude() : 0);
  }
  int pHSineAmplitude_f = (int)(pHSineAmplitude * 1000 + 0.5) % 1000;
  while (pHSineAmplitude_f && pHSineAmplitude_f % 10 == 0) {
    pHSineAmplitude_f /= 10;
  }
  float thermSineAmplitude = 0.0;
  if ((EEPROM_TC::instance()->getThermalSinePeriod() / 3600.0) > 0) {
    thermSineAmplitude = (ThermalControl::instance()->getAmplitude() ? ThermalControl::instance()->getAmplitude() : 0);
  }
  int thermSineAmplitude_f = (int)(thermSineAmplitude * 1000 + 0.5) % 1000;
  while (thermSineAmplitude_f && thermSineAmplitude_f % 10 == 0) {
    thermSineAmplitude_f /= 10;
  }
  PHProbe::instance()->getSlope(pHSlope, sizeof(pHSlope));
  float kp = PID_TC::instance()->getKp();
  float ki = PID_TC::instance()->getKi();
  float kd = PID_TC::instance()->getKd();
  char pidStatus[4];
  if (PHControl::instance()->getUsePID()) {
    snprintf_P(pidStatus, sizeof(pidStatus), (PGM_P)F("ON"));
  } else {
    snprintf_P(pidStatus, sizeof(pidStatus), (PGM_P)F("OFF"));
  }
  char heatStatus[6];
  if (EEPROM_TC::instance()->getHeat()) {
    snprintf_P(heatStatus, sizeof(heatStatus), (PGM_P)F("HEAT"));
  } else {
    snprintf_P(heatStatus, sizeof(heatStatus), (PGM_P)F("CHILL"));
  }
  uint32_t ms = millis();
  uint16_t days = ms / 86400000;
  uint16_t hours = (ms - (days * 86400000)) / 3600000;
  uint16_t minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  uint16_t seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;

  char pH_FunctionType[5];
  PHControl::pHFunctionTypes PHFunctionTypeEnum = PHControl::instance()->getPHFunctionType();
  switch (PHFunctionTypeEnum) {
    case PHControl::pHFunctionTypes::RAMP_TYPE:
      snprintf_P(pH_FunctionType, sizeof(pH_FunctionType), (PGM_P)F("RAMP"));
      break;
    case PHControl::pHFunctionTypes::SINE_TYPE:
      snprintf_P(pH_FunctionType, sizeof(pH_FunctionType), (PGM_P)F("SINE"));
      break;
    default:
      snprintf_P(pH_FunctionType, sizeof(pH_FunctionType), (PGM_P)F("FLAT"));
      break;
  }

  char Therm_FunctionType[5];
  ThermalControl::thermalFunctionTypes ThermFunctionTypeEnum = ThermalControl::instance()->getThermalFunctionType();
  switch (ThermFunctionTypeEnum) {
    case ThermalControl::thermalFunctionTypes::RAMP_TYPE:
      snprintf_P(Therm_FunctionType, sizeof(Therm_FunctionType), (PGM_P)F("RAMP"));
      break;
    case ThermalControl::thermalFunctionTypes::SINE_TYPE:
      snprintf_P(Therm_FunctionType, sizeof(Therm_FunctionType), (PGM_P)F("SINE"));
      break;
    default:
      snprintf_P(Therm_FunctionType, sizeof(Therm_FunctionType), (PGM_P)F("FLAT"));
      break;
  }
  /* Remote restart
  Clear/reset pH calibration
  Clear/reset temperature calibration
  Sine wavelength and amplitude*/

  float pH_SinePeriodHours = 0.0;
  int pH_SinePeriodHours_f = 0;
  // if sine amplitude is nonzero, then we are in sine mode and display the sine period
  if (pHSineAmplitude != 0) {
    pH_SinePeriodHours = EEPROM_TC::instance()->getPhSinePeriod() / 3600.0;
  }
  if (pH_SinePeriodHours > 0) {
    // stripping off integer part, leaving only the decimal part to avoid overflow
    float x = pH_SinePeriodHours - (int)pH_SinePeriodHours;
    pH_SinePeriodHours_f = (int)(x);
    while (pH_SinePeriodHours_f && pH_SinePeriodHours_f % 10 == 0) {
      pH_SinePeriodHours_f /= 10;
    }
  }

  float pH_RampHours = 0.0;
  int pH_RampHours_f = 0;
  // ramp hours could be problematic if ramp time start and end are in the past so think about that..........
  if (PHControl::instance()->getPhRampTimeEnd() > 0 && EEPROM_TC::instance()->getPhSinePeriod() == 0) {
    pH_RampHours = (PHControl::instance()->getPhRampTimeEnd() - PHControl::instance()->getPhRampTimeStart()) / 3600.0;
  }
  if (pH_RampHours > 0) {
    float x = pH_RampHours - (int)pH_RampHours;
    pH_RampHours_f = (int)(x);
    while (pH_RampHours_f && pH_RampHours_f % 10 == 0) {
      pH_RampHours_f /= 10;
    }
  }

  float therm_SinePeriodHours = 0.0;
  int therm_SinePeriodHours_f = 0;
  // if sine amplitude is nonzero, then we are in sine mode and display the sine period
  if (thermSineAmplitude != 0) {
    therm_SinePeriodHours = EEPROM_TC::instance()->getThermalSinePeriod() / 3600.0;
  }
  if (therm_SinePeriodHours > 0) {
    // stripping off integer part, leaving only the decimal part to avoid overflow
    float x = therm_SinePeriodHours - (int)therm_SinePeriodHours;
    therm_SinePeriodHours_f = (int)(x);
    while (therm_SinePeriodHours_f && therm_SinePeriodHours_f % 10 == 0) {
      therm_SinePeriodHours_f /= 10;
    }
  }

  float therm_rampHours = 0.0;
  int therm_rampHours_f = 0;
  // ramp hours could be problematic if ramp time start and end are in the past so think about that..........
  if (ThermalControl::instance()->getRampTimeEnd() > 0 && EEPROM_TC::instance()->getThermalSinePeriod() == 0) {
    therm_rampHours =
        (ThermalControl::instance()->getRampTimeEnd() - ThermalControl::instance()->getRampTimeStart()) / 3600.0;
  }
  if (therm_rampHours > 0) {
    float x = therm_rampHours - (int)therm_rampHours;
    therm_rampHours_f = (int)(x);
    while (therm_rampHours_f && therm_rampHours_f % 10 == 0) {
      therm_rampHours_f /= 10;
    }
  }

  bytes = snprintf_P(buffer, BUFFER_SIZE,
                     (PGM_P)F("{"
                              "\"pH\":%i.%i,"
                              "\"Temperature\":%i.%i,"
                              "\"pH_FunctionType\":\"%s\","
                              "\"Target_pH\":%i.%i,"
                              "\"pH_RampHours\":%i.%i,"
                              "\"pH_SinePeriodHours\":%i.%i,"
                              "\"pH_SineAmplitude\":%i.%i,"
                              "\"Therm_FunctionType\":\"%s\","
                              "\"TargetTemperature\":%i.%i,"
                              "\"Therm_RampHours\":%i.%i,"
                              "\"Therm_SinePeriodHours\":%i.%i,"
                              "\"Therm_SineAmplitude\":%i.%i,"
                              "\"IPAddress\":\"%d.%d.%d.%d\","
                              "\"MAC\":\"%02X:%02X:%02X:%02X:%02X:%02X\","
                              "\"FreeMemory\":\"%i bytes\","
                              "\"GoogleSheetInterval\":%i,"
                              "\"LogFile\":\"%s\","
                              "\"PHSlope\":\"%s\","
                              "\"Kp\":%i.%i,"
                              "\"Ki\":%i.%i,"
                              "\"Kd\":%i.%i,"
                              "\"PID\":\"%s\","
                              "\"TankID\":%i,"
                              "\"Uptime\":\"%id %ih %im %is\","
                              "\"HeatOrChill\":\"%s\","
                              "\"Version\":\"%s\","
                              "\"EditableFields\":["  // List of editable fields follows
                              "\"Target_pH\","
                              "\"TargetTemperature\","
                              "\"GoogleSheetInterval\","
                              "\"Kp\","
                              "\"Ki\","
                              "\"Kd\","
                              "\"pH_RampHours\","
                              // "\"pH_SineAmplitude\","
                              // "\"pH_SinePeriodHours\","
                              "\"Therm_RampHours\","
                              // "\"Therm_SineAmplitude\","
                              // "\"Therm_SinePeriodHours\","
                              "\"TankID\","
                              "\"HeatOrChill\","
                              "\"PID\""
                              "]"
                              "}"),
                     (int)pH, pH_f, (int)temperature, temperature_f, pH_FunctionType, (int)target_pH, target_pH_f,
                     (int)pH_RampHours, pH_RampHours_f, (int)pH_SinePeriodHours, pH_SinePeriodHours_f,
                     (int)pHSineAmplitude, pHSineAmplitude_f, Therm_FunctionType, (int)thermal_target, thermal_target_f,
                     (int)therm_rampHours, therm_rampHours_f, (int)therm_SinePeriodHours, therm_SinePeriodHours_f,
                     (int)thermSineAmplitude, thermSineAmplitude_f, IP[0], IP[1], IP[2], IP[3], mac[0], mac[1], mac[2],
                     mac[3], mac[4], mac[5], (int)TankController::instance()->freeMemory(),
                     EEPROM_TC::instance()->getGoogleSheetInterval(), logFilePath, pHSlope, (int)kp,
                     (int)((kp - (int)kp) * 10 + 0.5), (int)ki, (int)((ki - (int)ki) * 10 + 0.5), (int)kd,
                     (int)((kd - (int)kd) * 10 + 0.5), pidStatus, EEPROM_TC::instance()->getTankID(), days, hours,
                     minutes, seconds, heatStatus, TankController::instance()->version());

  // if (pH_FunctionType == "SINE") {
  //   buffer.append("\"pH_SinePeriodHours\",");
  // }

  return bytes;
}

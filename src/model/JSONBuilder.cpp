#include "model/JSONBuilder.h"

// Include all relevant classes
#include "TankController.h"
#include "model/PHControl.h"
#include "model/PHProbe.h"
#include "model/TC_util.h"
#include "model/ThermalControl.h"
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
  // https://github.com/Open-Acidification/TankController/issues/331
  int temperature_f = (temperature - (int)temperature) * 1000 + 0.5;
  while (temperature_f && temperature_f % 10 == 0) {
    temperature_f /= 10;
  }
  float thermal_target = ThermalControl::instance()->getCurrentThermalTarget();
  int thermal_target_f = (thermal_target - (int)thermal_target) * 1000 + 0.5;
  while (thermal_target_f && thermal_target_f % 10 == 0) {
    thermal_target_f /= 10;
  }
  IPAddress IP = Ethernet_TC::instance()->getIP();
  byte* mac = Ethernet_TC::instance()->getMac();
  char logFilePath[30];
  SD_TC::instance()->todaysDataFileName(logFilePath, sizeof(logFilePath));
  char pHSlope[20];
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
  uint32_t ms = millis();
  uint16_t days = ms / 86400000;
  uint16_t hours = (ms - (days * 86400000)) / 3600000;
  uint16_t minutes = (ms - (days * 86400000) - (hours * 3600000)) / 60000;
  uint16_t seconds = (ms - (days * 86400000) - (hours * 3600000) - (minutes * 60000)) / 1000;

  bytes = snprintf_P(buffer, BUFFER_SIZE,
                     (PGM_P)F("{"
                              "\"pH\":%i.%i,"
                              "\"Target_pH\":%i.%i,"
                              "\"Temperature\":%i.%i,"
                              "\"TargetTemperature\":%i.%i,"
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
                              "\"Version\":\"%s\""
                              "}"),
                     (int)pH, pH_f, (int)target_pH, target_pH_f, (int)temperature, temperature_f, (int)thermal_target,
                     thermal_target_f, IP[0], IP[1], IP[2], IP[3], mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                     (int)TankController::instance()->freeMemory(), EEPROM_TC::instance()->getGoogleSheetInterval(),
                     logFilePath, pHSlope, (int)kp, (int)((kp - (int)kp) * 10 + 0.5), (int)ki,
                     (int)((ki - (int)ki) * 10 + 0.5), (int)kd, (int)((kd - (int)kd) * 10 + 0.5), pidStatus,
                     EEPROM_TC::instance()->getTankID(), days, hours, minutes, seconds,
                     TankController::instance()->version());
  return bytes;
}

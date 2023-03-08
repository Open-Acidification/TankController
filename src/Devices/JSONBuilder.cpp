#include "JSONBuilder.h"

// Include all relevant classes
#include "Devices/EEPROM_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/SD_TC.h"
#include "TC_util.h"
#include "TankController.h"

int JSONBuilder::buildCurrentValues() {
  // Grab all necessary pieces
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
                     (PGM_P)F("{\"IPAddress\":\"%d.%d.%d.%d\","
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
                              "\"Version\":\"%s\"}"),
                     IP[0], IP[1], IP[2], IP[3], mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                     (int)TankController::instance()->freeMemory(), EEPROM_TC::instance()->getGoogleSheetInterval(),
                     logFilePath, pHSlope, (int)kp, (int)(kp * 10 + 0.5) % 10, (int)ki, (int)(ki * 10 + 0.5) % 10,
                     (int)kd, (int)(kd * 10 + 0.5) % 10, pidStatus, EEPROM_TC::instance()->getTankID(), days, hours,
                     minutes, seconds, TankController::instance()->version());
  return bytes;
}

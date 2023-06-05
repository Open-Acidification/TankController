#include "JSONBuilder.h"

// Include all relevant classes
#include "Devices/EEPROM_TC.h"
#include "Devices/Ethernet_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "Devices/PID_TC.h"
#include "Devices/SD_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TC_util.h"
#include "TankController.h"

int JSONBuilder::buildCurrentValues() {
  // Grab all necessary pieces
  float pH = PHProbe::instance()->getPh();
  float target_pH = PHControl::instance()->getTargetPh();
  float temp = TempProbe_TC::instance()->getRunningAverage();
  float target_temp = TemperatureControl::instance()->getTargetTemperature();
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
/*
expected
{"pH":8.125,"Target_pH":8.25,"Temperature":21.25,"TargetTemperature":21.75,    "IPAddress":"192.168.1.10","MAC":"90:A2:DA:FB:F6:F1","FreeMemory":"1024 bytes","GoogleSheetInterval":65535,"LogFile":"20220222.csv","PHSlope":"","Kp":100000.0,"Ki":0.0,"Kd":0.0,"PID":"ON","TankID":0,"Uptime":"0d 0h 0m 1s","Version":"23.03.1"}
{"pH":"0.0","Target_pH":"8.100","Temperature":"0.0","TargetTemperature":"20.0","IPAddress":"192.168.1.10","MAC":"90:A2:DA:FB:F6:F1","FreeMemory":"1024 bytes","GoogleSheetInterval":65535,"LogFile":"20220222.csv","PHSlope":"","Kp":100000.0,"Ki":0.0,"Kd":0.0
actual
*/
  bytes = snprintf_P(buffer, BUFFER_SIZE,
                     (PGM_P)F("{"
                              "\"pH\":\"%i.%i\","
                              "\"Target_pH\":\"%i.%i\","
                              "\"Temperature\":\"%i.%i\","
                              "\"TargetTemperature\":\"%i.%i\","
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
                     (int)pH, (int)(pH * 1000 + 0.5) %1000, 
                     (int)target_pH, (int)(target_pH * 1000 + 0.5) %1000, 
                     (int)temp, (int)(temp * 1000 + 0.5) %1000, 
                     (int)target_temp, (int)(target_temp * 1000 + 0.5) %1000, 
                     IP[0], IP[1], IP[2], IP[3], 
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                     (int)TankController::instance()->freeMemory(), 
                     EEPROM_TC::instance()->getGoogleSheetInterval(),
                     logFilePath, pHSlope, 
                     (int)kp, (int)(kp * 10 + 0.5) % 10, 
                     (int)ki, (int)(ki * 10 + 0.5) % 10,
                     (int)kd, (int)(kd * 10 + 0.5) % 10, 
                     pidStatus, EEPROM_TC::instance()->getTankID(), 
                     days, hours, minutes, seconds, 
                     TankController::instance()->version());
  return bytes;
}

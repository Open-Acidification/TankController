#pragma once
#include <Arduino.h>

/*
 * @brief DataLogger is a singleton that logs data to the SD card and serial port
 *
 * The singleton is called from the loop() method of the TankController class.
 * It manages three logs: data, serial, and event.
 * 
 * The data log is written every second and includes the following:
 * "time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd".
 * 
 * The serial log is written every minute and includes the following:
 * "HH:MM, current pH, current temperature".
 * 
 * The event log is written every minute and includes both data and other events.
 * The other events are based on either the PHProbe reporting a new slope or the 
 * EEPROM having a new value written to it. In each case, it means a configuration 
 * changed.
 *
 */

// Logging intervals (1 sec, 1 min, 1 min)
const unsigned long DATA_LOGGING_INTERVAL = 1000;     // 1 sec logging of basic data to SD card
const unsigned long EVENT_LOGGING_INTERVAL = 60000;   // 1 min logging of data and other events
const unsigned long SERIAL_LOGGING_INTERVAL = 60000;  // 1 min logging of data and debugging info

class DataLogger {
public:
  // class methods
  static DataLogger *instance();

  // instance methods
  void loop();

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  char *getBuffer() {
    return buffer;
  }
  void clearBuffer() {
    buffer[0] = '\0';
  }
#endif

private:
  // class variables
  static DataLogger *_instance;

  // instance variables
  char buffer[256];
  uint32_t nextDataLogTime = 0;
  uint32_t nextSerialLogTime = 0;

  // instance methods
  void writeToDataLog();
  void writeToSerialLog();
};

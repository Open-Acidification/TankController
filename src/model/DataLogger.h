#pragma once
#include <Arduino.h>

/*
  * @brief DataLogger is a singleton that logs data to the SD card and serial port
  *
  * The singleton is called from the loop() method of the TankController class.
  * It is also called from PHProbe if a slope event is received and from EEPROM_TC 
  * if a value is written (which means that some configuration changed).
  * 
  * The singleton logs data to the SD card's data log every second and to 
  * the serial port and info log every minute. The info log includes periodic data
  * and warnings/alerts. The SD_TC::instance()->writeAlert() handles notifying the
  * AlertPusher singleton when there is new data.
  * 
*/

// Logging intervals (1 min, 1 sec, 1 min)
const unsigned long INFO_LOGGING_INTERVAL = 60000;
const unsigned long SD_LOGGING_INTERVAL = 1000;
const unsigned long SERIAL_LOGGING_INTERVAL = 60000;

class DataLogger {
public:
  // class methods
  static DataLogger *instance();

  // instance methods
  void putAlertFileHeader(char *buffer, int size, int count);
  void loop();
  void writeWarningSoon() {
    shouldWriteWarning = true;
  }

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  char *getBuffer() {
    return buffer;
  }
  void clearBuffer() {
    buffer[0] = '\0';
  }
  bool getShouldWriteWarning() {
    return shouldWriteWarning;
  }
  void reset() {
    clearBuffer();
    shouldWriteWarning = false;
  }
#endif

private:
  // class variables
  static DataLogger *_instance;

  // instance variables
  char buffer[256];
  uint32_t nextSDLogTime = 0;
  uint32_t nextSerialLogTime = 0;
  uint32_t nextInfoLogTime = 0;
  bool shouldWriteWarning = false;

  // instance methods
  void writeAlertPreambleToBuffer(const char severity);
  void writeInfoToLog();
  void writeToSD();
  void writeToSerial();
  void writeWarningToLog();
};

#pragma once
#include <Arduino.h>

// Logging intervals (1 min, 1 sec, 1 min)
#define INFO_LOGGING_INTERVAL 60000
#define SD_LOGGING_INTERVAL 1000
#define SERIAL_LOGGING_INTERVAL 60000

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
  uint32_t nextInfoLogTime = INFO_LOGGING_INTERVAL;
  bool shouldWriteWarning = false;

  // instance methods
  void writeAlertPreambleToBuffer(const char severity);
  void writeInfoToLog();
  void writeToSD();
  void writeToSerial();
  void writeWarningToLog();
};

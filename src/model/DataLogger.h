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
  char buffer[128];
  uint32_t nextSDLogTime = 0;
  uint32_t nextSerialLogTime = 0;
  uint32_t nextInfoLogTime = 0;

  // instance methods
  void writeInfoToLog();
  void writeToSD();
  void writeToSerial();
};

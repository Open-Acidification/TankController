#pragma once
#include <Arduino.h>

// Logging intervals (1 sec, 1 min)
#define SD_LOGGING_INTERVAL 1000
#define SERIAL_LOGGING_INTERVAL 60000

class DataLogger {
public:
  // class methods
  static DataLogger *instance();

  // instance methods
  void loop();

  // instance variables
  char buffer[128];

private:
  // class variables
  static DataLogger *_instance;

  // instance variables
  uint32_t nextSDLogTime = 0;
  uint32_t nextSerialLogTime = 0;

  // instance methods
  void writeToSD();
  void writeToSerial();
};

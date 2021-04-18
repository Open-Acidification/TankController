#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <SD_CI.h>
#else
#include <SD.h>
#endif

class SD_TC : public SDClass {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendToDataLog(String header, String data);
  void appendToSerialLog(String data);
  void printRootDirectory();

private:
  // class variables
  static SD_TC* _instance;

  // instance methods
  void appendDataToPath(String data, String path);
  void printDirectory(File dir, int numTabs);
};

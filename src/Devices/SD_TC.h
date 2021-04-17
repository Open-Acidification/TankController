#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <SD_CI.h>
#else
#include <SD.h>
#endif

class SDClass_TC : public SDClass {
public:
  // class methods
  static SDClass_TC* instance();

  // instance methods
  void appendToDataLog(String header, String data);
  void appendToSerialLog(String data);
  void printRootDirectory();

private:
  // class variables
  static SDClass_TC* _instance;

  // instance methods
  void appendToPath(String data, String path);
  void printDirectory(File dir, int numTabs);
};

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
  void printRootDirectory();

private:
  // class variables
  static SDClass_TC* _instance;

  // instance methods
  void printDirectory(File dir, int numTabs);
};

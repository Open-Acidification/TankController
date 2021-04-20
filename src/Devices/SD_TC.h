#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <SD_CI.h>
#else
#include <SD.h>
#endif

typedef void (*visitor)(File entry, String parentPath);

class SD_TC : public SDClass {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendToDataLog(String header, String data);
  void appendToSerialLog(String data);
  String todaysDataFileName();
  void printRootDirectory();
  void visit(visitor pFunction);

private:
  // class variables
  static SD_TC* _instance;

  // instance methods
  void appendDataToPath(String data, String path);
  void visit(visitor pFunction, File dir, String parentPath);
};

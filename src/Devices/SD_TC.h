#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <SD_CI.h>
#else
#include <SD.h>
#endif

typedef void (*visitor)(File* pEntry, String parentPath);

class SD_TC : public SDClass {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendData(String header, String line);
  void appendToLog(String line);
  String todaysDataFileName();
  void printRootDirectory();
  void visit(visitor pFunction);

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const int IO_PIN = 10;
  const int SELECT_PIN = 4;
  bool hasHadError = false;

  // instance methods
  SD_TC();
  void appendDataToPath(String data, String path);
  void visit(visitor pFunction, File* pDir, String parentPath);
};

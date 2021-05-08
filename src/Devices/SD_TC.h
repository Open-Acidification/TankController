#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <SD_CI.h>
#else
#include <SD.h>
#endif

typedef void (*visitor)(File entry, const String parentPath);

class SD_TC : public SDClass {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendToDataLog(const String header, const String data);
  void appendToSerialLog(const String data);
  String todaysDataFileName();
  void printRootDirectory();
  void visit(visitor pFunction);

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const int PIN = 10;
  bool hasHadError = false;

  // instance methods
  SD_TC();
  void appendDataToPath(const String data, const String path);
  void visit(visitor pFunction, File dir, const String parentPath);
};

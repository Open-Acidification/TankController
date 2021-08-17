#pragma once

#include <Arduino.h>

#define SS 4
#include <SdFat.h>

typedef void (*visitor)(File* pEntry, String parentPath);

class SD_TC {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendData(String header, String line);
  void appendToLog(String line);
  bool exists(const char* path);
  bool format();
  bool mkdir(const char* path);
  File open(const char* path, oflag_t oflag = 0x00);
  String todaysDataFileName();
  void printRootDirectory();
  void visit(visitor pFunction);

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const uint8_t SD_SELECT_PIN = SS;
  bool hasHadError = false;
  SdFat sd;

  // instance methods
  SD_TC();
  void appendDataToPath(String data, String path);
};

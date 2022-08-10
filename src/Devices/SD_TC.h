#pragma once

#include <Arduino.h>

#define SS 4
#include <SdFat.h>

typedef void (*doOnFile)(File*);

class SD_TC {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendData(const char* header, const char* line);
  void appendToLog(const char* line);
  bool exists(const char* path);
  bool format();
  void listRootToBuffer(void (*callWhenFull)(char*, bool));
  int countFiles();
  bool mkdir(const char* path);
  File open(const char* path, oflag_t oflag = 0x00);
  void printRootDirectory();
  bool remove(const char* path);
  void todaysDataFileName(char* path, int size);

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const uint8_t SD_SELECT_PIN = SS;
  bool hasHadError = false;
  SdFat sd;
  const int maxDepth = 3; // Max depth of folders to list in rootdir()
  File fileStack[3]; // Agrees with maxDepth above. Each is 64 bytes
  int fileStackSize;
  bool inProgress = false;

  // instance methods
  SD_TC();
  void appendDataToPath(const char* data, const char* path);
  void listFiles(void (*callWhenFull)(char*, bool), byte tabulation = 0);
  void iterateOnFiles(doOnFile functionName);
  static void incrementFileCount(File* myFile);
};

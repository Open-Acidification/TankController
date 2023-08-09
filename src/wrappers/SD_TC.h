#pragma once

#include <Arduino.h>

#define MAX_DEPTH 2
#define SS 4
#include <SdFat.h>

typedef bool (*doOnFile)(File*, void*);

struct listFilesData_t {
  char buffer[250];  // Each line should be 24 characters long; 10 lines
  int linePos;
  int filesWritten;
};

class SD_TC {
public:
  // class methods
  static SD_TC* instance();

  // instance methods
  void appendData(const char* header, const char* line);
  void appendToLog(const char* line);
  bool exists(const char* path);
  bool format();
  bool listRootToBuffer(void (*callWhenFull)(const char*, bool));
  bool countFiles(void (*callWhenFinished)(int));
  bool mkdir(const char* path);
  File open(const char* path, oflag_t oflag = 0x00);
  void printRootDirectory();
  bool remove(const char* path);
  void todaysDataFileName(char* path, int size);

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  char mostRecentHeader[128] = "";
  char mostRecentLine[128] = "";
#endif

private:
  // class variables
  static SD_TC* _instance;

  // instance variables
  const uint8_t SD_SELECT_PIN = SS;
  bool hasHadError = false;
  SdFat sd;

  // Max depth of file system search for rootdir()
  // Two is minimum: First for root, second for files
  // Each is 64 bytes
  File fileStack[MAX_DEPTH];
  int fileStackSize;
  int fileCount;
  bool inProgress = false;

  // instance methods
  SD_TC();
  void appendDataToPath(const char* data, const char* path);
  bool iterateOnFiles(doOnFile functionName, void* userData);
  static bool incrementFileCount(File* myFile, void* pFileCount);
  static bool listFile(File* myFile, void* userData);
};

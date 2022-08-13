#include "Devices/SD_TC.h"

#include "DateTime_TC.h"
#include "Serial_TC.h"
#include "TC_util.h"

//  class variables
SD_TC* SD_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
SD_TC* SD_TC::instance() {
  if (!_instance) {
    _instance = new SD_TC();
  }
  return _instance;
}

//  instance methods

/**
 * constructor
 */
SD_TC::SD_TC() {
  Serial.println(F("SD_TC()"));  // Serial_TC might not be ready yet
  assert(_instance == nullptr);
  if (!sd.begin(SD_SELECT_PIN)) {
    Serial.println(F("SD_TC failed to initialize!"));
  }
}

/**
 * append data to a data log file
 */
void SD_TC::appendData(const char* header, const char* line) {
  char path[30];
  todaysDataFileName(path, sizeof(path));
  if (!sd.exists(path)) {
    appendDataToPath(header, path);
    COUT(header);
  }
  appendDataToPath(line, path);
  COUT(line);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(const char* line, const char* path) {
  COUT(path);
  File file = sd.open(path, O_CREAT | O_WRONLY | O_APPEND);
  if (file) {
    file.write(line);
    file.write("\n", 1);
    file.close();
    COUT(file);
  } else {
    if (!hasHadError) {
      hasHadError = true;
      serial(F("Unable to open file: \"%s\""), path);
      COUT("Unable to open file: \"" << path << "\"");
      return;
    }
  }
}

/**
 * append data to a serial log file
 */
void SD_TC::appendToLog(const char* line) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf_P(path, sizeof(path), (PGM_P)F("%4i%02i%02i.log"), now.year(), now.month(), now.day());
  appendDataToPath(line, path);
}

bool SD_TC::exists(const char* path) {
  return sd.exists(path);
}

bool SD_TC::format() {
  return sd.format();
}

bool SD_TC::iterateOnFiles(doOnFile functionName, void* userData) {
#ifndef MOCK_PINS_COUNT
  // Only called on real device
  // Returns false only when all files have been iterated on
  bool flag = true;
  while (flag) {
    if (fileStack[fileStackSize].openNext(&fileStack[fileStackSize - 1], O_READ)) {
      if (!fileStack[fileStackSize].isHidden()) {
        flag = functionName(&fileStack[fileStackSize], userData);
        if (fileStack[fileStackSize].isDir()) {
          // maxDepth was set to 2 in SD_TC.h
          // So this code is untested
          if (fileStackSize < maxDepth - 1) {
            ++fileStackSize;
          };
        } else {
          // Close current file; directories are closed later
          fileStack[fileStackSize].close();
        }
      }
    } else {
      // We're done with a directory
      fileStack[--fileStackSize].close();
      if (fileStackSize == 0) {
        return false;  // Done with root---there are no more files
      }
    }
  }
  return true;  // There are (probably) more files remaining
#else
  return false;
#endif
}

bool SD_TC::incrementFileCount(File* myFile, void* pFileCount) {
  return ++(*(int*)pFileCount) % 10 != 0;  // Pause after counting 10 files
}

void SD_TC::countFiles(void (*callWhenFinished)(int)) {
  if (!inProgress) {
    const char path[] PROGMEM = "/";
    fileStack[0] = SD_TC::instance()->open(path);
    if (!fileStack[0]) {
      serial(F("SD_TC open() failed"));
      return;
    }
    fileStack[0].rewind();
    fileStackSize = 1;
    fileCount = 0;
    inProgress = true;
  }
  inProgress = iterateOnFiles(incrementFileCount, (void*)&fileCount);
  if (!inProgress) {
    callWhenFinished(fileCount);
  }
}

struct listFilesData_t {
  // Each line should be 24 characters long; 10 lines
  char buffer[250];  // Flawfinder: ignore
  int linePos;       // Flawfinder: ignore
  int filesWritten;  // Flawfinder: ignore
};

// Issue: This function does not visually display depth for items in subfolders
// With maxDepth set to 2, no subfolders are traversed
bool SD_TC::listFile(File* myFile, void* userData) {
#ifndef MOCK_PINS_COUNT
  listFilesData_t* pListFileData = (listFilesData_t*)userData;  // Flawfinder: ignore
  char fileName[15];
  myFile->getName(fileName, sizeof(fileName));
  int bytesWritten;
  if (myFile->isDir()) {
    bytesWritten = snprintf_P(pListFileData->buffer + pListFileData->linePos,
                              sizeof(pListFileData->buffer) - pListFileData->linePos,
                              (PGM_P)F("%11.11s/          \r\n"), fileName);
  } else {
    bytesWritten = snprintf_P(pListFileData->buffer + pListFileData->linePos,
                              sizeof(pListFileData->buffer) - pListFileData->linePos, (PGM_P)F("%s\t%6u KB\r\n"),
                              fileName, myFile->fileSize() / 1024 + 1);
  }
  // "Overwrite" null terminator
  pListFileData->linePos += bytesWritten;
  return (++(pListFileData->filesWritten)) % 10 != 0;  // Stop iterating after 10 files
#else
  return false;
#endif
}

void SD_TC::listRootToBuffer(void (*callWhenFull)(char*, bool)) {
#ifndef MOCK_PINS_COUNT
  if (!inProgress) {
    const char path[] PROGMEM = "/";
    fileStack[0] = SD_TC::instance()->open(path);
    if (!fileStack[0]) {
      serial(F("SD_TC open() failed"));
      return;
    }
    fileStack[0].rewind();
    fileStackSize = 1;
    inProgress = true;
  }
  listFilesData_t listFileData;
  listFileData.linePos = 0;
  listFileData.filesWritten = 0;
  inProgress = iterateOnFiles(listFile, (void*)&listFileData);
  // Terminate the buffer
  listFileData.buffer[listFileData.linePos] = '\0';
  callWhenFull(listFileData.buffer, !inProgress);
#else
  static const char notImplemented[] PROGMEM = "Root directory not supported by CI framework.\r\n";
  char buffer[sizeof(notImplemented)];
  memcpy(buffer, (PGM_P)notImplemented, sizeof(notImplemented));
  callWhenFull(buffer, true);
#endif
}

bool SD_TC::mkdir(const char* path) {
  return sd.mkdir(path);
}

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

bool SD_TC::remove(const char* path) {
  return sd.remove(path);
}

void SD_TC::printRootDirectory() {
  sd.ls(LS_DATE | LS_SIZE | LS_R);
}

void SD_TC::todaysDataFileName(char* path, int size) {
  DateTime_TC now = DateTime_TC::now();
  snprintf_P(path, size, (PGM_P)F("%4i%02i%02i.csv"), now.year(), now.month(), now.day());
  COUT(path);
}

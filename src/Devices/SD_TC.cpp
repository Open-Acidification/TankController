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

int fileCount;

void SD_TC::incrementFileCount(File* myFile) {
  ++fileCount;
}

int SD_TC::countFiles() {
  fileCount = 0;
  iterateOnFiles(incrementFileCount);
  return fileCount;
}

void SD_TC::iterateOnFiles(doOnFile functionName) {
  // Only called on real device
#ifndef MOCK_PINS_COUNT
  const char path[] PROGMEM = "/";
  fileStack[0] = SD_TC::instance()->open(path);
  fileStack[0].rewind();
  fileStackSize = 1;
  inProgress = true;

  while (true) {
    if (fileStack[fileStackSize].openNext(&fileStack[fileStackSize - 1], O_READ)) {
      if (!fileStack[fileStackSize].isHidden()) {
        functionName(&fileStack[fileStackSize]);
        if (fileStack[fileStackSize].isDir()) {
          if(fileStackSize < maxDepth - 1) {fileStackSize++;};
        } else {
          // Close current file; directories are closed later
          fileStack[fileStackSize].close();
        }
      }
    } else {
      // We're done with a directory
      if (fileStackSize == 1) {
        // We're done with root
        fileStack[0].close();
        inProgress = false;
        break;
      }
      fileStack[fileStackSize--].close();
    }
  }
#endif
}

void SD_TC::listFiles(void (*callWhenFull)(char*, bool), byte tabulation) {
  // Only called on real device
#ifndef MOCK_PINS_COUNT
  char fileName[15];
  char buffer[250];  // Each line should be 24 characters long
  int linePos = 0;
  int filesWritten = 0;  // When we hit 10 here we will call buffer and suspend

  while (filesWritten < 10) {
    if (fileStack[fileStackSize].openNext(&fileStack[fileStackSize - 1], O_READ)) {
      if (!fileStack[fileStackSize].isHidden()) {
        fileStack[fileStackSize].getName(fileName, sizeof(fileName));
        if (fileStack[fileStackSize].isDir()) {
          int bytesWritten = snprintf_P(buffer + linePos, sizeof(buffer) - linePos, (PGM_P)F("%11.11s/          \r\n"), fileName);
          // "Overwrite" null terminator
          linePos += bytesWritten;
          ++filesWritten;
          if(fileStackSize < maxDepth - 1) {fileStackSize++;};
        } else {
          int bytesWritten = snprintf_P(buffer + linePos, sizeof(buffer) - linePos, (PGM_P)F("%s\t%6u KB\r\n"),
                                        fileName, fileStack[fileStackSize].fileSize() / 1024 + 1);
          // "Overwrite" null terminator
          linePos += bytesWritten;
          ++filesWritten;
          // Close current file; directories are closed later
          fileStack[fileStackSize].close();
        }
      }
    } else {
      // We're done with a directory
      if (fileStackSize == 1) {
        // We're done with root
        fileStack[0].close();
        buffer[linePos] = '\0';
        inProgress = false;
        callWhenFull(buffer, true);
        return;
      }
      fileStack[fileStackSize--].close();
    }
  }
  // We have 10 lines written, so add null terminator
  buffer[linePos] = '\0';
  callWhenFull(buffer, false);
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
  listFiles(callWhenFull);
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

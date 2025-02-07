#include "wrappers/SD_TC.h"

#include "model/DataLogger.h"
#include "model/RemoteLogPusher.h"
#include "model/TC_util.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/Serial_TC.h"

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

/**
 * delete singleton
 */
void SD_TC::deleteInstance() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

//  instance methods

/**
 * constructor
 */
SD_TC::SD_TC() {
  Serial.println(F("SD_TC"));  // Serial_TC might not be ready yet
  assert(_instance == nullptr);
  if (!sd.begin(SD_SELECT_PIN)) {
    Serial.println(F("SD_TC failed to initialize!"));
  }
  remoteLogName[0] = '\0';
}

/**
 * append data to a data log file
 */
void SD_TC::writeToDataLog(const char* header, const char* line) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  strncpy(mostRecentDataLogHeader, header, sizeof(mostRecentDataLogHeader));  // Flawfinder: ignore
  mostRecentDataLogHeader[sizeof(mostRecentDataLogHeader) - 1] = '\0';        // Ensure null-terminated string
  strncpy(mostRecentDataLogLine, line, sizeof(mostRecentDataLogLine));        // Flawfinder: ignore
  mostRecentDataLogLine[sizeof(mostRecentDataLogLine) - 1] = '\0';            // Ensure null-terminated string
#endif
  char path[30];
  todaysDataFileName(path, sizeof(path));
  if (!sd.exists(path)) {
    appendStringToPath(header, path);
  }
  appendStringToPath(line, path);
}

/**
 * append data to a path
 */
void SD_TC::appendStringToPath(const char* line, const char* path, bool appendNewline) {
  File file = sd.open(path, O_CREAT | O_WRONLY | O_APPEND);
  if (file) {
    file.write(line);
    if (appendNewline) {
      file.write("\n", 1);
    }
    file.close();
  } else {
    if (!hasHadError) {
      hasHadError = true;
      serial(F("Unable to open file: \"%s\""), path);
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
  appendStringToPath(line, path);
}

bool SD_TC::countFiles(void (*callWhenFinished)(int)) {
  if (!inProgress) {
    const char path[] PROGMEM = "/";
    fileStack[0] = SD_TC::instance()->open(path);
    if (!fileStack[0]) {
      serial(F("SD_TC open() failed"));
      return false;  // Function is unsuccessful
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
  return true;
}

bool SD_TC::exists(const char* path) {
  return sd.exists(path);
}

bool SD_TC::format() {
  return sd.format();
}

void SD_TC::getRemoteLogContents(char* buffer, int size, uint32_t index) {
  buffer[0] = '\0';
  const char* logName = getRemoteLogName();
  if (logName[0] == '\0') {
    return;
  }
  File file = open(logName, O_RDONLY);
  if (file) {
    file.seek(index);
    int remaining = file.available();
    if (remaining > 0) {
      int toRead = min(size - 1, remaining);
      int readSize = file.read(buffer, toRead);  // Flawfinder: ignore
      if (readSize == toRead) {
        buffer[readSize] = '\0';
      } else {
        buffer[0] = '\0';
      }
    }
    file.close();
  }
}

bool SD_TC::iterateOnFiles(doOnFile functionName, void* userData) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  return false;  // no more files
#else
  // Only called on real device
  // Returns false only when all files have been iterated on
  bool flag = true;
  while (flag) {
    if (fileStack[fileStackSize].openNext(&fileStack[fileStackSize - 1], O_READ)) {
      if (!fileStack[fileStackSize].isHidden()) {
        flag = functionName(&fileStack[fileStackSize], userData);
        if (fileStack[fileStackSize].isDir()) {
          // MAX_DEPTH was set to 2 in SD_TC.h
          // So this code is untested
          if (fileStackSize < MAX_DEPTH - 1) {
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
#endif
}

bool SD_TC::incrementFileCount(File* myFile, void* pFileCount) {
  return ++(*(int*)pFileCount) % 10 != 0;  // Pause after counting 10 files
}

// Issue: This function does not visually display depth for items in subfolders
// With MAX_DEPTH set to 2, no subfolders are traversed
bool SD_TC::listFile(File* myFile, void* userData) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  assert(myFile != nullptr);
  return false;
#else
  listFilesData_t* pListFileData = static_cast<listFilesData_t*>(userData);
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
#endif
}

bool SD_TC::listRootToBuffer(void (*callWhenFull)(const char*, bool)) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  const char buffer[] = "Root directory not supported by CI framework.\r\n";
  callWhenFull(buffer, true);
  return true;
#else
  if (!inProgress) {
    const char path[] PROGMEM = "/";
    fileStack[0] = SD_TC::instance()->open(path);
    if (!fileStack[0]) {
      serial(F("SD_TC open() failed"));
      return false;  // Function is unsuccessful
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
  return true;
#endif
}

bool SD_TC::mkdir(const char* path) {
  return sd.mkdir(path);
}

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

void SD_TC::printRootDirectory() {
  sd.ls(LS_DATE | LS_SIZE | LS_R);
}

bool SD_TC::remove(const char* path) {
  return sd.remove(path);
}

void SD_TC::setRemoteLogName(const char* newFileName) {
  if (newFileName == nullptr || newFileName[0] == '\0') {
    remoteLogName[0] = '\0';
    return;
  }
  // See TankController.ino for the definition of remoteLogName
  if (strnlen(newFileName, MAX_FILE_NAME_LENGTH + 1) <= MAX_FILE_NAME_LENGTH) {
    // valid file name has been provided (See TankController.ino)
    snprintf_P(remoteLogName, MAX_FILE_NAME_LENGTH + 5, PSTR("%s.log"), newFileName);
  }
}

void SD_TC::todaysDataFileName(char* path, int size) {
  DateTime_TC now = DateTime_TC::now();
  snprintf_P(path, size, (PGM_P)F("%4i%02i%02i.csv"), now.year(), now.month(), now.day());
}

void SD_TC::updateRemoteFileSize() {
  if (remoteLogName[0] == '\0') {
    remoteFileSize = 0;
    return;
  }
  File file = open(remoteLogName, O_RDONLY);
  if (file) {
    remoteFileSize = file.size();
    file.close();
  } else {
    remoteFileSize = 0;
  }
}

/**
 * @brief write to the appropriate "remote" file on the SD card
 *
 * @param line
 */
void SD_TC::writeToRemoteLog(const char* line) {
#if defined(ARDUINO_CI_COMPILATION_MOCKS)
  strncpy(mostRecentRemoteLogEntry, line, sizeof(mostRecentRemoteLogEntry));  // Flawfinder: ignore
  mostRecentRemoteLogEntry[sizeof(mostRecentRemoteLogEntry) - 1] = '\0';      // Ensure null-terminated string
#endif
  const char* logName = getRemoteLogName();
  if (logName[0] == '\0') {
    return;
  }
  if (!sd.exists(logName)) {
    // rather than write an entire header line in one buffer, we break it into chunks to save memory
    char buffer[200];
    DataLogger::instance()->writeRemoteFileHeader(buffer, sizeof(buffer), 0);
    appendStringToPath(buffer, remoteLogName, false);
    DataLogger::instance()->writeRemoteFileHeader(buffer, sizeof(buffer), 1);
    appendStringToPath(buffer, remoteLogName, false);
    DataLogger::instance()->writeRemoteFileHeader(buffer, sizeof(buffer), 2);
    appendStringToPath(buffer, remoteLogName, false);
    DataLogger::instance()->writeRemoteFileHeader(buffer, sizeof(buffer), 3);
    appendStringToPath(buffer, remoteLogName);
  }
  appendStringToPath(line, remoteLogName);
  updateRemoteFileSize();
  RemoteLogPusher::instance()->pushSoon();
}

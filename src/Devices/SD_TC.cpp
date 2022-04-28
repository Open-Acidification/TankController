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

bool SD_TC::mkdir(const char* path) {
  return sd.mkdir(path);
}

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

void SD_TC::printRootDirectory() {
  sd.ls(LS_DATE | LS_SIZE | LS_R);
}

void SD_TC::listRootToBuffer(void (*callWhenFull)(char*, bool)) {
#ifndef MOCK_PINS_COUNT
  if (!inProgress) {
    // Initialize hierarchy
    hierarchy = new File;
    ++hierarchySize;
    const char path[] PROGMEM = "/";
    File root = SD_TC::instance()->open(path);
    if (!root) {
      serial(F("SD_TC open() failed"));
      return;
    }
    // Add root to hierarchy if successful
    root.rewind();
    hierarchy[0] = root;
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

void SD_TC::listFiles(void (*callWhenFull)(char*, bool), byte tabulation) {
  // Only called on real device
#ifndef MOCK_PINS_COUNT
  File* parent = &hierarchy[hierarchySize - 1];
  File current;
  char fileName[15];
  char buffer[300];  // Each line shouldn't be more than 30 characters long
  int linePos = 0;
  int filesWritten = 0;  // When we hit 10 here we will call buffer and suspend

  while (filesWritten < 10) {
    if (current.openNext(parent, O_READ)) {
      if (!current.isHidden()) {
        current.getName(fileName, sizeof(fileName));
        for (int i = 0; i < tabulation; i++) {
          buffer[i] = '\t';
          ++linePos;
        }
        if (current.isDir()) {
          int bytesWritten = snprintf_P(buffer + linePos, sizeof(buffer) - linePos, (PGM_P)F("%s/\n"), fileName);
          // "Overwrite" null terminator
          linePos += bytesWritten;
          ++filesWritten;
          // Create a new array with bigger size
          File* newHierarchy = new File[hierarchySize + 1];
          memcpy(newHierarchy, hierarchy, sizeof(File) * hierarchySize);
          delete[] hierarchy;
          hierarchy = newHierarchy;
          ++hierarchySize;
          // Add the current file to the new array
          hierarchy[hierarchySize - 1] = current;
          // Now we change parent directory
          parent = &hierarchy[hierarchySize - 1];
        } else {
          // We'll say nothing is bigger than 10 MB
          int bytesWritten = snprintf_P(buffer + linePos, sizeof(buffer) - linePos, (PGM_P)F("%s\t%10lu B\n"), fileName,
                                        (unsigned long)current.size());
          // "Overwrite" null terminator
          linePos += bytesWritten;
          ++filesWritten;
          // Close current (if it's a file); close current (if it's a directory) as a parent later
          current.close();
        }
      }
    } else {
      if (hierarchySize == 1) {
        // We're done listing root
        parent->close();
        --hierarchySize;
        delete[] hierarchy;
        hierarchy = nullptr;
        buffer[linePos] = '\0';
        inProgress = false;
        callWhenFull(buffer, true);
        return;
      }
      // All done with parent, remove directory from hierarchy
      parent->close();
      --hierarchySize;
      parent = &hierarchy[hierarchySize - 1];
    }
  }
  // We have 10 lines written, so add null terminator
  buffer[linePos] = '\0';
  callWhenFull(buffer, false);
#endif
}

void SD_TC::todaysDataFileName(char* path, int size) {
  DateTime_TC now = DateTime_TC::now();
  snprintf_P(path, size, (PGM_P)F("%4i%02i%02i.csv"), now.year(), now.month(), now.day());
  COUT(path);
}

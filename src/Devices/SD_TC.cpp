#include "Devices/SD_TC.h"

#include "DateTime_TC.h"
#include "Serial_TC.h"
#define DEBUG 0
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
    serial("SD_TC::instance() called SD_TC::SD_TC()");
  }
  return _instance;
}

//  instance methods

/**
 * constructor
 */
SD_TC::SD_TC() {
  assert(_instance == nullptr);
  pinMode(IO_PIN, OUTPUT);
  digitalWrite(IO_PIN, HIGH);
  if (!begin(SELECT_PIN)) {
    Serial.println("SD_TC failed to initialize!");
  }
}

/**
 * append data to a data log file
 */
void SD_TC::appendData(String header, String line) {
  String path = todaysDataFileName();
  if (!exists(path.c_str())) {
    appendDataToPath(header, path.c_str());
    COUT(header);
  }
  appendDataToPath(line, path.c_str());
  COUT(data);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(String line, String path) {
  serial("SD_TC::appendDataToPath(\"%s\", \"%s\")", line.c_str(), path.c_str());
  int i = path.indexOf('/', 1);
  while (i > 0) {
    String s = path.substring(1, i);
    if (!SD.exists(s.c_str())) {
      if (!SD.mkdir(s.c_str())) {
        if (!hasHadError) {
          hasHadError = true;
          serial("Unable to create directory: \"%s\"", s.c_str());
          COUT("Unable to create directory: \"" << s.c_str() << "\"");
        }
        return;
      }
    }
    i = path.indexOf('/', i + 2);
  }
  File file = SD_TC::instance()->open(path, FILE_WRITE);
  if (file) {
    file.write(line.c_str(), line.length());
    file.write("\n", 1);
    file.close();
  } else {
    if (!hasHadError) {
      hasHadError = true;
      serial("Unable to open file: \"%s\"", path.c_str());
      COUT("Unable to open file: \"" << path.c_str() << "\"");
      return;
    }
  }
}

/**
 * append data to a serial log file
 */
void SD_TC::appendToLog(String line) {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "%4i-%02i-%02i.log", now.year(), now.month(), now.day());
  appendDataToPath(line, path);
}

void printEntry(File* pEntry, String parentPath) {
  size_t depth = 0;
  for (size_t i = 1; i < parentPath.length(); ++i) {
    if (parentPath[i] == '/') {
      ++depth;
    }
  }
  char prefix[] = "- - - - - - - - ";
  if (depth * 2 < strnlen(prefix, sizeof(prefix))) {
    prefix[depth * 2] = '\0';
  }
  Serial.print(prefix);
  Serial.print(pEntry->name());
  if (pEntry->isDirectory()) {
    Serial.println("/");
    serial("%s%12s/", prefix, pEntry->name());  // This line causes a crash
  } else {
    // files have sizes, directories do not
    Serial.print(" (");
    Serial.print(pEntry->size());
    Serial.println(")");
    // serial("%s%12s (%6u)", prefix, entry.name(), entry.size());  // This line crashes!
  }
}

/**
 * print the root directory and all subdirectories
 */
void SD_TC::printRootDirectory() {
  serial("SD_TC::printRootDirectory() - start");
  visit(printEntry);
  serial("SD_TC::printRootDirectory() - end");
}

String SD_TC::todaysDataFileName() {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "%4i-%02i-%02i.csv", now.year(), now.month(), now.day());
  COUT(path);
  return String(path);
}

void SD_TC::visit(visitor pFunction) {
  File root = open("/");
  if (root) {
    visit(pFunction, &root, "/");
  } else {
    serial("Unable to open root directory of SD card!");
  }
}

void SD_TC::visit(visitor pFunction, File* pDir, String parentPath) {
  int i = 0;
  while (i++ < 100) {
    File entry = pDir->openNextFile();
    if (!entry) {
      return;  // no more files
    }
    pFunction(&entry, parentPath);
    if (entry.isDirectory()) {
      visit(pFunction, &entry, parentPath + entry.name() + "/");
    }
    entry.close();
  }
  serial("Stopped after 100 entries");
}

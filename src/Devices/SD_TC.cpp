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
  Serial.println("SD_TC()");  // Serial_TC might not be ready yet
  assert(_instance == nullptr);
  if (!sd.begin(SD_SELECT_PIN)) {
    Serial.println("SD_TC failed to initialize!");
  }
}

/**
 * append data to a data log file
 */
void SD_TC::appendData(String header, String line) {
  String path = todaysDataFileName();
  if (!sd.exists(path.c_str())) {
    appendDataToPath(header, path.c_str());
    COUT(header);
  }
  appendDataToPath(line, path.c_str());
  COUT(line);
}

/**
 * append data to a path
 */
void SD_TC::appendDataToPath(String line, String path) {
  COUT(path);
  File file = sd.open(path, O_CREAT | O_WRONLY | O_APPEND);
  if (file) {
    file.write(line.c_str(), line.length());
    file.write("\n", 1);
    file.close();
    COUT(file);
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
  snprintf(path, sizeof(path), "%4i%02i%02i.log", now.year(), now.month(), now.day());
  appendDataToPath(line, path);
}

bool SD_TC::exists(const char* path) {
  return sd.exists(path);
}

bool SD_TC::format() {
  std::cout << "SD_TC::format()" << std::endl;
  return sd.format();
}

bool SD_TC::mkdir(const char* path) {
  return sd.mkdir(path);
}

File SD_TC::open(const char* path, oflag_t oflag) {
  return sd.open(path, oflag);
}

/**
 * print the root directory and all subdirectories
 */
void SD_TC::printRootDirectory() {
  // serial("SD_TC::printRootDirectory()");
}

String SD_TC::todaysDataFileName() {
  DateTime_TC now = DateTime_TC::now();
  char path[30];
  snprintf(path, sizeof(path), "%4i%02i%02i.csv", now.year(), now.month(), now.day());
  COUT(path);
  return String(path);
}

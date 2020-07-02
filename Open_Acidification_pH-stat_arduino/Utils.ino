// https://stackoverflow.com/questions/15515088/how-to-check-if-string-starts-with-certain-string-in-c/15515276
bool startsWith(const char *a, const char *b) {
  if (strncmp(a, b, strlen(b)) == 0)
    return 1;
  return 0;
}
void printDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(F(":"));
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void recursiveRm(File dir, String tempPath) {
  Serial.println("CURRENT TEMPPATH: ");
  Serial.println(tempPath);
  while (true) {
    File entry = dir.openNextFile();
    String localPath;

    Serial.println("CURRENT ENTRY: ");
    Serial.println(entry.name());
    if (entry) {
      if (entry.isDirectory()) {
        localPath = tempPath + entry.name() + "/" + '\0';
        char folderBuf[localPath.length()];
        localPath.toCharArray(folderBuf, localPath.length());
        recursiveRm(entry, folderBuf);

        if (SD.rmdir(folderBuf)) {
          Serial.print("Deleted folder ");
          Serial.println(folderBuf);
        } else {
          Serial.print("Unable to delete folder ");
          Serial.println(folderBuf);
        }
      } else {
        localPath = tempPath + entry.name() + '\0';
        char charBuf[localPath.length()];
        localPath.toCharArray(charBuf, localPath.length());

        if (SD.remove(charBuf)) {
          Serial.print("Deleted ");
          Serial.println(localPath);
        } else {
          Serial.print("Failed to delete ");
          Serial.println(localPath);
        }
      }
      entry.close();
    } else {
      // break out of recursion
      break;
    }
  }
  // delete specified root dir
  if (SD.rmdir(tempPath)) {
    Serial.print("Deleted root dir: ");
    Serial.println(tempPath);
  } else {
    Serial.print("Failed to delete root dir: ");
    Serial.println(tempPath);
  }
}
void doDirectoryMaintenance() {  // TODO: investigate bug with un-deletable files
  char nameBuffer[20];
  char deleteBuffer[20];
  memset(nameBuffer, 0, 20);
  strcat(nameBuffer, "/");
  DateTime now = rtc.now();
  int day = now.day();

  int year = now.year();
  Serial.println("CURRENT YEAR IS: ");
  Serial.println(year);
  char yearBuffer[4];
  itoa(year, yearBuffer, 10);
  strcat(nameBuffer, yearBuffer);
  Serial.println("NAME BUFFER IS: ");
  Serial.println(nameBuffer);

  int month = now.month();
  Serial.println("CURRENT MONTH IS: ");
  Serial.println(month);
  strcat(nameBuffer, "/");
  if (month < 10) {
    strcat(nameBuffer, "0");
  }
  char monthBuffer[12];
  itoa(month, monthBuffer, 10);
  strcat(nameBuffer, monthBuffer);
  Serial.println("NAME BUFFER IS: ");
  Serial.println(nameBuffer);
  File tempDir = SD.open(nameBuffer);

  // for relevant directories
  while (true) {
    File entry = tempDir.openNextFile();
    if (entry) {
      Serial.println("ENTRY NAME IS: ");
      Serial.println(entry.name());
      // calculate elapsed time
      int dirAgeInDays = day - atoi(entry.name());
      Serial.println("ENTRY AGE IS: ");
      Serial.println(dirAgeInDays);
      // remove if directory is too old
      if (dirAgeInDays > maxDataAge) {
        Serial.println("ENTRY IS TOO OLD");
        memset(deleteBuffer, 0, 20);
        strcat(deleteBuffer, nameBuffer);
        strcat(deleteBuffer, "/");
        strcat(deleteBuffer, entry.name());
        recursiveRm(entry, deleteBuffer);
      }
    } else {
      break;
    }
  }
  Serial.println("maxDataAge IS: ");
  Serial.println(maxDataAge);
}

//
// Produce a formatted string in a buffer corresponding to the value provided.
// If the 'width' parameter is non-zero, the value will be padded with leading
// zeroes to achieve the specified width.  The number of characters added to
// the buffer (not including the null termination) is returned.
//
unsigned fmtUnsigned(unsigned long val, char *buf, unsigned bufLen, byte width) {
  if (!buf || !bufLen)
    return (0);

  // produce the digit string (backwards in the digit buffer)
  char dbuf[10];
  unsigned idx = 0;
  while (idx < sizeof(dbuf)) {
    dbuf[idx++] = (val % 10) + '0';
    if ((val /= 10) == 0)
      break;
  }

  // copy the optional leading zeroes and digits to the target buffer
  unsigned len = 0;
  byte padding = (width > idx) ? width - idx : 0;
  char c = '0';
  while ((--bufLen > 0) && (idx || padding)) {
    if (padding)
      padding--;
    else
      c = dbuf[--idx];
    *buf++ = c;
    len++;
  }

  // add the null termination
  *buf = '\0';
  return (len);
}

//
// Format a floating point value with number of decimal places.
// The 'precision' parameter is a number from 0 to 6 indicating the desired decimal places.
// The 'buf' parameter points to a buffer to receive the formatted string.  This must be
// sufficiently large to contain the resulting string.  The buffer's length may be
// optionally specified.  If it is given, the maximum length of the generated string
// will be one less than the specified value.
//
// example: fmtDouble(3.1415, 2, buf); // produces 3.14 (two decimal places)
//
void fmtDouble(double val, byte precision, char *buf) {
  unsigned bufLen = 0xffff;
  if (!buf || !bufLen)
    return;

  // limit the precision to the maximum allowed value
  const byte maxPrecision = 6;
  if (precision > maxPrecision)
    precision = maxPrecision;

  if (--bufLen > 0) {
    // check for a negative value
    if (val < 0.0) {
      val = -val;
      *buf = '-';
      bufLen--;
    }

    // compute the rounding factor and fractional multiplier
    double roundingFactor = 0.5;
    unsigned long mult = 1;
    for (byte i = 0; i < precision; i++) {
      roundingFactor /= 10.0;
      mult *= 10;
    }

    if (bufLen > 0) {
      // apply the rounding factor
      val += roundingFactor;

      // add the integral portion to the buffer
      unsigned len = fmtUnsigned((unsigned long)val, buf, bufLen);
      buf += len;
      bufLen -= len;
    }

    // handle the fractional portion
    if ((precision > 0) && (bufLen > 0)) {
      *buf++ = '.';
      if (--bufLen > 0)
        buf += fmtUnsigned((unsigned long)((val - (unsigned long)val) * mult), buf, bufLen, precision);
    }
  }

  // null-terminate the string
  *buf = '\0';
}
#include "string.h"

void clearBuffer(char *htmlRequestBuffer, int size) {
  memset(htmlRequestBuffer, ' ', size);
}
#include <string>

String readUntilSpace(char *buffer, int &end, int size) {
  String result = "";
  int start = end;
  char c = buffer[end];
  while (c == ' ' || c == '\n' || c == '\r') {
    c = buffer[++end];
    start++;
  }
  while (c != ' ' && c != '\n' && c != '\r' && end < size) {
    c = buffer[++end];
  }
  for (int i = start; i < end; i++) {
    result += buffer[i];
  }
  return result;
}
long getLastJsonArrayValue(JsonArray array) {
  JsonArray::iterator it = array.begin();
  JsonArray::iterator it2 = array.begin();
  for (it; it != array.end();) {
    it++;
    if (it == array.end()) {
      break;
    } else {
      it2++;
    }
  }
  return it2->as<long>();
}

void handleDevice(String endpoint, EthernetClient client) {
  // send all device info
  printHeader(client, 200);
  StaticJsonDocument<200> doc;
  JsonObject object = doc.to<JsonObject>();
  object["granularity"] = granularity;
  object["maxDataAge"] = maxDataAge;
  object["macstr"] = macstr;
  object["tankid"] = tankid;
  serializeJson(doc, client);
}
void handleGoal(EthernetClient client) {
  printHeader(client, 200);

  StaticJsonDocument<200> goalDoc;
  JsonObject goalObj = goalDoc.to<JsonObject>();

  StaticJsonDocument<100> phValuesDoc;
  JsonArray phValues = phValuesDoc.to<JsonArray>();
  for (int i = 0; i < phSeriesSize; i++) {
    long phValueCurrent = readLineFromSD("pv.txt", i, goalRecordLength);
    phValues.add(phValueCurrent);
  }
  goalObj["phValues"] = phValues;

  StaticJsonDocument<100> phTimesDoc;
  JsonArray phTimes = phTimesDoc.to<JsonArray>();
  for (int i = 0; i < phSeriesSize; i++) {
    long phTimeCurrent = readLineFromSD("pt.txt", i, goalRecordLength);
    phTimes.add(phTimeCurrent);
  }
  goalObj["phTimes"] = phTimes;
  goalObj["phInterval"] = phInterval;
  goalObj["phDelay"] = phDelay;

  StaticJsonDocument<100> tempValuesDoc;
  JsonArray tempValues = tempValuesDoc.to<JsonArray>();
  for (int i = 0; i < tempSeriesSize; i++) {
    long tempValueCurrent = readLineFromSD("tv.txt", i, goalRecordLength);
    tempValues.add(tempValueCurrent);
  }
  goalObj["tempValues"] = tempValues;

  StaticJsonDocument<100> tempTimesDoc;
  JsonArray tempTimes = tempTimesDoc.to<JsonArray>();
  for (int i = 0; i < tempSeriesSize; i++) {
    long tempTimeCurrent = readLineFromSD("tt.txt", i, goalRecordLength);
    tempTimes.add(tempTimeCurrent);
  }
  goalObj["tempTimes"] = tempTimes;
  goalObj["tempInterval"] = tempInterval;
  goalObj["tempDelay"] = tempDelay;

  serializeJson(goalObj, client);
}
void handleMisc(EthernetClient client) {
  printHeader(client, 200);
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print("analog input ");
    client.print(analogChannel);
    client.print(" is ");
    client.println(sensorReading);
  }
}
void printDirectoryToClient(File dir, int numTabs, EthernetClient client) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      client.print('\t');
    }
    client.print(entry.name());
    if (entry.isDirectory()) {
      client.println("/");
      printDirectoryToClient(entry, numTabs + 1, client);
    } else {
      // files have sizes, directories do not
      client.print("\t\t");
      client.println(entry.size(), DEC);
    }
    entry.close();
  }
}
#include <string>

void handleRequest(EthernetClient client) {
  Serial.println("new client");

  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean headerFinished = false;
  String postData;
  String header;
  char* requestType;
  String endpoint;
  char* requestProtocol;
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      // read first line as header contains endpoint and request type information
      if (!headerFinished) {
        header.concat(c);
      }
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        // Here is where the POST data is.
        while (client.available()) {
          char c = client.read();
          Serial.write(c);
          postData.concat(c);
        }

        Serial.println();
        Serial.println("Parsing request");

        Serial.println("header:");
        Serial.println(header);
        Serial.println("postData:");
        Serial.println(postData);

        requestType = strtok(header.c_str(), " ");
        endpoint = String(strtok(NULL, " "));
        requestProtocol = strtok(NULL, " ");

        Serial.println("requestType:");
        Serial.println(requestType);
        Serial.println("endpoint:");
        Serial.println(endpoint);
        Serial.println("requestProtocol:");
        Serial.println(requestProtocol);
        break;
      } else if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
        headerFinished = true;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }

    // parse and handle request based on endpoint
    if (endpoint.startsWith("/config")) {
      handleConfig(postData.c_str(), client);
    } else if (endpoint.startsWith("/series")) {
      if (startsWith(requestType, "POST")) {
        // upload new timeseries
        Serial.println("POST TIMESERIES");
        handleSeries(postData.c_str(), client);
      } else if (startsWith(requestType, "GET")) {
        // return current timeseries
        Serial.println("GET TIMESERIES");
        handleGoal(client);
      } else {
        // wrong request type
        Serial.println("WRONG TIMESERIES");
        printHeader(client, 400);
      }
    } else if (endpoint.startsWith("/device")) {
      handleDevice(endpoint, client);
    } else if (endpoint.startsWith("/goal")) {
      handleGoal(client);
    } else if (endpoint.startsWith("/info")) {
      handleInfo(client);
    } else if (endpoint.startsWith("/data")) {
      handleData(endpoint, client);
    } else if (endpoint.startsWith("/lines")) {
      handleLines(endpoint, client);
    } else if (endpoint.startsWith("/test")) {
      printHeader(client, 200);
      File root = SD.open("/");
      printDirectoryToClient(root, 0, client);
    } else if (endpoint.startsWith("/mac")) {
      printHeader(client, 200);
      client.println(macstr);
    } else if (endpoint.startsWith("/time")) {
      printHeader(client, 200);
      DateTime now = rtc.now();
      char formattedSDString[50];
      char* timeFormat = "MM/DD/YYYY hh:mm:ss";
      char* formattedTime = now.toString(timeFormat);
      strcpy(formattedSDString, formattedTime);
      char buffer[100];
      snprintf(buffer, 100, "%d", tankid);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(temp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(tempset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(pH, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(phset, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(onTime, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Kp, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Ki, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      fmtDouble(Kd, 3, buffer);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      client.println(formattedSDString);
    } else {
      handleMisc(client);
    }
    return;
  }
}
void printLastLines(String dirName, EthernetClient client, long lastLines) {
  File myFile = SD.open(dirName);
  if (myFile) {
    Serial.print("LASTLINES: ");
    Serial.println(lastLines);
    int printedLines = 0;
    // seek starting byte
    String lineDirName = dirName.substring(0, dirName.length() - 12) + dirName.substring(dirName.length() - 6);
    Serial.println("lineDirName");
    Serial.println(lineDirName);
    long lineCount = readLineFromSD(lineDirName.c_str(), 0, 10);
    long startingLine = lineCount - lastLines;
    long startingByte = startingLine * (recordLength + 2);
    if (myFile.seek(startingByte)) {
      Serial.println("SUCCESSFUL SEEK");
      printHeader(client, 200);
      client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
      Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
    } else {
      Serial.println("FAILED SEEK");
      printHeader(client, 400);
      // if the starting byte is greater than file size, print an error:
      client.print("starting byte is greater than file size at file: ");
      client.println(dirName);
      // close the file:
      myFile.close();
      return;
    }
    // print specified lines
    while (printedLines++ < lastLines) {
      // fill up buffer
      char line[100];
      memset(line, 0, 100);
      for (int i = 0; i < 100 && myFile.available(); i++) {
        byte read = myFile.read();
        if ((char)read == '\n') {
          break;
        }
        line[i] = read;
      }
      // write buffer to client
      client.write(line, strlen(line));
      Serial.write(line, strlen(line));
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    client.println("error opening file at directory: ");
    client.println(dirName);
  }
}
void printSpecifiedLines(String dirName, EthernetClient client, long startingLine, long numLines) {
  File myFile = SD.open(dirName);
  if (myFile) {
    Serial.print("STARTINGLINE: ");
    Serial.println(startingLine);
    Serial.print("NUMLINES: ");
    Serial.println(numLines);
    int printedLines = 0;
    // seek starting byte
    long startingByte = startingLine * (recordLength + 2);
    if (myFile.seek(startingByte)) {
      Serial.println("SUCCESSFUL SEEK");
      printHeader(client, 200);
      client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
      Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
    } else {
      Serial.println("FAILED SEEK");
      printHeader(client, 416);
      // if the starting byte is greater than file size, print an error:
      client.print("starting byte is greater than file size at file: ");
      client.println(dirName);
      // close the file:
      myFile.close();
      return;
    }
    // print specified lines
    while (printedLines++ < numLines) {
      // fill up buffer
      char line[100];
      memset(line, 0, 100);
      for (int i = 0; i < 100 && myFile.available(); i++) {
        byte read = myFile.read();
        if ((char)read == '\n') {
          break;
        }
        line[i] = read;
      }
      // write buffer to client
      client.write(line, strlen(line));
      Serial.write(line, strlen(line));
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    printHeader(client, 404);
    client.print("error opening file at directory");
    client.println(dirName);
  }
}
void handleInfo(EthernetClient client) {
  printHeader(client, 200);
  client.println(tankid);
}
void handleLines(String endpoint, EthernetClient client) {
  // lines endpoint format: lines/year/month/day/hour
  String directoryName;
  if (endpoint.length() >= 6) {
    directoryName = endpoint.substring(6);  // remove "/lines/" prefix
  } else {
    directoryName = "/";
  }
  int slashIndex = endpoint.lastIndexOf("/");
  String hourName = endpoint.substring(slashIndex);
  directoryName.concat(hourName + ".txt");
  char* pch;
  int slashes = -1;
  pch = strtok(endpoint.c_str(), "/");
  while (pch != NULL) {
    pch = strtok(NULL, "/");
    slashes += 1;
  }
  switch (slashes) {
    case 4:
      // hour: return csv of the specific hour
      printHeader(client, 200);
      Serial.print("PRINTING LINE FILE: ");
      Serial.println(directoryName);
      long lineCount = readLineFromSD(directoryName.c_str(), 0, 10);
      Serial.print("LINE COUNT: ");
      Serial.println(lineCount);
      client.println(lineCount);
      break;
    default:
      printHeader(client, 400);
      client.println("REQUIRES FULL YEAR/MONTH/DAY/HOUR SPECIFICATION");
      break;
  }
}
void recursiveDeleteDirectory(String dirName, EthernetClient client) {
  File dir = SD.open(dirName);
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    client.println("REMOVING: ");
    if (entry.isDirectory()) {
      client.println(dirName);
      recursiveDeleteDirectory(dirName + "/" + entry.name(), client);
    } else {
      client.println(dirName + "/" + entry.name());
      SD.remove(dirName + "/" + entry.name());
    }
    SD.rmdir(entry.name());
  }
  SD.rmdir(dirName);
}
#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void* p1, const void* p2) {
  const char* aName = *(const char**)p1;
  const char* bName = *(const char**)p2;

  while (*aName != '\0' && *aName == *bName) {
    ++aName;
    ++bName;
  }

  return (*aName - *bName);
}

JsonDocument printCurrentLevelDirectories(File dir, EthernetClient client, int level) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain; charset=UTF-8");
  client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
  client.println("Keep-Alive: timeout=5, max=1000");
  client.println("X-Content-Type-Options: nosniff");
  client.println();
  Serial.println(dir.name());

  // add directories to array
  char** files;
  files = malloc(40 * sizeof(char*));
  int fileCounter = 0;
  while (true) {
    File entry = dir.openNextFile();
    Serial.println(entry.name());
    if (!entry) {
      // no more files
      break;
    }
    char* hasLetterS = strchr(entry.name(), 'S');
    if (entry.isDirectory() && !hasLetterS) {  // WILL NOT PRINT SYSTEM~1 AT ROOT LEVEL
      files[fileCounter] = malloc(10 * sizeof(char));
      strcpy(files[fileCounter], entry.name());
      fileCounter++;
    }
    entry.close();
  }

  // sort array
  qsort(files, fileCounter, sizeof(files[0]), cmpfunc);

  // add items in sorted array to json array
  StaticJsonDocument<512> doc;
  JsonArray filesArray = doc.to<JsonArray>();
  for (int i = 0; i < fileCounter; i++) {
    filesArray.add(files[i]);
  }

  // free memory
  for (int i = 0; i < fileCounter; i++) {
    free(files[i]);
  }
  free(files);

  Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());

  serializeJson(doc, client);
  return doc;
}
void printFileInDirectory(String dirName, EthernetClient client) {
  File myFile = SD.open(dirName);
  if (myFile) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain; charset=UTF-8");
    client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
    client.println("Keep-Alive: timeout=5, max=1000");
    client.println("X-Content-Type-Options: nosniff");
    client.println();
    client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
    Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      byte read = myFile.read();
      client.write(read);
      Serial.write(read);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    client.println("HTTP/1.1 404 Not Found");
    client.print("error opening file at directory");
    client.println(dirName);
  }
}
void printHeader(EthernetClient client, int code) {
  switch (code) {
    case 200:
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain; charset=UTF-8");
      client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
      client.println("Keep-Alive: timeout=5, max=1000");
      client.println("X-Content-Type-Options: nosniff");
      client.println();
      break;
    case 400:
      client.println("HTTP/1.1 400 Bad Request");
    case 416:
      client.println("HTTP/1.1 416 Requested Range Not Satisfiable");
      break;
    default:
      client.println("HTTP/1.1 404 Not Found");
  }
}
void handleConfig(char* body, EthernetClient client) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, body);
  if (error) {
    printHeader(client, 400);
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  } else {
    printHeader(client, 200);
    granularity = doc["granularity"];
    maxDataAge = doc["maxDataAge"];
    client.print("granularity: ");
    client.println(granularity);
    if (granularity) {
      EEPROM_writeDouble(granularityAddress, granularity);
    }
    client.print("maxDataAge: ");
    client.println(maxDataAge);
    if (maxDataAge) {
      EEPROM_writeDouble(maxDataAgeAddress, maxDataAge);
    }
  }
}
void handleData(String endpoint, EthernetClient client) {
  // data endpoint format: data/year/month/day/hour
  String directoryName;
  if (endpoint.length() >= 5) {
    directoryName = endpoint.substring(5);  // remove "/data/" prefix
  } else {
    directoryName = "/";
  }
  long startingLine = 0;
  long numLines = 0;
  long lastLines = 0;
  int lastIndex = -1;
  // check if there are url parameters
  int urlParameterIndex = directoryName.indexOf("?");
  if (urlParameterIndex > -1) {
    String urlParameter = directoryName.substring(urlParameterIndex);
    directoryName = directoryName.substring(0, urlParameterIndex);
    lastIndex = urlParameter.indexOf("last");
    if (lastIndex > -1) {  // print last lines
      lastLines = urlParameter.substring(lastIndex + 5).toInt();
      Serial.println("========");
      Serial.println(lastLines);
      Serial.println("========");
    } else {  // print specified lines
      int startIndex = urlParameter.indexOf("start");
      int numIndex = urlParameter.indexOf("num");
      startingLine = urlParameter.substring(startIndex + 6, numIndex - 1).toInt();
      numLines = urlParameter.substring(numIndex + 4).toInt();
      Serial.println("========");
      Serial.println(startingLine);
      Serial.println(numLines);
      Serial.println("========");
    }
  }
  char* pch;
  int slashes = -1;
  pch = strtok(endpoint.c_str(), "/");
  while (pch != NULL) {
    pch = strtok(NULL, "/");
    slashes += 1;
  }
  File dir = SD.open(directoryName);
  Serial.print("HOW MANY SLASHES: ");
  Serial.println(slashes);
  switch (slashes) {
    case 0:  // data: return years available
    case 1:  // year: return months available
    case 2:  // month: return days available
    case 3:  // day: return hours available
      Serial.println("START BUILDING JSON");
      printCurrentLevelDirectories(dir, client, slashes);
      break;
    case 4:
      String fileName = "/" + directoryName.substring(3, 5) + directoryName.substring(6, 8) + directoryName.substring(9, 11) + directoryName.substring(12, 14) + ".txt";
      directoryName.concat(fileName);
      if (urlParameterIndex > -1 && lastIndex > -1) {  // last lines: print lastLines in specified csv
        Serial.print("PRINTING LAST LINES FROM FILE: ");
        Serial.println(directoryName);
        printLastLines(directoryName, client, lastLines);
      } else if (urlParameterIndex > -1) {  // specified lines: print numLines from startingLine in specified csv
        Serial.print("PRINTING SPECIFIED LINES FROM FILE: ");
        Serial.println(directoryName);
        printSpecifiedLines(directoryName, client, startingLine, numLines);
      } else {  // hour: return csv of the specific hour
        Serial.print("PRINTING FILE: ");
        Serial.println(directoryName);
        printFileInDirectory(directoryName, client);
      }
      break;
    default:
      printHeader(client, 404);
      break;
  }
  dir.close();
}
void handleSeries(char* body, EthernetClient client) {
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, body);
  char goalRecordString[goalRecordLength];
  memset(goalRecordString, 0, goalRecordLength);
  if (error) {
    printHeader(client, 400);

    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  } else {
    printHeader(client, 200);

    int counter = 0;
    File seriesFile;
    JsonObject phObject = doc["pH"];
    JsonArray phValueJsonArray = phObject["value"];
    JsonArray phTimeJsonArray = phObject["time"];
    phInterval = phObject["interval"];
    phDelay = phObject["delay"];
    phSeriesSize = phValueJsonArray.size();
    long phValueArray[phSeriesSize];
    long phTimeArray[phSeriesSize];

    // validate pH series
    bool phSeriesIsValid = true;
    if (phValueJsonArray.size() != phTimeJsonArray.size()) {
      client.println("pH SERIES NOT SAME SIZE");
      phSeriesIsValid = false;
    }
    long phTimeLast = getLastJsonArrayValue(phTimeJsonArray);
    if (phTimeLast > phInterval) {
      client.println("phTimeLast GREATER THAN phInterval");
      phSeriesIsValid = false;
    }

    if (phSeriesIsValid) {
      phSeriesPointer = 0;
      EEPROM_writeDouble(phIntervalAddress, phInterval);
      EEPROM_writeDouble(phDelayAddress, phDelay);
      EEPROM_writeDouble(phSeriesSizeAddress, phSeriesSize);
      EEPROM_writeDouble(phSeriesPointerAddress, phSeriesPointer);
      pinMode(10, OUTPUT);
      digitalWrite(10, HIGH);
      SD.remove("pv.txt");
      seriesFile = SD.open("pv.txt", FILE_WRITE);  // pH values
      client.println("pH values: ");
      for (JsonVariant v : phValueJsonArray) {
        phValueArray[counter++] = v.as<long>();
        itoa(phValueArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < goalRecordLength;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();

      counter = 0;
      SD.remove("/pt.txt");
      seriesFile = SD.open("/pt.txt", FILE_WRITE);  // pH times
      client.println("pH times: ");
      memset(goalRecordString, 0, goalRecordLength);
      for (JsonVariant v : phTimeJsonArray) {
        phTimeArray[counter++] = v.as<long>();
        itoa(phTimeArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < goalRecordLength;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();
      client.println("pH interval: ");
      client.println(phInterval);
      client.println("pH delay: ");
      client.println(phDelay);
      client.print("ph series size: ");
      client.println(phSeriesSize);
    } else {
      client.println("pH SERIES INVALID");
    }

    JsonObject tempObject = doc["temp"];
    JsonArray tempValueJsonArray = tempObject["value"];
    JsonArray tempTimeJsonArray = tempObject["time"];
    tempInterval = tempObject["interval"];
    tempDelay = tempObject["delay"];
    tempSeriesSize = tempValueJsonArray.size();
    long tempInterval = tempObject["interval"];
    long tempDelay = tempObject["delay"];
    int tempSeriesSize = tempValueJsonArray.size();
    long tempValueArray[tempSeriesSize];
    long tempTimeArray[tempSeriesSize];

    // validate temp series
    bool tempSeriesIsValid = true;
    if (tempValueJsonArray.size() != tempTimeJsonArray.size()) {
      client.println("temp SERIES NOT SAME SIZE");
      tempSeriesIsValid = false;
    }
    long tempTimeLast = getLastJsonArrayValue(tempTimeJsonArray);
    if (tempTimeLast > tempInterval) {
      client.println("tempTimeLast GREATER THAN tempInterval");
      tempSeriesIsValid = false;
    }

    if (tempSeriesIsValid) {
      counter = 0;
      tempSeriesPointer = 0;
      EEPROM_writeDouble(tempIntervalAddress, tempInterval);
      EEPROM_writeDouble(tempDelayAddress, tempDelay);
      EEPROM_writeDouble(tempSeriesSizeAddress, tempSeriesSize);
      EEPROM_writeDouble(tempSeriesPointerAddress, tempSeriesPointer);
      SD.remove("/tv.txt");
      seriesFile = SD.open("/tv.txt", FILE_WRITE);  // temperature values
      client.println("temp values: ");
      memset(goalRecordString, 0, goalRecordLength);
      for (JsonVariant v : tempValueJsonArray) {
        tempValueArray[counter++] = v.as<long>();
        itoa(tempValueArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < goalRecordLength;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();

      counter = 0;
      SD.remove("/tt.txt");
      seriesFile = SD.open("/tt.txt", FILE_WRITE);  // temperature times
      client.println("temp values: ");
      memset(goalRecordString, 0, goalRecordLength);
      for (JsonVariant v : tempTimeJsonArray) {
        tempTimeArray[counter++] = v.as<long>();
        itoa(tempTimeArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < goalRecordLength;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();
      client.println("temp interval: ");
      client.println(tempInterval);
      client.println("temp delay: ");
      client.println(tempDelay);
      client.print("temp series size: ");
      client.println(tempSeriesSize);
    } else {
      client.println("temp SERIES INVALID");
    }
  }
}

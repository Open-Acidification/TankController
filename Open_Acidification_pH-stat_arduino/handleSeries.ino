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
void handleSeries(char* body, EthernetClient client) {
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, body);
  char goalRecordString[GOAL_RECORD_LENGTH];
  memset(goalRecordString, 0, GOAL_RECORD_LENGTH);
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
      client.println(F("pH SERIES NOT SAME SIZE"));
      phSeriesIsValid = false;
    }
    long phTimeLast = getLastJsonArrayValue(phTimeJsonArray);
    if (phTimeLast > phInterval) {
      client.println(F("phTimeLast GREATER THAN phInterval"));
      phSeriesIsValid = false;
    }

    if (phSeriesIsValid) {
      phSeriesPointer = 0;
      EEPROM_writeDouble(PH_INTERVAL_ADDRESS, phInterval);
      EEPROM_writeDouble(PH_DELAY_ADDRESS, phDelay);
      EEPROM_writeDouble(PH_SERIES_SIZE_ADDRESS, phSeriesSize);
      EEPROM_writeDouble(PH_SERIES_POINTER_ADDRESS, phSeriesPointer);
      pinMode(10, OUTPUT);
      digitalWrite(10, HIGH);
      SD.remove("pv.txt");
      seriesFile = SD.open("pv.txt", FILE_WRITE);  // pH values
      client.println(F("pH values: "));
      for (JsonVariant v : phValueJsonArray) {
        phValueArray[counter++] = v.as<long>();
        itoa(phValueArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < GOAL_RECORD_LENGTH;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();

      counter = 0;
      SD.remove("/pt.txt");
      seriesFile = SD.open("/pt.txt", FILE_WRITE);  // pH times
      client.println(F("pH times: "));
      memset(goalRecordString, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : phTimeJsonArray) {
        phTimeArray[counter++] = v.as<long>();
        itoa(phTimeArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < GOAL_RECORD_LENGTH;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();
      client.println(F("pH interval: "));
      client.println(phInterval);
      client.println(F("pH delay: "));
      client.println(phDelay);
      client.print(F("ph series size: "));
      client.println(phSeriesSize);
    } else {
      client.println(F("pH SERIES INVALID"));
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
      client.println(F("temp SERIES NOT SAME SIZE"));
      tempSeriesIsValid = false;
    }
    long tempTimeLast = getLastJsonArrayValue(tempTimeJsonArray);
    if (tempTimeLast > tempInterval) {
      client.println(F("tempTimeLast GREATER THAN tempInterval"));
      tempSeriesIsValid = false;
    }

    if (tempSeriesIsValid) {
      counter = 0;
      tempSeriesPointer = 0;
      EEPROM_writeDouble(TEMP_INTERVAL_ADDRESS, tempInterval);
      EEPROM_writeDouble(TEMP_DELAY_ADDRESS, tempDelay);
      EEPROM_writeDouble(TEMP_SERIES_SIZE_ADDRESS, tempSeriesSize);
      EEPROM_writeDouble(TEMP_SERIES_POINTER_ADDRESS, tempSeriesPointer);
      SD.remove("/tv.txt");
      seriesFile = SD.open("/tv.txt", FILE_WRITE);  // temperature values
      client.println(F("temp values: "));
      memset(goalRecordString, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : tempValueJsonArray) {
        tempValueArray[counter++] = v.as<long>();
        itoa(tempValueArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < GOAL_RECORD_LENGTH;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();

      counter = 0;
      SD.remove("/tt.txt");
      seriesFile = SD.open("/tt.txt", FILE_WRITE);  // temperature times
      client.println(F("temp values: "));
      memset(goalRecordString, 0, GOAL_RECORD_LENGTH);
      for (JsonVariant v : tempTimeJsonArray) {
        tempTimeArray[counter++] = v.as<long>();
        itoa(tempTimeArray[counter - 1], goalRecordString, 10);
        for (strlen(goalRecordString); strlen(goalRecordString) < GOAL_RECORD_LENGTH;) {
          strcat(goalRecordString, " ");
        }
        client.println(goalRecordString);
        seriesFile.println(goalRecordString);
      }
      seriesFile.close();
      client.println(F("temp interval: "));
      client.println(tempInterval);
      client.println(F("temp delay: "));
      client.println(tempDelay);
      client.print(F("temp series size: "));
      client.println(tempSeriesSize);
    } else {
      client.println(F("temp SERIES INVALID"));
    }
  }
}
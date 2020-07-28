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
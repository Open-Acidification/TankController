void HandleGoal(EthernetClient client) {
  PrintHeader(client, 200);

  StaticJsonDocument<200> goalDoc;
  JsonObject goalObj = goalDoc.to<JsonObject>();

  StaticJsonDocument<100> phValuesDoc;
  JsonArray phValues = phValuesDoc.to<JsonArray>();
  for (int i = 0; i < phSeriesSize; i++) {
    long phValueCurrent = ReadLineFromSd("pv.txt", i, GOAL_RECORD_LENGTH);
    phValues.add(phValueCurrent);
  }
  goalObj["phValues"] = phValues;

  StaticJsonDocument<100> phTimesDoc;
  JsonArray phTimes = phTimesDoc.to<JsonArray>();
  for (int i = 0; i < phSeriesSize; i++) {
    long phTimeCurrent = ReadLineFromSd("pt.txt", i, GOAL_RECORD_LENGTH);
    phTimes.add(phTimeCurrent);
  }
  goalObj["phTimes"] = phTimes;
  goalObj["phInterval"] = phInterval;
  goalObj["phDelay"] = phDelay;

  StaticJsonDocument<100> tempValuesDoc;
  JsonArray tempValues = tempValuesDoc.to<JsonArray>();
  for (int i = 0; i < tempSeriesSize; i++) {
    long tempValueCurrent = ReadLineFromSd("tv.txt", i, GOAL_RECORD_LENGTH);
    tempValues.add(tempValueCurrent);
  }
  goalObj["tempValues"] = tempValues;

  StaticJsonDocument<100> tempTimesDoc;
  JsonArray tempTimes = tempTimesDoc.to<JsonArray>();
  for (int i = 0; i < tempSeriesSize; i++) {
    long tempTimeCurrent = ReadLineFromSd("tt.txt", i, GOAL_RECORD_LENGTH);
    tempTimes.add(tempTimeCurrent);
  }
  goalObj["tempTimes"] = tempTimes;
  goalObj["tempInterval"] = tempInterval;
  goalObj["tempDelay"] = tempDelay;

  serializeJson(goalObj, client);
}
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
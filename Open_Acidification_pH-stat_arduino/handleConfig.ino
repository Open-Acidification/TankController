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
    client.print(F("granularity: "));
    client.println(granularity);
    if (granularity) {
      EEPROM_writeDouble(GRANULARITY_ADDRESS, granularity);
    }
    client.print(F("maxDataAge: "));
    client.println(maxDataAge);
    if (maxDataAge) {
      EEPROM_writeDouble(MAX_DATA_AGE_ADDRESS, maxDataAge);
    }
  }
}
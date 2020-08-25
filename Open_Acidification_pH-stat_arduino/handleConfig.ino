void HandleConfig(char* body, EthernetClient client) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, body);
  if (error) {
    PrintHeader(client, 400);
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  } else {
    PrintHeader(client, 200);
    granularity = doc["granularity"];
    maxDataAge = doc["maxDataAge"];
    client.print(F("granularity: "));
    client.println(granularity);
    if (granularity) {
      EEPROM_WriteDouble(GRANULARITY_ADDRESS, granularity);
    }
    client.print(F("maxDataAge: "));
    client.println(maxDataAge);
    if (maxDataAge) {
      EEPROM_WriteDouble(MAX_DATA_AGE_ADDRESS, maxDataAge);
    }
  }
}
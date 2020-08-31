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
    max_data_age = doc["max_data_age"];
    client.print(F("granularity: "));
    client.println(granularity);
    if (granularity) {
      EepromWriteDouble(GRANULARITY_ADDRESS, granularity);
    }
    client.print(F("max_data_age: "));
    client.println(max_data_age);
    if (max_data_age) {
      EepromWriteDouble(MAX_DATA_AGE_ADDRESS, max_data_age);
    }
  }
}

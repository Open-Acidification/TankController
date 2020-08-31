void HandleDevice(String endpoint, EthernetClient client) {
  // send all device info
  PrintHeader(client, 200);
  StaticJsonDocument<200> doc;
  JsonObject object = doc.to<JsonObject>();
  object["granularity"] = granularity;
  object["max_data_age"] = max_data_age;
  object["mac_str"] = mac_str;
  object["tank_id"] = tank_id;
  serializeJson(doc, client);
}

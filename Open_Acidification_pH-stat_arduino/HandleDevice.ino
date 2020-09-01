void HandleDevice(String endpoint, EthernetClient client) {
  // send all device info
  PrintHeader(client, 200);
  StaticJsonDocument<200> doc;
  JsonObject object = doc.to<JsonObject>();
  object["granularity"] = granularity;
  object["maxDataAge"] = max_data_age;
  object["macstr"] = mac_str;
  object["tankid"] = tank_id;
  serializeJson(doc, client);
}

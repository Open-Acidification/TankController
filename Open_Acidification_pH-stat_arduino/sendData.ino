// ************************************************
// Sending Pushingbox request
// ************************************************
void sendData() {
  Serial.println(F("connected"));
  Serial.println(data);
  client.println(data);
  client.println(F("Host: api.pushingbox.com"));
  client.println(F("Connection: close"));
  client.println();
}
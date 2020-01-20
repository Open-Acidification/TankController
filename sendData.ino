// ************************************************
// Sending Pushingbox request
// ************************************************
void sendData() {
  Serial.println(F("connected"));
  Serial.println(data);
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}
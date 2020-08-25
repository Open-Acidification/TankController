void HandleMisc(EthernetClient client) {
  PrintHeader(client, 200);
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print(F("analog input "));
    client.print(analogChannel);
    client.print(F(" is "));
    client.println(sensorReading);
  }
}
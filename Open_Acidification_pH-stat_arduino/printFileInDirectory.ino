void printFileInDirectory(String dirName, EthernetClient client) {
  File myFile = SD.open(dirName);
  if (myFile) {
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/plain; charset=UTF-8"));
    client.println(F("Connection: keep-alive"));  // the connection will be closed after completion of the response
    client.println(F("Keep-Alive: timeout=5, max=1000"));
    client.println(F("X-Content-Type-Options: nosniff"));
    client.println();
    client.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
    Serial.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      byte read = myFile.read();
      client.write(read);
      Serial.write(read);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    client.println(F("HTTP/1.1 404 Not Found"));
    client.print(F("error opening file at directory"));
    client.println(dirName);
  }
}
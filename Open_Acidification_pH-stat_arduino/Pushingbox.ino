// ************************************************
// Packing data into Pushingbox request
// ************************************************
void packData() {
  data += "";
  data += "GET /pushingbox?devid=";  // GET request query to pushingbox API
  data += DevID;                     // GET request query to pushingbox API
  data += "&tankid=";                // GET request query to pushingbox API
  data += tankid;
  data += "&tempData=";  // GET request query to pushingbox API
  data += temp;
  data += "&pHdata=";  // GET request query to pushingbox API
  data += String(pHDisplay, 3);
  data += " HTTP/1.1";
}
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

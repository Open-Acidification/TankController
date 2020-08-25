// ************************************************
// Packing data into Pushingbox request
// ************************************************
void PackData() {
  data += "";
  data += "GET /pushingbox?devid=";  // GET request query to pushingbox API
  data += DEV_ID;                    // GET request query to pushingbox API
  data += "&tankid=";                // GET request query to pushingbox API
  data += tankid;
  data += "&tempData=";  // GET request query to pushingbox API
  data += temp;
  data += "&pHdata=";  // GET request query to pushingbox API
  data += String(pHDisplay, 3);
  data += " HTTP/1.1";
}
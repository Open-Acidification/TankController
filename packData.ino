// ************************************************
// Packing data into Pushingbox request
// ************************************************
void packData() {
  data += "";
  data += "GET /pushingbox?devid="; //GET request query to pushingbox API
  data += DevID; //GET request query to pushingbox API
  data += "&tankid="; //GET request query to pushingbox API
  data += tankid;
  data += "&tempData="; //GET request query to pushingbox API
  data += temp;
  data += "&pHdata="; //GET request query to pushingbox API
  data += String(pHDisplay, 3);
  data += " HTTP/1.1";
}
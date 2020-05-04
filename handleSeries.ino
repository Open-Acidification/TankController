void handleSeries(char* body, EthernetClient client) {
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();
	client.println("<!DOCTYPE HTML>");
	client.println("<html>");
	client.println("HANDLING SERIES <br>");
	
	StaticJsonDocument<200> doc;
	DeserializationError error = deserializeJson(doc, body);	
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
    } else {
		JsonArray timeJsonArray = doc["time"];
		JsonArray phJsonArray = doc["pH"];
		JsonArray tempJsonArray = doc["temp"];
		int seriesSize = timeJsonArray.size();
		int timeArray[seriesSize];
		int phArray[seriesSize];
		int tempArray[seriesSize];
		int counter = 0;
		client.println("time: ");
		for (JsonVariant v : timeJsonArray) {
			timeArray[counter++] = v.as<int>();
			Serial.println(timeArray[counter-1]);
			client.println(timeArray[counter-1]);
		}
		counter = 0;
		client.println("pH: ");
		for (JsonVariant v : phJsonArray) {
			phArray[counter++] = v.as<int>();
			Serial.println(phArray[counter-1]);
			client.println(phArray[counter-1]);
		}
		counter = 0;
		client.println("temp: ");
		for (JsonVariant v : tempJsonArray) {
			tempArray[counter++] = v.as<int>();
			Serial.println(tempArray[counter-1]);
			client.println(tempArray[counter-1]);
		}
		client.print("Series size: ");
		client.println(seriesSize);
	}

	client.println("</html>");
}
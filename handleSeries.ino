void handleSeries(char* body, EthernetClient client) {
	
	StaticJsonDocument<1000> doc;
	DeserializationError error = deserializeJson(doc, body);	
	if (error) {
		client.println("HTTP/1.1 400 Not Found");
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
    } else {
		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/html");
		client.println("Connection: close");  // the connection will be closed after completion of the response
		client.println("Refresh: 5");  // refresh the page automatically every 5 sec
		client.println();
		client.println("<!DOCTYPE HTML>");
		client.println("<html>");
		client.println("HANDLING SERIES <br>");

		int counter = 0;
		File seriesFile;
		JsonObject phObject = doc["pH"];
		JsonArray phValueJsonArray = phObject["value"];
		JsonArray phTimeJsonArray = phObject["time"];
		phInterval = phObject["interval"];
		phDelay = phObject["delay"];
		phSeriesSize = phValueJsonArray.size();
		phSeriesPointer = 0;
		int phValueArray[phSeriesSize];
		int phTimeArray[phSeriesSize];
		pinMode(10, OUTPUT);
		digitalWrite(10, HIGH);
		SD.remove("pv.txt");
		seriesFile = SD.open("pv.txt", FILE_WRITE); // pH values
		client.println("pH values: ");
		char goalRecordString[goalRecordLength];
		memset(goalRecordString, 0, goalRecordLength);
		for (JsonVariant v : phValueJsonArray) {
			phValueArray[counter++] = v.as<int>();
			itoa(phValueArray[counter-1], goalRecordString, 10);
			for (strlen(goalRecordString); strlen(goalRecordString) < goalRecordLength;) {
				strcat(goalRecordString, " ");
			}
			client.println(goalRecordString);
			seriesFile.println(goalRecordString);
		}
		seriesFile.close();
		counter = 0;
		SD.remove("/pt.txt");
		seriesFile = SD.open("/pt.txt", FILE_WRITE); // pH times
		client.println("pH times: ");
		for (JsonVariant v : phTimeJsonArray) {
			phTimeArray[counter++] = v.as<int>();
			Serial.println(phTimeArray[counter-1]);
			client.println(phTimeArray[counter-1]);
			seriesFile.println(phTimeArray[counter-1]);
		}
		seriesFile.close();
		client.println("pH interval: ");
		client.println(phInterval);
		client.println("pH delay: ");
		client.println(phDelay);
		client.print("ph series size: ");
		client.println(phSeriesSize);

		counter = 0;
		JsonObject tempObject = doc["temp"];
		JsonArray tempValueJsonArray = tempObject["value"];
		JsonArray tempTimeJsonArray = tempObject["time"];
		int tempInterval = tempObject["interval"];
		int tempDelay = tempObject["delay"];
		int tempSeriesSize = tempValueJsonArray.size();
		int tempValueArray[tempSeriesSize];
		int tempTimeArray[tempSeriesSize];
		SD.remove("/tv.txt");
		seriesFile = SD.open("/tv.txt", FILE_WRITE); // temperature values
		client.println("temp values: ");
		for (JsonVariant v : tempValueJsonArray) {
			tempValueArray[counter++] = v.as<int>();
			Serial.println(tempValueArray[counter-1]);
			client.println(tempValueArray[counter-1]);
			seriesFile.println(tempValueArray[counter-1]);
		}
		seriesFile.close();
		counter = 0;
		SD.remove("/tt.txt");
		seriesFile = SD.open("/tt.txt", FILE_WRITE); // temperature times
		client.println("temp values: ");
		for (JsonVariant v : tempTimeJsonArray) {
			tempTimeArray[counter++] = v.as<int>();
			Serial.println(tempTimeArray[counter-1]);
			client.println(tempTimeArray[counter-1]);
			seriesFile.println(tempTimeArray[counter-1]);
		}
		seriesFile.close();
		client.println("temp interval: ");
		client.println(tempInterval);
		client.println("temp delay: ");
		client.println(tempDelay);
		client.print("temp series size: ");
		client.println(tempSeriesSize);
	}

	client.println("</html>");
}
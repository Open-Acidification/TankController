void handleSeries(char* body, EthernetClient client) {
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();
	client.println("<!DOCTYPE HTML>");
	client.println("<html>");
	client.println("HANDLING CONFIG <br>");
	
	StaticJsonDocument<200> doc;
	DeserializationError error = deserializeJson(doc, body);	
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
    } else {
		int granularity;
		int maxDataAge;
		granularity = doc["granularity"];
		maxDataAge = doc["maxDataAge"];
		client.print("granularity: ");
		client.println(granularity);
		client.print("maxDataAge: ");
		client.println(maxDataAge);
    }

	client.println("</html>");
}
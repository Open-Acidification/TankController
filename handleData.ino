void handleData(String endpoint, EthernetClient client) {	  
	client.println("HTTP/1.1 200 OK");
	// data endpoint format: data/year/month/day/hour
	String directoryName;
	if (endpoint.length() >= 5) {
		directoryName = endpoint.substring(5); // remove "/data/" prefix
	} else {
		directoryName = "/";
	}
	char* pch;
	int slashes = -1;
	pch = strtok(endpoint.c_str(), "/");
	while (pch != NULL)
	{
		pch = strtok(NULL, "/");
		slashes += 1;
	}
	File dir = SD.open(directoryName);
	Serial.print("HOW MANY SLASHES: ");
	Serial.println(slashes);
	switch (slashes) {
		case 0: // data: return years available
		case 1: // year: return months available
		case 2: // month: return days available
		case 3: // day: return hours available
			client.println("X-Content-Type-Options: nosniff");
			client.println("Content-Type: text/plain; charset=UTF-8");
			client.println("Connection: close");  // the connection will be closed after completion of the response
			client.println();
			Serial.println("START BUILDING JSON");
			StaticJsonDocument<512> doc = printCurrentLevelDirectories(dir, client, slashes);
			serializeJson(doc, client);
			break;
		case 4: // hour: return csv of the specific hour
			client.println("Content-Type: text/plain; charset=UTF-8");
			client.println("Connection: close");  // the connection will be closed after completion of the response
			client.println();
			printFileInDirectory(directoryName, client);
			break;
		default:
			client.println("INVALID FORMAT");
			break;
	}
	dir.close();
}
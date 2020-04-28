void handleData(String endpoint, EthernetClient client) {	  
	client.println("HTTP/1.1 200 OK");
	client.println("X-Content-Type-Options: nosniff");
	client.println("Content-Type: text/plain; charset=UTF-8");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println();
	// data endpoint format: data/year/month/day/hour
	File myFile;
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
	switch (slashes) {
		case 0: // data: return years available
		case 1: // year: return months available
		case 2: // month: return days available
		case 3: // day: return hours available
			StaticJsonDocument<512> doc = printCurrentLevelDirectories(dir, client, slashes);
			serializeJson(doc, client);
			break;
		case 4: // hour: return csv of the specific hour
			printFileInDirectory(directoryName, client);
			break;
		default:
			client.println("INVALID FORMAT");
			break;
	}
	dir.close();
}
void handleData(String endpoint, EthernetClient client) {	  
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/plain; charset=UTF-8");
	client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
	client.println("Keep-Alive: timeout=5, max=1000");
	// data endpoint format: data/year/month/day/hour
	String directoryName;
	if (endpoint.length() >= 5) {
		directoryName = endpoint.substring(5); // remove "/data/" prefix
	} else {
		directoryName = "/";
	}
	long startingLine = 0;
	long numLines = 0;
	// check if there are url parameters 
	int urlParameterIndex = directoryName.indexOf("?");
	if (urlParameterIndex > -1) {
		String urlParameter = directoryName.substring(urlParameterIndex);
		directoryName = directoryName.substring(0, urlParameterIndex);
		int startIndex = urlParameter.indexOf("start");
		int numIndex = urlParameter.indexOf("num");
		startingLine = urlParameter.substring(startIndex+6, numIndex-1).toInt();
		numLines = urlParameter.substring(numIndex+4).toInt();
		Serial.println("========");
		Serial.println(startingLine);
		Serial.println(numLines);
		Serial.println("========");
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
			client.println();
			Serial.println("START BUILDING JSON");
			printCurrentLevelDirectories(dir, client, slashes);
			break;
		case 4: 
			String fileName = "/" + directoryName.substring(3,5) + directoryName.substring(6,8) + directoryName.substring(9,11) + directoryName.substring(12,14) + ".txt";  
			directoryName.concat(fileName);
			if (urlParameterIndex > -1) { // specified lines: print numLines from startingLine in specified csv
				client.println("X-Content-Type-Options: nosniff");
				client.println();
				Serial.print("PRINTING SPECIFIED LINES FROM FILE: ");
				Serial.println(directoryName);
				printSpecifiedLines(directoryName, client, startingLine, numLines);
			} else { // hour: return csv of the specific hour
				client.println("X-Content-Type-Options: nosniff");
				client.println();
				Serial.print("PRINTING FILE: ");
				Serial.println(directoryName);
				printFileInDirectory(directoryName, client);
			}
			break;
		default:
			client.println("INVALID FORMAT");
			break;
	}
	dir.close();
}
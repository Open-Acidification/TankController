void handleData(String endpoint, EthernetClient client) {	  
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/plain");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println();
	client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
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
	switch (slashes) {
		case 0: // data: return years available
			printCurrentLevelDirectories(directoryName, client);
			break;
		case 1: // year: return months available
			printCurrentLevelDirectories(directoryName, client);
			break;
		case 2: // month: return days available
			printCurrentLevelDirectories(directoryName, client);
			break;
		case 3: // day: return hours available
			printCurrentLevelDirectories(directoryName, client);
			break;
		case 4: // hour: return csv of the specific hour
			printFileInDirectory(directoryName, client);
			break;
		default:
			client.println("INVALID FORMAT");
			break;
	}
}
void handleGoal(String endpoint, EthernetClient client) {	
	long startingByte = 2 * (goalRecordLength+2);
	File pvFile = SD.open("pv.txt");
	if (pvFile.seek(startingByte)) {
		Serial.println("SUCCESSFUL GOAL SEEK");
		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/plain; charset=UTF-8");
		client.println("Connection: keep-alive");  // the connection will be closed after completion of the response
		client.println("Keep-Alive: timeout=5, max=1000");
		client.println("X-Content-Type-Options: nosniff");
		client.println();
		client.println("ph values: ");
	} else {
		Serial.println("FAILED GOAL SEEK");
		client.println("HTTP/1.1 416 Requested Range Not Satisfiable");
		// close the file:
		pvFile.close();
		return;
	}
	// fill up buffer
	char line[10];
	memset(line, 0, 10);
	for (int i = 0; i < 10 && pvFile.available(); i++) {
		byte read = pvFile.read();
		if ((char)read == '\n') {
			break;
		}
		line[i] = read;
	}
	// write buffer to client
	client.write(line, strlen(line));
	Serial.write(line, strlen(line));
	// close the file:
	pvFile.close();
}
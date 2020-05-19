void handleGoal(String endpoint, EthernetClient client) {	
	long startingByte = 2 * (goalRecordLength+2);
	File pvFile = SD.open("pv.txt");
	if (pvFile.seek(startingByte)) {
		printHeader(client, 200);
		client.println("ph values: ");
	} else {
		Serial.println("FAILED GOAL SEEK");
		printHeader(client, 416);
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
void updateGoals() {
	long now = millis();

	Serial.print("UPDATING GOALS AT MILLIS: ");
	Serial.println(now);
	Serial.print("INTERVAL: ");
	Serial.println(phInterval);

	long phTime = now % phInterval;

	Serial.print("CURRENT TIME: ");
	Serial.println(phTime);

	long startingByte = phSeriesPointer * (goalRecordLength+2);
	long phTimeCurrent;

	File pvFile = SD.open("pv.txt");
	if (pvFile.seek(startingByte)) {
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
		line[i] = '\0';
		phTimeCurrent = atoi(line);
		Serial.print("phTimeCurrent: ");
		Serial.println(phTimeCurrent + 1);
	} else {
		Serial.println("FAILED UPDATE GOAL SEEK");
	}
	// close the file:
	pvFile.close();
}
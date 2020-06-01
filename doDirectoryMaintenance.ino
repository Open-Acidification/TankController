void doDirectoryMaintenance() { // TODO: investigate bug with un-deletable files
	char nameBuffer[20];
	char deleteBuffer[20];
	memset(nameBuffer, 0, 20);
	strcat(nameBuffer, "/");
	DateTime now = rtc.now();
	int day = now.day();

	int year = now.year();
	Serial.println("CURRENT YEAR IS: ");
	Serial.println(year);
	char yearBuffer[4];
	itoa(year, yearBuffer, 10);
	strcat(nameBuffer, yearBuffer);
	Serial.println("NAME BUFFER IS: ");
	Serial.println(nameBuffer);

	int month = now.month();
	Serial.println("CURRENT MONTH IS: ");
	Serial.println(month);
	strcat(nameBuffer, "/");
	if (month < 10) {
		strcat(nameBuffer, "0");
	}
	char monthBuffer[12];
	itoa(month, monthBuffer, 10);
	strcat(nameBuffer, monthBuffer);
	Serial.println("NAME BUFFER IS: ");
	Serial.println(nameBuffer);
	File tempDir = SD.open(nameBuffer);

	// for relevant directories
	while (true) {
		File entry = tempDir.openNextFile();
		if (entry) {
			Serial.println("ENTRY NAME IS: ");
			Serial.println(entry.name());
			// calculate elapsed time
			int dirAgeInDays = day - atoi(entry.name());
			Serial.println("ENTRY AGE IS: ");
			Serial.println(dirAgeInDays);
			// remove if directory is too old
			if (dirAgeInDays > maxDataAge) {
				Serial.println("ENTRY IS TOO OLD");
				memset(deleteBuffer, 0, 20);
				strcat(deleteBuffer, nameBuffer);
				strcat(deleteBuffer, "/");
				strcat(deleteBuffer, entry.name());
				recursiveRm(entry, deleteBuffer);
			}
		} else {
			break;
		}
	}
	Serial.println("maxDataAge IS: ");
	Serial.println(maxDataAge);
}
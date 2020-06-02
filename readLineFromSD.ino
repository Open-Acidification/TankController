long readLineFromSD(char* fileName, int line, int lineLength) {
	long startingByte = line * (lineLength+2);
	long lineValue;

	File tempFile = SD.open(fileName);
	if (tempFile.seek(startingByte)) {
		// fill up buffer
		char line[lineLength];
		memset(line, 0, lineLength);
		int i;
		for (i = 0; i < lineLength && tempFile.available(); i++) {
			byte read = tempFile.read();
			if ((char)read == '\n') {
				break;
			}
			line[i] = read;
		}
		line[i] = '\0';
		lineValue = atoi(line);
	} else {
		Serial.println("FAILED READ LINE SEEK");
	}

	// close the file:
	tempFile.close();

	return lineValue;
}
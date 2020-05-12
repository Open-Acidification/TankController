void printSpecifiedLines(String dirName, EthernetClient client, int startingLine, int numLines) {	
	File myFile = SD.open(dirName);
	if (myFile) {
		Serial.print("STARTINGLINE: ");
		Serial.println(startingLine);
		Serial.print("NUMLINES: ");
		Serial.println(numLines);
		client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
		Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
		// read from the file until there's nothing else in it:
		int printedLines = 0;
		while (printedLines++ < numLines) {
			// skip to line
			int linesSkipped = 0;
			while (myFile.available() && linesSkipped < startingLine) {
				byte read = myFile.read();
				if ((char)read == '\n') {
					linesSkipped++;
				}
			}
			// fill up buffer
			char line[100];
			memset(line, 0, 100);
			for (int i = 0; i < 100 && myFile.available(); i++) {
				byte read = myFile.read();
				if ((char)read == '\n') {
					break;
				}
				line[i] = read;
			}
			// write buffer to client
			client.write(line, strlen(line));
			Serial.write(line, strlen(line));
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		client.print("error opening file at directory");
		client.println(dirName);
	}
}
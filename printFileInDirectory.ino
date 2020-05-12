void printFileInDirectory(String dirName, EthernetClient client) {	
	File myFile = SD.open(dirName);
	if (myFile) {
		client.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
		Serial.println("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			byte read = myFile.read();
			client.write(read);
			Serial.write(read);
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		client.print("error opening file at directory");
		client.println(dirName);
	}
}
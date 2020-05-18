void updateGoals() {
	long now = millis();

	Serial.print("UPDATING GOALS AT MILLIS: ");
	Serial.println(now);
	Serial.print("INTERVAL: ");
	Serial.println(phInterval);

	long phTime = now % phInterval;

	Serial.print("CURRENT TIME: ");
	Serial.println(phTime);
	
	long phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);

	Serial.print("phTimeCurrent: ");
	Serial.println(phTimeCurrent);
}
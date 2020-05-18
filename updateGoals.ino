void updateGoals() {
	long now = millis();

	Serial.print("UPDATING GOALS AT MILLIS: ");
	Serial.println(now);
	Serial.print("phInterval: ");
	Serial.println(phInterval);

	long phTime = now % phInterval;

	Serial.print("phTime: ");
	Serial.println(phTime);
	
	long phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
	long phTimeNext = readLineFromSD("pt.txt", phSeriesPointer+1 >= phSeriesSize ? 0 : phSeriesPointer+1, goalRecordLength);

	Serial.print("phTimeCurrent: ");
	Serial.println(phTimeCurrent);
	Serial.print("phTimeNext: ");
	Serial.println(phTimeNext);

	// we want to make sure phTime is always between phTimeCurrent and phTimeNext
	if (phSeriesSize) {
		while ((phTimeNext > phTimeCurrent) && (phTimeNext < phTime)) {
			phSeriesPointer++;
			if (phSeriesPointer >= phSeriesSize) {
				phSeriesPointer = 0;
			}
			phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
			phTimeNext = readLineFromSD("pt.txt", phSeriesPointer+1 >= phSeriesSize ? 0 : phSeriesPointer+1, goalRecordLength);
			Serial.print("phTimeCurrent: ");
			Serial.println(phTimeCurrent);
			Serial.print("phTimeNext: ");
			Serial.println(phTimeNext);
			Serial.print("phSeriesPointer: ");
			Serial.println(phSeriesPointer);
		}
		if ((phTimeNext < phTimeCurrent) && (phTimeCurrent > phTime)) {
			phSeriesPointer = 0;
			phTimeCurrent = readLineFromSD("pt.txt", phSeriesPointer, goalRecordLength);
			phTimeNext = readLineFromSD("pt.txt", phSeriesPointer+1 >= phSeriesSize ? 0 : phSeriesPointer+1, goalRecordLength);
			Serial.print("phTimeCurrent: ");
			Serial.println(phTimeCurrent);
			Serial.print("phTimeNext: ");
			Serial.println(phTimeNext);
			Serial.print("phSeriesPointer: ");
			Serial.println(phSeriesPointer);
		}
	}

	// interpolate ph goal
	long phValueCurrent = readLineFromSD("pv.txt", phSeriesPointer, goalRecordLength);
	long phValueNext;
	if (phSeriesPointer+1 >= phSeriesSize) {
		phValueNext = readLineFromSD("pv.txt", 0, goalRecordLength);
		phTimeNext = phInterval;
	} else {
		phValueNext = readLineFromSD("pv.txt", phSeriesPointer+1, goalRecordLength);
	}
	phset = phValueCurrent + (phValueNext - phValueCurrent) * (phTime - phTimeCurrent) / (phTimeNext - phTimeCurrent);
	Serial.print("phValueCurrent: ");
	Serial.println(phValueCurrent);
	Serial.print("phValueNext: ");
	Serial.println(phValueNext);
	Serial.print("phset: ");
	Serial.println(phset);

	// now do the same for temp
	Serial.print("tempInterval: ");
	Serial.println(tempInterval);

	long tempTime = now % tempInterval;

	Serial.print("tempTime: ");
	Serial.println(tempTime);
	
	long tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
	long tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer+1 >= tempSeriesSize ? 0 : tempSeriesPointer+1, goalRecordLength);

	Serial.print("tempTimeCurrent: ");
	Serial.println(tempTimeCurrent);
	Serial.print("tempTimeNext: ");
	Serial.println(tempTimeNext);

	// we want to make sure tempTime is always between tempTimeCurrent and tempTimeNext
	if (tempSeriesSize) {
		while ((tempTimeNext > tempTimeCurrent) && (tempTimeNext < tempTime)) {
			tempSeriesPointer++;
			if (tempSeriesPointer >= tempSeriesSize) {
				tempSeriesPointer = 0;
			}
			tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
			tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer+1 >= tempSeriesSize ? 0 : tempSeriesPointer+1, goalRecordLength);
			Serial.print("tempTimeCurrent: ");
			Serial.println(tempTimeCurrent);
			Serial.print("tempTimeNext: ");
			Serial.println(tempTimeNext);
			Serial.print("tempSeriesPointer: ");
			Serial.println(tempSeriesPointer);
		}
		if ((tempTimeNext < tempTimeCurrent) && (tempTimeCurrent > tempTime)) {
			tempSeriesPointer = 0;
			tempTimeCurrent = readLineFromSD("tt.txt", tempSeriesPointer, goalRecordLength);
			tempTimeNext = readLineFromSD("tt.txt", tempSeriesPointer+1 >= tempSeriesSize ? 0 : tempSeriesPointer+1, goalRecordLength);
			Serial.print("tempTimeCurrent: ");
			Serial.println(tempTimeCurrent);
			Serial.print("tempTimeNext: ");
			Serial.println(tempTimeNext);
			Serial.print("tempSeriesPointer: ");
			Serial.println(tempSeriesPointer);
		}
	}

	// interpolate temp goal
	long tempValueCurrent = readLineFromSD("tv.txt", tempSeriesPointer, goalRecordLength);
	long tempValueNext;
	if (tempSeriesPointer+1 >= tempSeriesSize) {
		tempValueNext = readLineFromSD("tv.txt", 0, goalRecordLength);
		tempTimeNext = tempInterval;
	} else {
		tempValueNext = readLineFromSD("tv.txt", tempSeriesPointer+1, goalRecordLength);
	}
	tempset = tempValueCurrent + (tempValueNext - tempValueCurrent) * (tempTime - tempTimeCurrent) / (tempTimeNext - tempTimeCurrent);
	Serial.print("tempValueCurrent: ");
	Serial.println(tempValueCurrent);
	Serial.print("tempValueNext: ");
	Serial.println(tempValueNext);
	Serial.print("tempset: ");
	Serial.println(tempset);
}
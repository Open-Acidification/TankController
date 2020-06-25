// ************************************************
// Change Kp value
// ************************************************

void Change_Kp() {
  double KpTemp;
  KpTemp = Kp;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kp:"));
  lcd.setCursor(0, 1);
  lcd.print(KpTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KpTemp = KpTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }
    if (Key == '4') {
      KpTemp = KpTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '2') {
      KpTemp = KpTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '5') {
      KpTemp = KpTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '3') {
      KpTemp = KpTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '6') {
      KpTemp = KpTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'A') {
      KpTemp = KpTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == 'B') {
      KpTemp = KpTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '7') {
      KpTemp = KpTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '*') {
      KpTemp = KpTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '8') {
      KpTemp = KpTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }

    if (Key == '0') {
      KpTemp = KpTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kp:"));
      lcd.setCursor(0, 1);
      lcd.print(KpTemp, 2);
      Serial.print(F("New Kp: "));
      Serial.println(KpTemp);
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kp:"));
  lcd.print(KpTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kp = KpTemp;
      EEPROM_writeDouble(KpAddress, Kp);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
// ************************************************
// Change Ki value
// ************************************************

void Change_Ki() {
  Serial.println(F("starting Ki adjustment routine"));
  double KiTemp;
  KiTemp = Ki;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ki:"));
  lcd.setCursor(0, 1);
  lcd.print(KiTemp);
  Key = NO_KEY;
  Serial.print(F("Key: "));
  Serial.println(Key);
  while (Key != '#') {
    Serial.println(F("In the while loop to wait for key presses"));
    Key = customKeypad.waitForKey();
    Serial.print(F("Key: "));
    Serial.println(Key);
    if (Key == '1') {
      Serial.println(F("in the 1 if statement"));
      KiTemp = KiTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
    if (Key == '4') {
      Serial.println(F("in the 4 if statement"));
      KiTemp = KiTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '2') {
      KiTemp = KiTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '5') {
      KiTemp = KiTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '3') {
      KiTemp = KiTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '6') {
      KiTemp = KiTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'A') {
      KiTemp = KiTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == 'B') {
      KiTemp = KiTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '7') {
      KiTemp = KiTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '*') {
      KiTemp = KiTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '8') {
      KiTemp = KiTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }

    if (Key == '0') {
      KiTemp = KiTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Ki:"));
      lcd.setCursor(0, 1);
      lcd.print(KiTemp, 2);
      Serial.print(F("New Ki: "));
      Serial.println(KiTemp);
    }
  }
  Serial.println(F("out of the Ki adjusting while loop"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Ki:"));
  lcd.print(KiTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Ki = KiTemp;
      EEPROM_writeDouble(KiAddress, Ki);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}

// ************************************************
// Change Kd value
// ************************************************

void Change_Kd() {
  double KdTemp;
  KdTemp = Kd;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Kd:"));
  lcd.setCursor(0, 1);
  lcd.print(KdTemp);
  Key = NO_KEY;
  while (Key != '#') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      KdTemp = KdTemp + 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }
    if (Key == '4') {
      KdTemp = KdTemp - 10000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '2') {
      KdTemp = KdTemp + 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '5') {
      KdTemp = KdTemp - 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '3') {
      KdTemp = KdTemp + 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '6') {
      KdTemp = KdTemp - 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'A') {
      KdTemp = KdTemp + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == 'B') {
      KdTemp = KdTemp - 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '7') {
      KdTemp = KdTemp + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '*') {
      KdTemp = KdTemp - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '8') {
      KdTemp = KdTemp + 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }

    if (Key == '0') {
      KdTemp = KdTemp - 0.1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Kd:"));
      lcd.setCursor(0, 1);
      lcd.print(KdTemp, 2);
      Serial.print(F("New Kd: "));
      Serial.println(KdTemp);
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("New Kd:"));
  lcd.print(KdTemp);
  lcd.setCursor(0, 1);
  lcd.print(F("Keep:1 Discard:2"));

  while (Key != '2') {
    Key = customKeypad.waitForKey();
    if (Key == '1') {
      Kd = KdTemp;
      EEPROM_writeDouble(KdAddress, Kd);
      myPID.SetTunings(Kp, Ki, Kd);
      Key = '2';
    }
  }
}
// ************************************************
// Run the Auto-Tuning cycle
// ************************************************
void RunAutoTune() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("   AUTOTUNING   "));
  lcd.setCursor(0, 1);
  lcd.print(F("pH=      T="));
  tuning = true;
  // Remember the mode we were in
  ATuneModeRemember = myPID.GetMode();

  // set up the auto-tune parameters
  aTune.SetNoiseBand(aTuneNoise);
  aTune.SetOutputStep(aTuneStep);
  aTune.SetLookbackSec((int)aTuneLookBack);
  while (tuning) {
    if (tuning) {
      byte val = (aTune.Runtime());
      Get_pH();
      lcd.setCursor(3, 1);
      lcd.print(pH, 3);
      Get_Temperature();
      lcd.setCursor(11, 1);
      lcd.print(temp, 2);
      Set_Temp_Comp();
      pH = -99;
      if (val != 0) {
        tuning = false;
      }
      if (!tuning) {  // we're done, set the tuning parameters
        Kp = aTune.GetKp();
        Ki = aTune.GetKi();
        Kd = aTune.GetKd();

        // Re-tune the PID and revert to normal control mode
        myPID.SetTunings(Kp, Ki, Kd);
        myPID.SetMode(ATuneModeRemember);

        // Persist any changed parameters to EEPROM
        EEPROM_writeDouble(KpAddress, Kp);
        EEPROM_writeDouble(KiAddress, Ki);
        EEPROM_writeDouble(KdAddress, Kd);
      }
    }
    char atune_key = customKeypad.getKey();
    if (atune_key == "#") {
      aTune.Cancel();
      tuning = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("   CANCELLING   "));
      lcd.setCursor(0, 1);
      lcd.print(F("   AUTOTUNING   "));
      delay(3000);
    }
  }
}

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
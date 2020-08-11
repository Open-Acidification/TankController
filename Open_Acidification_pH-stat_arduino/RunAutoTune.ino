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
  aTune.SetNoiseBand(ATUNE_NOISE);
  aTune.SetOutputStep(ATUNE_STEP);
  aTune.SetLookbackSec((int)ATUNE_LOOK_BACK);
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
        EEPROM_writeDouble(KP_ADDRESS, Kp);
        EEPROM_writeDouble(KI_ADDRESS, Ki);
        EEPROM_writeDouble(KD_ADDRESS, Kd);
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
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
  a_tune_mode_remember = my_pid.GetMode();

  // set up the auto-tune parameters
  aTune.SetNoiseBand(ATUNE_NOISE);
  aTune.SetOutputStep(ATUNE_STEP);
  aTune.SetLookbackSec((int)ATUNE_LOOK_BACK);
  while (tuning) {
    if (tuning) {
      byte val = (aTune.Runtime());
      GetPh();
      lcd.setCursor(3, 1);
      lcd.print(pH, 3);
      GetTemperature();
      lcd.setCursor(11, 1);
      lcd.print(temp, 2);
      SetTempComp();
      pH = -99;
      if (val != 0) {
        tuning = false;
      }
      if (!tuning) {  // we're done, set the tuning parameters
        Kp = aTune.GetKp();
        Ki = aTune.GetKi();
        Kd = aTune.GetKd();

        // Re-tune the PID and revert to normal control mode
        my_pid.SetTunings(Kp, Ki, Kd);
        my_pid.SetMode(a_tune_mode_remember);

        // Persist any changed parameters to EEPROM
        EepromWriteDouble(KP_ADDRESS, Kp);
        EepromWriteDouble(KI_ADDRESS, Ki);
        EepromWriteDouble(KD_ADDRESS, Kd);
      }
    }
    char atune_key = custom_keypad.getKey();
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
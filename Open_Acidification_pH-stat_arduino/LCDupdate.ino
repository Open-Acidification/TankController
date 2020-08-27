void LcdUpdate() {
  lcd.setCursor(3, 0);
  lcd.print(pHDisplay, 3);
  lcd.setCursor(11, 0);
  lcd.print(phset, 3);
  lcd.setCursor(0, 1);
  lcd.setCursor(2, 1);
  lcd.print(temp, 2);
  lcd.setCursor(11, 1);
  lcd.print(tempset, 2);
  lcd.setCursor(9, 1);
  if (heat == 0) {
    lcd.print("C");
  }
  if (heat == 1) {
    lcd.print("H");
  }
}
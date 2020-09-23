void LcdUpdate() {
  lcd.setCursor(3, 0);
  lcd.print(ph_display, 3);
  lcd.setCursor(11, 0);
  lcd.print(ph_set, 3);
  lcd.setCursor(0, 1);
  lcd.setCursor(2, 1);
  lcd.print(temp, 2);
  lcd.setCursor(11, 1);
  lcd.print(temp_set, 2);
  lcd.setCursor(9, 1);
  if (heat == 0) {
    lcd.print("C");
  }
  if (heat == 1) {
    lcd.print("H");
  }
}

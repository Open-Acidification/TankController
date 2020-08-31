// ************************************************
// Send temperature compensation to Atlas pH EZO
// ************************************************
void SetTempComp() {
  const String PRE_TEMP_COMP = "T,";
  String temp_comp;
  if (temp > 0 && temp < 100) {
    temp_comp = PRE_TEMP_COMP + String(temp, 2);
  } else {
    temp_comp = PRE_TEMP_COMP + 20;
  }
  Serial.println(temp_comp);
  Serial1.print(temp_comp);  // send that string to the Atlas Scientific product
  Serial1.print('\r');       // add a <CR> to the end of the string
}
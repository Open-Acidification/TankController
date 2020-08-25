// ************************************************
// Send temperature compensation to Atlas pH EZO
// ************************************************
void Set_Temp_Comp() {
  const String PRE_TEMP_COMP = "T,";
  String tempcomp;
  if (temp > 0 && temp < 100) {
    tempcomp = PRE_TEMP_COMP + String(temp, 2);
  } else {
    tempcomp = PRE_TEMP_COMP + 20;
  }
  Serial.println(tempcomp);
  Serial1.print(tempcomp);  // send that string to the Atlas Scientific product
  Serial1.print('\r');      // add a <CR> to the end of the string
}
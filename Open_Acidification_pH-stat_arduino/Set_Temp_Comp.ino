// ************************************************
// Send temperature compensation to Atlas pH EZO
// ************************************************
void Set_Temp_Comp() {
  if (temp > 0 && temp < 100) {
    tempcomp = pretempcomp + String(temp, 2);
  } else {
    tempcomp = pretempcomp + 20;
  }
  Serial.println(tempcomp);
  Serial1.print(tempcomp);  // send that string to the Atlas Scientific product
  Serial1.print('\r');      // add a <CR> to the end of the string
}
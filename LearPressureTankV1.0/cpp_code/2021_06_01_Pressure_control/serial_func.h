void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      stringComplete = true;
    }
    if (!stringComplete) {
      // add it to the inputString:
      inputString += inChar;
    }
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

  }
}

void serial_setup() {
  Serial.begin(38400);
  //  printf_init(Serial);
  Debug.timestampOn();
  Debug.print(DBG_INFO, F("---Boot---\n"));
  inputString.reserve(200);
}

void do_serial_debug(int _enc_movement){
  Serial.print("#");
    Serial.print(inputString);
    Serial.println("#");
   

    if (inputString.indexOf("in") >= 0) {
      Serial.println("Rec in");
      prog_in_out = FILL_AIR;
      uint8_t pin = (prog_in_out) ? PIN_IN_SOL : PIN_OUT_SOL;
      if (digitalRead(pin)) pulse_off_now(); else pulse_on_now();
    }
    if (inputString.indexOf("out") >= 0) {
      Serial.println("Rec out");
      prog_in_out = RELIEF_AIR;
      uint8_t pin = (prog_in_out) ? PIN_IN_SOL : PIN_OUT_SOL;
      if (digitalRead(pin)) pulse_off_now(); else pulse_on_now();
    }
    if (inputString.indexOf("e+") >= 0) {
      Serial.println(F("enc+"));
      _enc_movement = ENC_UP;
    }
    if (inputString.indexOf("e-") >= 0) {
      Serial.println(F("enc-"));
      _enc_movement = ENC_DOWN;
    }
    if (inputString.indexOf("enc_click") >= 0) {
      Serial.println(F("enc click"));
      button_click = BUTTON_ENC;
    }
     // clear the string:
    inputString = "";
    stringComplete = false;
}

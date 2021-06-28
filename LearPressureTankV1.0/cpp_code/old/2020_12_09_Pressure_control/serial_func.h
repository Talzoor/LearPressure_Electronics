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
  Serial.begin(57600);
  //  printf_init(Serial);
  Debug.timestampOn();
  Debug.print(DBG_INFO, F("---Boot---\n"));
  inputString.reserve(200);
}

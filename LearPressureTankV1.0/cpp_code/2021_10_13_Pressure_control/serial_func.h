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

void do_serial_debug(int _enc_movement) {
  Serial.print("#");
  Serial.print(inputString);
  Serial.println("#");

  if (inputString.startsWith("readlog") or inputString.startsWith("readcsv")) {
    bool csv = inputString.startsWith("readcsv");
    int pos = inputString.indexOf(" ");
    char file_name[40];
    if (pos == -1) {
      sprintf(file_name, "%s_%d%s", \
              (csv ? "csv" : "log"), \
              sd_file_no, \
              (csv ? ".csv" : ".txt")); // filename ie. "log_76_csv.csv"
      //      Serial.println(F("Error(MAX_SPEED): Command 'read_page' expects an int operand"));
    } else {
      int page = (word)inputString.substring(pos).toInt();
      sprintf(file_name, "%s_%d%s", \
              (csv ? "csv" : "log"), \
              page, \
              (csv ? ".csv" : ".txt")); // filename ie. "log_76_csv.csv"
    }
    dumpFile(file_name);
  }

  //  if (inputString.indexOf("readlog") >= 0) {
  //    dumpFile(sd_file_name);
  //  }
  //  if (inputString.indexOf("readcsv") >= 0) {
  //    char sd_csv_file_name[20];
  //    sprintf(sd_csv_file_name, "%s_%d%s", "csv", sd_file_no, ".csv"); // filename ie. "log_76_csv.csv"
  //    dumpFile(sd_csv_file_name);
  //  }

  if (inputString.indexOf("files") >= 0) {
    File root = SD.open("/");
    printDirectory(root, 0);
  }

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

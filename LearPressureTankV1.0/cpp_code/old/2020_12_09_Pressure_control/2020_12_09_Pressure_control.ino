#define LCD_ON
#define _EEPROMex_DEBUG
#include "include_h.h"

Encoder myEnc(RotaryEncPinB, RotaryEncPinA);

void setup() {
  serial_setup();
  pinMode_setup();
  ADC_setup();
  timer_setup();
  button_setup();
  //  rot_setup();
  EEPROM_setup();
  read_default_vars();
  global_vars_init();
#ifdef LCD_ON
  lcd_setup();
  menu.print_screen(99);
  delay(1000);
  main_screen();
  //  menu.print_screen(0, defaultProg);
#endif

  //  write_vars_for_test(true);



}

void loop() {
  timer.tick();
  //  trig_timer.tick();
  button_check();

  if (run_program_comand) {
    run_program_comand = false;
    run_program_now();
  }
  if (stop_program_comand) {
    stop_program_comand = false;
    stop_program_now(true);
  }
#ifdef LCD_ON
  //  if (enc_move > 0) menu_change(enc_move) ;

  long encNewPosition = myEnc.read() / 4;
  int enc_move = enc_check(encNewPosition);
  if (enc_move > 0) menu_change(enc_move) ;

  if (button_click != NO_CLICK) {
    uint8_t* _vars = menu.Get_vars();
    decide(menu.Get_cur_screen() , menu.Get_selection() , button_click , button_event , _vars);
    //    menu.next_screen(0);
    button_click = NO_CLICK; // take care and pull button down
  }
#endif

  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis >= interval) {
  //    previousMillis = currentMillis;
  //    check_trigs();
  ////    trig_timer = timer.every(10, check_trigs);
  //  }
  if (stringComplete) {
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
      enc_move = ENC_UP;
    }
    if (inputString.indexOf("e-") >= 0) {
      Serial.println(F("enc-"));
      enc_move = ENC_DOWN;
    }
    if (inputString.indexOf("enc_click") >= 0) {
      Serial.println(F("enc click"));
      button_click = BUTTON_ENC;
    }
     // clear the string:
    inputString = "";
    stringComplete = false;
  }
} //loop

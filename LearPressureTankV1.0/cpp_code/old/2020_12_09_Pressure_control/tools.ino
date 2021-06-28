void pinMode_setup() {
  pinMode(RotaryEncPinA , INPUT_PULLUP);
  pinMode(RotaryEncPinB , INPUT_PULLUP);
  pinMode(RotaryEncButton , INPUT_PULLUP);
  //  pinMode(A1 , INPUT);
  pinMode(TRIG_IN_START_PIN, INPUT);
  pinMode(TRIG_IN_STOP_PIN , INPUT);
  pinMode(TRIG_IN_P_UP_PIN, INPUT_PULLUP);
  pinMode(TRIG_IN_P_DOWN_PIN, INPUT_PULLUP);
  pinMode(PIN_IN_SOL, OUTPUT);
  pinMode(PIN_OUT_SOL, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON[i], INPUT_PULLUP);
  }
}



//void print_all_in() {
//  Debug.print(DBG_INFO, F("in: "));
//  for (int i = 14; i < 15; i++) {
//    Debug.print(DBG_INFO, F("%d:%4d\t"), i, (i > 13) ? analogRead(i) : digitalRead(i));
//  }
//  Debug.print(DBG_INFO, F("\n"));
//}


void ADC_setup() {
  // defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

  // set prescale to 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
}

/*void RotaryEncIntA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
  }

  void RotaryEncIntB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
  }

*/

int enc_check(int16_t encNewPosition) {
  int tmp_res = 0;
  if (encOldPosition != encNewPosition) {
    Serial.println(encNewPosition);
    if (encNewPosition > encOldPosition) {
      tmp_res = ENC_UP;
    } else {
      tmp_res = ENC_DOWN;
    }
    encOldPosition = encNewPosition;
  }
  return tmp_res;
}
/*
  void rot_setup() {
  attachInterrupt(digitalPinToInterrupt(RotaryEncPinA), RotaryEncIntA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(digitalPinToInterrupt(RotaryEncPinB), RotaryEncIntB, RISING);
  }
*/

void read_default_vars() {
  //  ReadVar_int(defaultPulse);
  //  Serial.print("defaultPulse:");
  //  Serial.println((int)defaultPulse);
  ReadVar_byte(defaultProg);
  ReadVar_byte(PressureSensor);
  //  Serial.print("defaultProg:");
  //  Serial.println((int)defaultProg);
  //Serial.print("ProgTime:");
  for (int i = 0; i < 10; i++) {
    ReadVar_byte(ProgTimeMin[i]);
    ReadVar_byte(ProgTimeSec[i]);
    ReadVar_byte(ProgInOut[i]);
    ReadVar_byte(ProgNextProg[i]);
    ReadVar_byte(ProgRepeat[i]);
    ReadVar_int(ProgPulseOnLen[i]);
    ReadVar_int(ProgPulseOffLen[i]);
    ReadVar_byte(ProgTrigOffset[i]);

    Debug.print(DBG_INFO, F("#%d# -> P_Time=%d:%d, P_PWM:%d, P_Freq:%d, P_Trig:%d\n"),
                i, (uint8_t)ProgTimeMin[i], (uint8_t)ProgTimeSec[i],
                (uint16_t)ProgPulseOnLen[i], (uint8_t)ProgPulseOffLen[i], (uint8_t)ProgTrigOffset[i]);
  }
  //now manual vars
  ReadVar_int(ManPulseLen);
  ReadVar_byte(ManFullDownPLenMin);
  ReadVar_byte(ManFullDownPLenSec);
  ReadVar_byte(ManFullUpPLenMin);
  ReadVar_byte(ManFullUpPLenSec);

} // read_default_vars

void decide(uint8_t _cur_screen, uint8_t cur_selection, uint8_t _button_click, uint8_t _button_event, uint8_t* _vars) {
  uint8_t prog_num = menu.Get_vars_by_i(0);
  char* scr_cur_text = menu.get_cur_screen_selection_str();

  Debug.print(DBG_INFO, F("_button_click:%d, _cur_screen:%d, cur_selection:%d, _button_event:%d, scr_cur_text:%s\n"),
              _button_click, _cur_screen, cur_selection, _button_event, scr_cur_text);
  //  Debug.print(DBG_INFO, F("size=%d\n"), sizeof(_vars));
  Debug.print(DBG_INFO, F("vars="));
  Debug.timestampOff();
  for (int i = 0; i < 5; i++) {
    Debug.print(DBG_INFO, F("[%d]:%d, "), i, _vars[i]);
  }
  //  Debug.print(DBG_INFO, F("\n"));
  Debug.print(DBG_INFO, F("time=%d:%d\n"), (uint8_t)ProgTimeMin[prog_num], (uint8_t)ProgTimeSec[prog_num]);
  Debug.timestampOn();

  uint8_t inc;
  bool move_to_next_scr = false;
  bool move_to_prev_scr = false;
  if (_button_event == 0) inc = 1; else if (_button_event == 5) inc = 5;  //short press +1, long press +5

  //  if (_button_click == BUTTON_MID) {
  //    Debug.print(DBG_INFO, F("scr_cur_text:%s\n"), scr_cur_text);
  //  }

  if (_button_click == BUTTON_ENC) {      // ------------- ENCODER -------------
    //    Debug.print(DBG_INFO, F("BUTTON_ENC"));
    if (_cur_screen == 0) {
      move_to_next_scr = true;
      menu.next_screen(0);
      menu.Reset_var();
    }
    else if (_cur_screen == 11) {
      if (cur_selection == 1) move_to_next_scr = true;
      save_def_prog();
    }// scr=11

    else if (_cur_screen == 111) {
      //      Debug.print(DBG_INFO, "_cur_screen:%d, defaultProg:%d\n", _cur_screen, defaultProg);
      if (_button_event == 0 and cur_selection == 2) {
        t_prog_sec = !t_prog_sec;
        //                Debug.print(DBG_INFO, F("t_prog_sec:%d\n"), t_prog_sec);
        menu.update_screen();
      }
      //      if (cur_selection == 6) { //save prog
      if (strcmp_P(scr_cur_text, PSTR("Save prog")) == 0) {
        save_vars_for_prog(prog_num);

      } //sel=6
      //      if (cur_selection == 7) { //save all
      if (strcmp_P(scr_cur_text, PSTR("Save all")) == 0) {
        save_all_progs();
      } //sel=7
      //      set_scr111_vars(defaultProg);

    } //scr=111
    else if (_cur_screen == 123) {
      if (_button_event == 4 and (cur_selection == 2 or cur_selection == 3)) {
        t_prog_sec = !t_prog_sec;
        //        Debug.print(DBG_INFO, F("t_prog_sec:%d\n"), t_prog_sec);
        menu.update_screen();
      }
    } //scr=123
    else if (_cur_screen == 14) {
      //nothing
    }
    else if (_cur_screen == 90) {
      //nothing
    }
    else { //all other scr
      move_to_next_scr = true;
    } //all other scr

    if (move_to_next_scr) {
      //      timer.cancel(trig_timer);
      move_to_next_scr = false;
      menu.save_screen_params();
      byte next_scr = (_cur_screen * 10) + cur_selection;
      if (next_scr == 111)
        set_scr111_vars(defaultProg);
      else if (next_scr == 11)
        set_scr11_vars();
      else if (next_scr == 14)
      {
        global_vars_init();
        timer.cancel(trig_timer);
        trig_timer = timer.every(10, check_trigs);
      } // scr=14
      else if (next_scr == 123)
        set_scr123_vars();
      else
        menu.Reset_var();

      menu.Set_selection(1);
      Debug.print(DBG_INFO, F("next_scr:%d\n"), next_scr);
      Debug.print(DBG_INFO, F("Vars reset\n"));
      menu.Set_cur_screen(next_scr);
      menu.update_screen();
    }

  } //button = enc
  if (_button_click == BUTTON_LEFT) {     // ------------- BUTTON_LEFT -------------
    //    menu.save_screen_params();
    menu.Reset_var();
    byte prev_screen = _cur_screen / 10;
    Debug.print(DBG_INFO, F("prev_screen:%d\n"), prev_screen);
    if (_cur_screen == 11) {
      save_def_prog();
    }//cur_scr=11
    else if (_cur_screen == 14) {
      timer.cancel(trig_timer);
    }
    else if (_cur_screen == 123) {
      save_Manual_vars();
    }//cur_scr=123
    else if (_cur_screen == 90) {
      stop_program_now(true);
      prev_screen = 0;
    }

    if (prev_screen == 0) {
      menu.Set_selection(1);
      main_screen();
    }

    else if (prev_screen == 11) {
      set_scr11_vars();
      move_to_prev_scr = true;
    }
    else {
      move_to_prev_scr = true;
    } //prev_screen

    if (move_to_prev_scr) {
      move_to_prev_scr = false;
      menu.Set_cur_screen(prev_screen);
      menu.set_prev_screen_params();
      menu.update_screen();
    }
  }

  if (_button_click == BUTTON_UP) {       // ------------- BUTTON_UP -------------
    if (_cur_screen == 0) {

      Debug.print(DBG_INFO, F("----- _button_event:%d -----  \n"), _button_event);
      if (_button_event == 5) {
        timer.cancel(pulse_off_timer);
        prog_in_out = FILL_AIR;
        pulse_on_now();
        pulse_off_timer = timer.in(250, pulse_off);
      }
      int8_t prog = menu.Get_vars_by_i(0);
      if (_button_event == 0) prog++;
      set_scr00_vars(prog);
    } //scr=0
    else if (_cur_screen == 11) {
      if (cur_selection == 2) {
        //        var_default[1]++;
        //        set_scr11_vars(var_default[1]);
        defaultProg++;
        set_scr11_vars();
        menu.update_screen();
      }
      else if (cur_selection == 3) {
        PressureSensor = !PressureSensor;
        set_scr11_vars();
        menu.update_screen();
      }
    }//scr=11
    else if (_cur_screen == 13) {
      init_prog_vars(prog_num);
      run_program_now();
    } //scr=13
    else if (_cur_screen == 111) {
      //      if (cur_selection == 1) {
      if (strcmp_P(scr_cur_text, PSTR("PROG")) == 0) {
        prog_num = (prog_num >= 9) ? 0 : ++prog_num;
      } //sel=1
      //      if (cur_selection == 2) {
      if (strcmp_P(scr_cur_text, PSTR("T[m:s]")) == 0) {
        //        uint8_t* t_vars = menu.Get_time_vars();
        uint8_t t1 = ProgTimeMin[prog_num]; //min
        uint8_t t2 = ProgTimeSec[prog_num]; //sec
        if (t_prog_sec) { //seconds
          t2 += inc;
          t2 = (t2 >= 59) ? 0 : t2;
        } else {          // minutes
          t1 += inc;
          t1 = (t1 >= 59) ? 0 : t1;
        }
        ProgTimeMin[prog_num] = t1; ProgTimeSec[prog_num] = t2;
        menu.Set_time_vars(t1, t2, 0 , 0);
      } //sel=2
      //      if (cur_selection == 3) {
      if (strcmp_P(scr_cur_text, PSTR("In/Out")) == 0) {
        uint16_t p_in = ProgInOut[prog_num];
        p_in = !p_in;   // 0 or 1
        ProgInOut[prog_num] = p_in;
      }
      if (strcmp_P(scr_cur_text, PSTR("NextProg")) == 0) {
        uint16_t p_next = ProgNextProg[prog_num];
        p_next += 1;
        p_next = (p_next > 10) ? 0 : p_next;
        ProgNextProg[prog_num] = p_next;
      }
      if (strcmp_P(scr_cur_text, PSTR("Repeat")) == 0) {
        uint16_t p_repeat = ProgRepeat[prog_num];
        p_repeat += 1;
        p_repeat = (p_repeat > 100) ? 0 : p_repeat;
        ProgRepeat[prog_num] = p_repeat;
      }

      if (strcmp_P(scr_cur_text, PSTR("OPEN[ms]")) == 0) {
        uint16_t p_on_len = ProgPulseOnLen[prog_num];
        p_on_len += inc;
        p_on_len = (p_on_len > 9999) ? 1 : p_on_len;
        ProgPulseOnLen[prog_num] = p_on_len;
      } //sel=3
      //      if (cur_selection == 4) {

      if (strcmp_P(scr_cur_text, PSTR("CLOSED[ms]")) == 0) {
        uint16_t p_off_len = ProgPulseOffLen[prog_num];
        p_off_len += (inc * 100);
        p_off_len = p_off_len / 100 * 100; // integer division
        p_off_len = (p_off_len > 65000L) ? 1 : p_off_len;
        ProgPulseOffLen[prog_num] = p_off_len;
      } //sel=4
      //      if (cur_selection == 5) {
      if (strcmp_P(scr_cur_text, PSTR("TrgOfst[s]")) == 0) {
        uint8_t t_off = ProgTrigOffset[prog_num];
        t_off += inc;
        t_off = (t_off > 60) ? 0 : t_off;
        ProgTrigOffset[prog_num] = t_off;
      } //sel=5
      set_scr111_vars(prog_num);
      menu.update_screen();
    } //scr=111

    if (_cur_screen == 123) {
      if (cur_selection == 1) {
        uint16_t pulse_ms = ManPulseLen;
        pulse_ms += inc;
        pulse_ms = (pulse_ms >= 10L * 1000) ? 0 : pulse_ms;
        ManPulseLen = pulse_ms;
      } //sel=1
      if (cur_selection == 2) {
        uint8_t t1 = ManFullDownPLenMin, t2 = ManFullDownPLenSec;
        uint8_t t3 = ManFullUpPLenMin, t4 = ManFullUpPLenSec;
        if (t_prog_sec) { //seconds
          t2 += inc;
          t2 = (t2 > 59) ? 0 : t2;
        } else {          // minutes
          t1 += inc;
          t1 = (t1 > 59) ? 1 : t1;
        }
        ManFullDownPLenMin = t1; ManFullDownPLenSec = t2;
        //        menu.Set_time_vars(t1, t2, t3 , t4);
      } //sel=2
      if (cur_selection == 3) {
        uint8_t t1 = ManFullDownPLenMin, t2 = ManFullDownPLenSec;
        uint8_t t3 = ManFullUpPLenMin, t4 = ManFullUpPLenSec;
        if (t_prog_sec) { //seconds
          t4 += inc;
          t4 = (t4 > 59) ? 0 : t4;
        } else {          // minutes
          t3 += inc;
          t3 = (t3 > 59) ? 1 : t3;
        }
        ManFullUpPLenMin = t3; ManFullUpPLenSec = t4;
        //        menu.Set_time_vars(t1, t2, t3 , t4);
      } //sel=3
      set_scr123_vars();
      menu.update_screen();
    }//scr=123
  } //button up

  if (_button_click == BUTTON_DOWN) {       // ------------- BUTTON_DOWN -------------
    if (_cur_screen == 0) {
      if (_button_event == 5) {
        timer.cancel(pulse_off_timer);
        prog_in_out = RELIEF_AIR;
        pulse_on_now();
        pulse_off_timer = timer.in(250, pulse_off);
      }
      int8_t prog = menu.Get_vars_by_i(0);
      if (_button_event == 0) prog--;
      set_scr00_vars(prog);
    } //scr=0
    else if (_cur_screen == 11) {
      if (cur_selection == 2) {
        //        --var_default[1];
        //        set_scr11_vars(var_default[1]);
        defaultProg--;
        set_scr11_vars();
        menu.update_screen();
      }
      else if (cur_selection == 3) {
        PressureSensor = !PressureSensor;
        set_scr11_vars();
        menu.update_screen();
      }
    }//scr=11
    else if (_cur_screen == 90) {
      stop_program_now(true);
    }
    else if (_cur_screen == 13) {
      stop_program_now(true);
    } //scr=13
    else if (_cur_screen == 111) {
      //      if (cur_selection == 1) {
      if (strcmp_P(scr_cur_text, PSTR("PROG")) == 0) {
        //        uint8_t prog_num = menu.Get_vars_by_i(0);
        prog_num = (prog_num == 0) ? 9 : --prog_num;
        //        set_scr111_vars(prog_num);

      } //sel=1
      //  if (cur_selection == 2) {
      if (strcmp_P(scr_cur_text, PSTR("T[m:s]")) == 0) {
        //        uint8_t* t_vars = menu.Get_time_vars();
        uint8_t t1 = ProgTimeMin[prog_num]; //min
        uint8_t t2 = ProgTimeSec[prog_num]; //sec
        //        uint8_t t1 = t_vars[0]; //min
        //        uint8_t t2 = t_vars[1]; //sec
        if (t_prog_sec) { //seconds
          t2 = (t2 == 0) ? 59 : t2 -= inc;   //uint8 has not negatives
        } else {          // minutes
          t1 = (t1 == 0) ? 59 : t1 -= inc;
        }
        ProgTimeMin[prog_num] = t1; ProgTimeSec[prog_num] = t2;
        menu.Set_time_vars(t1, t2, 0 , 0);
      } //sel=2

      if (strcmp_P(scr_cur_text, PSTR("In/Out")) == 0) {
        uint16_t p_in = ProgInOut[prog_num];
        p_in = !p_in;   // 0 or 1
        ProgInOut[prog_num] = p_in;
      }

      if (strcmp_P(scr_cur_text, PSTR("NextProg")) == 0) {
        uint16_t p_next = ProgNextProg[prog_num];
        p_next = (p_next == 0) ? 10 : --p_next;
        ProgNextProg[prog_num] = p_next;
      }
      if (strcmp_P(scr_cur_text, PSTR("Repeat")) == 0) {
        uint16_t p_repeat = ProgRepeat[prog_num];
        p_repeat = (p_repeat <= 0) ? 100 : --p_repeat;
        ProgRepeat[prog_num] = p_repeat;
      }
      if (strcmp_P(scr_cur_text, PSTR("OPEN[ms]")) == 0) {
        //    if (cur_selection == 3) {
        uint16_t p_on_len = ProgPulseOnLen[prog_num];
        Debug.print(DBG_INFO, F("ProgPulseOnLen:%d\n"), ProgPulseOnLen[prog_num]);
        p_on_len -= inc;
        p_on_len = (p_on_len < 1) ? 9999 : p_on_len;
        ProgPulseOnLen[prog_num] = p_on_len;
        Debug.print(DBG_INFO, F("ProgPulseOnLen:%d\n"), ProgPulseOnLen[prog_num]);
      } //sel=3

      //    if (cur_selection == 4) {
      if (strcmp_P(scr_cur_text, PSTR("CLOSED[ms]")) == 0) {
        uint16_t p_off_len = ProgPulseOffLen[prog_num];
        p_off_len = (p_off_len <= 0) ? 65000L : p_off_len -= inc * 100;
        p_off_len = p_off_len / 100 * 100; // integer division
        ProgPulseOffLen[prog_num] = p_off_len;
      } //sel=4
      //    if (cur_selection == 5) {
      if (strcmp_P(scr_cur_text, PSTR("TrgOfst[s]")) == 0) {
        uint8_t t_off = ProgTrigOffset[prog_num];
        t_off = (t_off == 0) ? 60 : t_off -= inc;
        ProgTrigOffset[prog_num] = t_off;
      } //sel=5

      set_scr111_vars(prog_num);
      menu.update_screen();
    } //scr=111

    if (_cur_screen == 123) {
      if (cur_selection == 1) {
        uint16_t pulse_ms = ManPulseLen;
        pulse_ms = (pulse_ms == 0) ? 10L * 1000 : pulse_ms -= inc;
        ManPulseLen = pulse_ms;
      } //sel=1
      if (cur_selection == 2) {
        uint8_t t1 = ManFullDownPLenMin, t2 = ManFullDownPLenSec;
        uint8_t t3 = ManFullUpPLenMin, t4 = ManFullUpPLenSec;
        if (t_prog_sec) { //seconds
          t2 = (t2 == 0) ? 59 : t2 -= inc;   //uint8 has not negatives
        } else {          // minutes
          t1 = (t1 == 0) ? 59 : t1 -= inc;
        }
        ManFullDownPLenMin = t1; ManFullDownPLenSec = t2;
        //        menu.Set_time_vars(t1, t2, t3 , t4);
      } //sel=2
      if (cur_selection == 3) {
        uint8_t t1 = ManFullDownPLenMin, t2 = ManFullDownPLenSec;
        uint8_t t3 = ManFullUpPLenMin, t4 = ManFullUpPLenSec;
        if (t_prog_sec) { //seconds
          t4 = (t4 == 0) ? 59 : t4 -= inc;   //uint8 has not negatives
        } else {          // minutes
          t3 = (t3 == 0) ? 59 : t3 -= inc;
        }
        ManFullUpPLenMin = t3; ManFullUpPLenSec = t4;
        //        menu.Set_time_vars(t1, t2, t3 , t4);
      } //sel=3
      set_scr123_vars();
      menu.update_screen();
    }//scr=123
  } //button down
} //decide

void set_scr111_vars(uint8_t _prog) {
  uint8_t n = 0;
  global_vars_init();
  var_default[n++] = _prog;
  var_default[n++] = 0; //time replacment
  var_default[n++] = ProgInOut[_prog];
  var_default[n++] = ProgNextProg[_prog];
  var_default[n++] = ProgRepeat[_prog];
  //  var_default[n++] = ProgPulseOnLen[_prog];
  var_default[n++] = 0; //Pulse activation
  //  var_default[n++] = ProgPulseOffLen[_prog];
  var_default[n++] = 0;
  var_default[n++] = ProgTrigOffset[_prog];
  menu.Set_vars(var_default);
  menu.Set_PulseLen(ProgPulseOnLen[_prog], ProgPulseOffLen[_prog]);
  menu.Set_time_vars(ProgTimeMin[_prog], ProgTimeSec[_prog], 0 , 0);
}

void set_scr11_vars() {
  uint8_t prog = defaultProg;
  global_vars_init();
  prog = (prog<0 or prog>250) ? 9 : prog;
  prog = (prog > 9) ? 0 : prog;
  defaultProg = prog;
  //  var_default[0] = NULL_VAL;
  var_default[1] = defaultProg;
  var_default[2] = PressureSensor;
  menu.Set_PulseLen(ManPulseLen, 0);
  menu.Set_vars(var_default);
}

void set_scr123_vars() {
  //  Debug.print(DBG_INFO, F("set_scr123_vars"));
  global_vars_init();
  for (int i = 0; i < 3; i++) var_default[i] = 0; //to activate vars
  uint8_t t1m = ManFullDownPLenMin, t1s = ManFullDownPLenSec;
  uint8_t t2m = ManFullUpPLenMin, t2s = ManFullUpPLenSec;
  menu.Set_time_vars(t1m, t1s, t2m , t2s);
  menu.Set_PulseLen(ManPulseLen, 0);
  menu.Set_vars(var_default);
}

void set_scr00_vars(int8_t _prog) {
  uint8_t prog;
  global_vars_init();
  prog = (_prog > 9) ? 0 : ((_prog < 0) ? 9 : _prog);
  var_default[0] = prog;
  menu.Set_vars(var_default);
  menu.update_screen();
}


void global_vars_init() {
  //  Debug.print(DBG_INFO, "sizeof(var_default)/sizeof(byte):%d\n", sizeof(var_default) / sizeof(byte));
  for (int i = 0; i < sizeof(var_default) / sizeof(byte); i++) {
    var_default[i] = NULL_VAL;
    //    Debug.print(DBG_INFO, "var_default[%d]=%d, ", i, var_default[i]);
  }
  //  t_prog_sec = false;
}

void save_vars_for_prog(uint8_t _prog_num) {
  WriteVar_byte(ProgTimeMin[_prog_num]);
  WriteVar_byte(ProgTimeSec[_prog_num]);
  WriteVar_byte(ProgInOut[_prog_num]);
  WriteVar_byte(ProgNextProg[_prog_num]);
  WriteVar_byte(ProgRepeat[_prog_num]);
  WriteVar_int(ProgPulseOnLen[_prog_num]);
  WriteVar_int(ProgPulseOffLen[_prog_num]);
  WriteVar_byte(ProgTrigOffset[_prog_num]);
  menu.print_ok();
}

void save_all_progs() {
  for (int i = 0; i < menu.VAR_SIZE; i++) {
    save_vars_for_prog(i);
  }
}

void save_def_prog() {
  WriteVar_byte(defaultProg);
  WriteVar_byte(PressureSensor);
}

void save_Manual_vars() {
  WriteVar_int(ManPulseLen);
  WriteVar_byte(ManFullDownPLenMin);
  WriteVar_byte(ManFullDownPLenSec);
  WriteVar_byte(ManFullUpPLenMin);
  WriteVar_byte(ManFullUpPLenSec);
}

void write_vars_for_test(bool _zero) {
  for (int i = 0; i < 10; i++) {
    //    uint16_t time_var = 10 * 60 + 46; //for 10min and 46sec
    if (_zero) {
      ProgTimeMin[i] = 0;
      ProgTimeSec[i] = 0;
      ProgTimeSec[i] = 0;
      ProgInOut[i] = 0;
      ProgPulseOnLen[i] = 0;
      ProgPulseOffLen[i] = 0;
      ProgTrigOffset[i] = 0;
    }
    else {  // some random vars
      ProgTimeMin[i] = 10 - i;
      ProgTimeSec[i] = 46 + i;
      ProgTimeSec[i] = 20 - i;
      ProgInOut[i] = 1 ^ i;
      ProgPulseOnLen[i] = 1 + pow(i, 2);
      ProgPulseOffLen[i] = pow(i, 2) * 1.1;
      ProgTrigOffset[i] = pow(i, 2) * 1.5;
    }

    WriteVar_byte(ProgTimeMin[i]);
    WriteVar_byte(ProgTimeSec[i]);
    WriteVar_byte(ProgInOut[i]);
    WriteVar_byte(ProgNextProg[i]);
    WriteVar_byte(ProgRepeat[i]);
    WriteVar_int(ProgPulseOnLen[i]);

    WriteVar_int(ProgPulseOffLen[i]);

    WriteVar_byte(ProgTrigOffset[i]);

    Debug.print(DBG_INFO, F("#%d# -> P_Time=%d:%d, P_PWM:%d, P_Freq:%d, P_Trig:%d\n"),
                i, (uint8_t)ProgTimeMin[i], (uint8_t)ProgTimeSec[i],
                (uint16_t)ProgPulseOnLen[i], (uint16_t)ProgPulseOffLen[i], (uint8_t)ProgTrigOffset[i]);
  }
}



void print_prog_params(byte _prog_to_run, long _ttl_time, long _offset_ms, bool _prog_in_out, int _p_up_len, int _p_dn_len, byte _prog_next) {
  Debug.print(DBG_INFO, F("prog:%d, ttl_time_ms:%ld, offset:%ld, in/out:%s, pulse_len_ms:%d, pulse_dn_ms:%d, next:%d\n"),
              _prog_to_run, _ttl_time, _offset_ms, (_prog_in_out == FILL_AIR) ? "IN" : "OUT", _p_up_len, _p_dn_len, _prog_next);
}

void run_program_now() {
  menu.Set_cur_screen(90);
  menu.update_screen();
  Debug.print(DBG_INFO, F("prog_to_run:%d \n"), prog_to_run);
  prog_time = ProgTimeMin[prog_to_run] * 60L + ProgTimeSec[prog_to_run];  // Time in sec
  //  prog_in_out = !prog_in_out; // so we can repeate same prog just opp.
  //  prog_next = ProgNextProg[prog_to_run];
  Debug.print(DBG_INFO, F("prog_time_sec:%d \n"), prog_time);
  uint16_t prog_pulse_len_ms = ProgPulseOnLen[prog_to_run];
  //  float Pul_Down_Ratio = (1.0 / (ProgRatioHg[prog_to_run] / 100.0)) - 1;
  //  uint16_t prog_pulse_down_ms = prog_pulse_len_ms * Pul_Down_Ratio;
  uint16_t prog_pulse_down_ms = ProgPulseOffLen[prog_to_run];
  uint16_t prog_start_offset = ProgTrigOffset[prog_to_run];                  // offset in sec
  Debug.print(DBG_INFO, F("prog_start_offset_sec:%d \n"), prog_start_offset);
  uint32_t prog_start_offset_ms = prog_start_offset * 1000;
  prog_off_var = false;
  uint32_t ttl_prog_time_ms = (prog_time + prog_start_offset) * 1000L;
  params.p_up_len = prog_pulse_len_ms;
  params.p_down_len = prog_pulse_down_ms;

  print_prog_params(prog_to_run, ttl_prog_time_ms, prog_start_offset_ms, \
                    prog_in_out, params.p_up_len, params.p_down_len, prog_next);

  Debug.print(DBG_INFO, F("------------- PROG_ON ------------- \n"));
  strcpy_P(run_status, run_pr_offset);
  menu.update_screen();
  //  timer.cancel(trig_timer);
  program_offset_timer = timer.in(10 + prog_start_offset_ms, start_prog);   // 10ms+ so timer will not be 0.
  program_shutoff_timer = timer.in(ttl_prog_time_ms, shutoff_prog);         // main program timer
  trig_timer = timer.every(10, check_trigs);                                // keep trigers active
  timer.cancel(next_p_timer);
}

uint8_t prog_next_calc(volatile uint8_t &p_next_ret, uint8_t &times) {
  uint8_t p_next = prog_next;
  //  p_next_ret = 10;
  //  times = 0;

  p_next_ret = p_next;
  init_prog_vars(p_next_ret);
  if (prog_next == prog_to_run) {
    int8_t fill_or_relief;
    fill_or_relief = pow(-1, times);
    prog_in_out = (fill_or_relief == -1) ? FILL_AIR : RELIEF_AIR;
    Debug.print(DBG_INFO, F("fill_or_relief:%d, prog_in_out:%d\n"), fill_or_relief, prog_in_out);
  } else { // separate programs
    prog_in_out = ((ProgInOut[prog_to_run]) ? FILL_AIR : RELIEF_AIR);
  }
  //  prog_in_out = !((ProgInOut[prog_to_run]) ? FILL_AIR : RELIEF_AIR);     // starting opposite so we can auto flip rev prog (start_prog.. flip it back)

  if (p_next == 10) {  // no next prog
    p_next_ret = 10;
  }
  if (times > 0) { // repeat x-10 times
    Debug.print(DBG_INFO, F("prog_prev:%d, prog_next:%d\n"), prog_next, prog_to_run);
    p_next_ret = prog_to_run;
    times--;
  }
  if (times == 0) {
    p_next_ret = 10;
  }
  //  return p_next_ret;
}

void stop_program_now(bool _stop_all) {
  Debug.print(DBG_INFO, F("stop_program_now(%d)\n"), _stop_all);
  timer.cancel(program_offset_timer);
  timer.cancel(program_shutoff_timer);
  timer.cancel(pulse_on_timer);
  timer.cancel(pulse_off_timer);
  timer.cancel(next_p_timer);

  prog_off_var = true;
  pulse_off_now();
  strcpy_P(run_status, run_pr_fin);

  if (_stop_all) {
    prog_run_x_times = 0; //reset
    prog_to_run = 10;
  }
  if (!_stop_all) {   //next prog
    //    prog_run_x_times = 0;
    prog_next_calc(prog_to_run, prog_run_x_times);
    Debug.print(DBG_INFO, F("prog_to_run:%d, times:%d\n"), prog_to_run, prog_run_x_times);
    if (prog_to_run != 10) {
      next_p_timer = timer.in(PAUSE_BETWEEN_PROGS, run_program_now); // 10=stop, otherwise run next prog
      strcpy_P(run_status, run_pr_fin_and_next);  // status_update
    }
  }
  if (prog_to_run == 10) {
    analogWrite(FINISH_OUT_SIGNAL_PIN, PWM_HIGH); // send pulse out when finised
    pulse_out_timer = timer.in(FINISH_P_OUT_SIGNAL_LEN_MS, out_finish_signal_off); // 1s pulse out
  }
  menu.update_screen();
  Debug.print(DBG_INFO, F("------------- PROG_OFF ------------- \n"));
}

void pulse_on_now() {
  strcpy_P(run_status, run_pr_in);
  menu.update_screen();
  uint8_t pin = (prog_in_out) ? PIN_IN_SOL : PIN_OUT_SOL;
  Debug.print(DBG_INFO, F("ON ------------- (pin%d)\n"), pin);
  analogWrite(pin, ON_VAL);
}

void pulse_off_now() {
  strcpy_P(run_status, run_pr_out);
  menu.update_screen();
  uint8_t pin = (prog_in_out) ? PIN_IN_SOL : PIN_OUT_SOL;
  Debug.print(DBG_INFO, F("OFF ------------- (pin%d)\n"), pin);
  digitalWrite(pin, OFF_VAL);
}

void init_prog_vars(uint8_t _prog) {
  Debug.print(DBG_INFO, F("-----INIT------ (_prog:%d)\n"), _prog);
  if (prog_run_x_times == 0 and ProgRepeat[_prog] != 0) prog_run_x_times = ProgRepeat[_prog];
  prog_to_run = _prog;
  prog_next = ProgNextProg[prog_to_run];
}

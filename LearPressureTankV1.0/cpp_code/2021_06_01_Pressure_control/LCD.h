#ifdef LCD_ON
#define LCD_ROWS 16
LiquidCrystal_I2C lcd(0x27, LCD_ROWS, 2);

class lcd_menu
{
  public:
    static const byte VAR_SIZE = 10;


    lcd_menu() {  //init
      this->Reset_var();
    }

    void init() {
      lcd.createChar(0, arrow_up);
      lcd.createChar(1, arrow_down);
      lcd.createChar(2, selection_sign);
      lcd.createChar(3, round_sign);
      lcd.createChar(4, empty_square);
      lcd.createChar(5, full_square);
    }

    void scr14update() {
      lcd.setCursor(5, 0);    //Start
      if (this->vars[0] == true) lcd.write(5); else lcd.write(4);
      sprintf(buffer, "%02d", this->vars[1]);
      lcd.print(buffer);
      lcd.setCursor(12, 0);   //pUp
      if (this->vars[4] == true) lcd.write(5); else lcd.write(4);
      sprintf(buffer, "%02d", this->vars[5]);
      lcd.print(buffer);
      lcd.setCursor(5, 1);    //Stop
      if (this->vars[2] == true) lcd.write(5); else lcd.write(4);
      sprintf(buffer, "%02d", this->vars[3]);
      lcd.print(buffer);
      lcd.setCursor(12, 1);   //pDn
      if (this->vars[6] == true) lcd.write(5); else lcd.write(4);
      sprintf(buffer, "%02d", this->vars[7]);
      lcd.print(buffer);
    }

    void print_ok() {
      strcpy_P(buffer, (char *)pgm_read_word(&string_OK[0]));
      lcd.setCursor(14, 0);
      lcd.print(buffer);
    }

    byte screen_idx(byte _no, bool &_r_scrren) { //screen index
      int _len = sizeof(menu_table) / sizeof(*menu_table);
      byte return_val = 99;
      //      Debug.print(DBG_INFO, "_no:%d\n", _no);
      for (int i = 0; i < _len; i++) {
        strcpy_P(buffer, (char *)pgm_read_word(&(menu_table[i])));  // Necessary casts and dereferencing, just copy.
        //        Debug.print(DBG_INFO, "i:%d ,buf(i):%s\n", i, buffer);
        byte tmp_b = this->scr_exist(buffer, _no);

        if (tmp_b == SCREEN_EXIST or tmp_b == SCREEN_EXIST_W_R) {
          if (tmp_b == SCREEN_EXIST_W_R) _r_scrren = true;
          return_val = i;
          break;
        }
      }
      return return_val;

    }

    byte R_screen_idx(byte _no) { //screen index
      //      Debug.print(DBG_INFO, "--------------R_screen--------------\n", _no);

      int _len = sizeof(right_menu_table) / sizeof(*right_menu_table);
      byte return_val = 0;
      for (int i = 0; i < _len; i++) {
        strcpy_P(buffer, (char *)pgm_read_word(&(right_menu_table[i])));  // Necessary casts and dereferencing, just copy.
        byte tmp_b = this->scr_exist(buffer, _no);

        if (tmp_b == SCREEN_EXIST or tmp_b == SCREEN_EXIST_W_R) {
          return_val = i;
          break;
        }
      }
      //      Debug.print(DBG_INFO, "return_val:%d\n", return_val);

      return return_val;
    }

    bool print_left_dot() {
      bool tmp_b = true;
      if (this->cur_screen == 99) tmp_b = false;
      if (this->cur_screen == 0) tmp_b = false;
      if (this->cur_screen == 14) tmp_b = false;
      if (this->cur_screen == 90) tmp_b = false;
      return tmp_b;
    }

    void print_screen(byte _scr) {
      this->print_screen(_scr, this->vars);
    }

    void print_screen(byte _scr, uint8_t *_var) {
      byte n = 1;
      //      Debug.print(DBG_INFO, "_prog_num:%d\n", _prog_num);
      this->cur_screen = _scr;
      bool r_screen_exist = false;
      byte scr_idx = this->screen_idx(_scr, r_screen_exist);
      char* _tmp_sec_str;

      //      Debug.print(DBG_INFO, "_scr:%d, scr_idx:%d\n", _scr, scr_idx);
      if (this->cur_selection > 2) { //odd selection
        n = this->cur_selection;
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      if (this->print_left_dot() and this->cur_selection == n) lcd.write(2); //else lcd.write(" ");
      _tmp_sec_str = this->screen_line(scr_idx + n);

      lcd.print(_tmp_sec_str);

      // ------------------- line 1 only -------------------
      if (_var[n - 1] != NULL_VAL) {

        if (strcmp_P(_tmp_sec_str, PSTR("Fill")) == 0) {
          //          Debug.print(DBG_INFO, "YES, %s\n", _tmp_sec_str);
          sprintf_P(buffer, PSTR(" %c%d:%02d%c"),
                    ((t_prog_sec == false) ? (char*)62 : (char*)32),
                    (time_vars[2]),
                    (time_vars[3]),
                    ((t_prog_sec == true) ? (char*)60 : (char*)0));
          //          if (defaultProg == true) strcat(buffer, "<-");
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Run P")) == 0) {
          if (prog_to_run != 10) {
            char bf[10];
            sprintf_P(buffer, PSTR("%d P=%s"), prog_to_run, dtostrf(this->pressure, 2, 2, bf));
            if (prog_run_x_times > 0) sprintf(buffer, "%sx%d", buffer, prog_run_x_times);
          }
          if (prog_to_run == 10) {
            sprintf(buffer, " --- ");
          }
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("In/Out")) == 0) {
          sprintf_P(buffer, PSTR(" %s"),
                    (_var[n - 1] == FILL_AIR) ? "OUT" : "IN");
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("PROG")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Prog")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("NextProg")) == 0) {
          if (_var[n - 1] == 10) sprintf_P(buffer, PSTR(" -|"));
          else if (_var[n - 1] >= 11) sprintf_P(buffer, PSTR(" <-%d"), _var[n - 1] - 10);
          else sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("OPEN[ms]")) == 0) {
          //          Debug.print(DBG_INFO, "YES, %s\n", _tmp_sec_str);
          sprintf_P(buffer, PSTR(" %d"), (this->PulseOnLen));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("CLOSED[ms]")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), (this->PulseOffLen));
          //          Debug.print(DBG_INFO, F("PULSE"));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("P.sensor")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), (_var[n - 1]));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("P=")) == 0) {
          char bf[10];
          sprintf_P(buffer, PSTR("%sATM"), dtostrf(this->pressure, 2, 2, bf)/*P_val*/);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Step")) == 0) {
          char bf[10];
          sprintf_P(buffer, PSTR(" %s"), dtostrf(this->step_length, 2, 2, bf));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Time/stp")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("PrgOfst[s]")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("NextProg")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Repeat")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n - 1]);
        }
        else
        {
          //          sprintf(buffer, " %d", _var[n - 1]);
        }
        lcd.print(buffer);
        if (_scr == 13 and this->cur_selection == 1) lcd.write(3);
      } //var[n-1]

      lcd.setCursor(0, 1);
      if (this->print_left_dot() and this->cur_selection == (n + 1)) lcd.write(2); //else lcd.write(" ");

      if (n + 1 <= this->screen_lines_available) {
        _tmp_sec_str = this->screen_line(scr_idx + (n + 1));
        lcd.print(_tmp_sec_str);
      }

      if (_scr == 90) {
        strcpy(buffer, run_status);
        lcd.print(buffer);
      }

      // ------------------- line 2 only -------------------
      if (_var[n] != NULL_VAL) {
        //        Debug.print(DBG_INFO, F("n=%d,_var[n]:[n+1]=%d:%d\n"), n, (uint8_t)_var[n],(uint8_t)_var[n+1]);
        if (strcmp_P(_tmp_sec_str, PSTR("T[m:s]")) == 0) {
          //          Debug.print(DBG_INFO, "YES, %s\n", _tmp_sec_str);
          sprintf(buffer, " %c%d:%02d%c",
                  ((t_prog_sec == false) ? (char*)62 : (char*)32),
                  (time_vars[0]),
                  (time_vars[1]),
                  ((t_prog_sec == true) ? (char*)60 : (char*)0));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Ps.trgt")) == 0) {
          char bf[10];
          sprintf_P(buffer, PSTR(" %s"), dtostrf(this->pressure_target, 2, 2, bf));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("OPEN[ms]")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), (this->PulseOnLen));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("CLOSED[ms]")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), (this->PulseOffLen));
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("NextProg")) == 0) {
          if (_var[n] == 10) sprintf_P(buffer, PSTR(" -|"));
          else if (_var[n] >= 11) sprintf_P(buffer, PSTR(" <-%d"), _var[n] - 10);
          else sprintf_P(buffer, PSTR(" %d"), _var[n]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Relief")) == 0) {
          //          Debug.print(DBG_INFO, "YES, %s\n", _tmp_sec_str);
          sprintf_P(buffer, PSTR(" %c%d:%02d%c"),
                    ((t_prog_sec == false) ? (char*)62 : (char*)32),
                    (time_vars[0]),
                    (time_vars[1]),
                    ((t_prog_sec == true) ? (char*)60 : (char*)0));
          //          if (defaultProg == true) strcat(buffer, "<-");
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Pr")) == 0) {
          //          lcd.setCursor(2, 1);
          sprintf_P(buffer, PSTR("%2d Trig rdy"), _var[n]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Time/stp")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("PrgOfst[s]")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("NextProg")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n]);
        }
        else if (strcmp_P(_tmp_sec_str, PSTR("Repeat")) == 0) {
          sprintf_P(buffer, PSTR(" %d"), _var[n]);
        }

        else {
          sprintf(buffer, " %d", _var[n]);
        }

        lcd.print(buffer);
      }
      //now check if there is R screen:

      if (r_screen_exist) {
        //        Debug.print(DBG_INFO, "there is R screen");
        byte R_scr_idx = this->R_screen_idx(_scr);

        //        Debug.print(DBG_INFO, "R_scr_idx:%d, R_screen_line:%s\n", R_scr_idx, this->R_screen_line(R_scr_idx + 1));


        if (_scr == 0
            or _scr == 13
            or (_scr == 11 and (this->cur_selection == 2 or this->cur_selection == 3))
            or _scr == 123
            or _scr == 90
            or (_scr == 12 and this->cur_selection != 3)
            or (_scr == 111 and (this->cur_selection != 6 and this->cur_selection != 7))
           ) {  //right arrows
          lcd.setCursor(15, 0);
          lcd.write(0);
          lcd.setCursor(15, 1);
          lcd.write(1);

        }
        char *str = this->R_screen_line(R_scr_idx + 1);
        byte str_len = strlen(str);
        if (_scr == 12 and this->cur_selection == 3) { //don't print right str on this screen

        } else
        {
          lcd.setCursor(16 - str_len, 0);
          lcd.print(str);   //first left line
        }
        str = this->R_screen_line(R_scr_idx + 2);
        str_len = strlen(str);
        if (_scr == 12 and this->cur_selection == 3) {

        } else
        {
          lcd.setCursor(16 - str_len, 1);
          lcd.print(str); //sec left line
        }
      } //r_screen_exist

    }

    //    void print_left(byte _scr) {
    //      bool r_screen_exist = false;
    //      byte scr_idx = this->screen_idx(_scr, r_screen_exist);
    //      lcd.setCursor(0, 0);
    //      lcd.print(this->screen_line(scr_idx + 1));
    //      lcd.setCursor(0, 1);
    //      lcd.print(this->screen_line(scr_idx + 2));
    //    }

    void update_pressure() {
      if (this->cur_screen == 0) { //main
        lcd.setCursor(2, 0);
        char bf[10];
        sprintf_P(buffer, PSTR("%s"), dtostrf(this->pressure, 2, 2, bf)/*P_val*/);
        lcd.print(buffer);
      } else if (this->cur_screen == 90) {
        lcd.setCursor(9, 0);
        char bf[10];
        sprintf_P(buffer, PSTR("%s"), dtostrf(this->pressure, 2, 2, bf)/*P_val*/);
        lcd.print(buffer);
      }

    }

    void update_screen() {
      //      Debug.print(DBG_INFO, "update_screen:cur_selection:%d", this->cur_selection);
      if (this->cur_screen == 99) {
        this->cur_selection = 1;

      }
      else if (this->cur_screen == 13) {
        this->print_screen(13, this->vars);
      }
      else if (this->cur_screen == 12) {
        this->print_screen(12, this->vars);
      }
      else
      {
        this->print_screen(this->cur_screen);
      }
    } // update_screen

    char* screen_line(byte _idx) {
      strcpy_P(buffer, (char *)pgm_read_word(&(menu_table[_idx])));  // Necessary casts and dereferencing, just copy.
      return buffer;
    }

    char* R_screen_line(byte _idx) {
      //            Debug.print(DBG_INFO, "R_idx:%d", _idx);
      strcpy_P(buffer, (char *)pgm_read_word(&(right_menu_table[_idx])));  // Necessary casts and dereferencing, just copy.
      return buffer;
    }

    void next_screen(byte _this_scr_selection) {
      //      Debug.print(DBG_INFO, "_this_scr_selection:%d", _this_scr_selection);
      if (this->cur_screen == 11) {
        this->cur_screen = 111;
      }
      if (this->cur_screen == 1) {
        this->cur_screen = 11;
      }
      if (this->cur_screen == 0) {
        this->cur_screen = 1;
      }
      this->update_screen();
    }

    char* get_cur_screen_selection_str() {
      char* _tmp_sec_str;
      bool r_screen_exist = false;
      byte scr_idx = this->screen_idx(this->cur_screen, r_screen_exist);
      _tmp_sec_str = this->screen_line(scr_idx + this->cur_selection);
      return _tmp_sec_str;
    }

    byte scr_exist(char* _str, byte _no_to_search) {
      char tmp_str[30];
      byte n = _no_to_search;
      strcpy(tmp_str, _str);
      strcpy(buffer, "scr");
      if (n < 100) strcat(buffer, "0"); //should be 001 or 112  (3 digits)
      if (n < 10) strcat(buffer, "0");
      sprintf(buffer, "%s%d", buffer, n);
      //      Debug.print(DBG_INFO, "tmp_str:%s ", tmp_str);
      //      Debug.print(DBG_INFO, "\t\t buf:%s", buffer);

      char* pch = strstr(tmp_str, buffer);
      bool bool_pch = (strcmp(pch, NULL)) ? 1 : 0;
      //      Debug.print(DBG_INFO, " pch:%s", pch);
      //      Debug.print(DBG_INFO, " blpch:%d", bool_pch);
      if (bool_pch == 1) { //when screen was found check how many lines in it
        pch = strstr(tmp_str, ",");
        int lines_number = this->num_b_commas(pch);
        //        Debug.print(DBG_INFO, " FOUND_pch:%s", pch);
        this->screen_lines_available = lines_number;
      }

      //now check if there is R screen
      bool R_bool_pch = false;
      if (bool_pch == 1) {
        char* R_pch = strstr(tmp_str, ",R");
        R_bool_pch = (strcmp(R_pch, NULL)) ? 1 : 0;
        //        Debug.print(DBG_INFO, " R_pch:%s", R_pch);
        //        Debug.print(DBG_INFO, " R_bool_pch:%d", R_bool_pch);
      }
      //      Debug.print(DBG_INFO, "\n");
      return (bool_pch + R_bool_pch);
    }

    byte R_scr_exist(char* _str, byte _no_to_search) {
      char tmp_str[30];
      byte n = _no_to_search;
      strcpy(tmp_str, _str);
      strcpy(buffer, "scr");
      if (n < 100) strcat(buffer, "0"); //should be 001 or 112  (3 digits)
      if (n < 10) strcat(buffer, "0");
      sprintf(buffer, "%s%d", buffer, n);
      //      Debug.print(DBG_INFO, "tmp_str:%s ", tmp_str);
      //      Debug.print(DBG_INFO, "\t\t buf:%s", buffer);

      char* pch = strstr(tmp_str, buffer);
      bool bool_pch = (strcmp(pch, NULL)) ? 1 : 0;
      //      Debug.print(DBG_INFO, " pch:%s", pch);
      //      Debug.print(DBG_INFO, " blpch:%d", bool_pch);
      if (bool_pch == 1) { //when screen was found check how many lines in it
        pch = strstr(tmp_str, ",");
        int lines_number = this->num_b_commas(pch);
        //        Debug.print(DBG_INFO, " FOUND_pch:%s", pch);
        this->screen_lines_available = lines_number;
      }

      return (bool_pch);
    }

    int num_b_commas(char* _str) {
      int tmp_num;
      int i, j;
      char tmp_str[10];
      for (i = 1; i < strlen(_str); i++) {
        if (_str[i] == ',') break;
      }
      //      Debug.print(DBG_INFO, "STR=%s len=%d i=%d\n", _str, strlen(_str), i);

      for (j = 0; j < i - 1; j++) {
        tmp_str[j] = _str[j + 1];
      }
      tmp_str[j] = '\0';
      tmp_num = atoi(tmp_str);
      //      Debug.print(DBG_INFO, "NEW_STR=%s NUM=%d\n", tmp_str, tmp_num);

      return tmp_num;
    }

    void save_screen_params() {
      byte scr = this->cur_screen;
      byte i = (scr > 10) ? ((scr > 100) ? 3 : 2) : 1;  //menu level
      prev_menu_selection[i] = this->cur_selection;
    }

    void set_prev_screen_params() {
      byte scr = this->cur_screen;
      byte i = (scr > 10) ? ((scr > 100) ? 3 : 2) : 1;  //menu level
      this->cur_selection = prev_menu_selection[i];
    }

    void Set_cur_screen(byte _screen) {
      this->cur_screen = _screen;
      if (_screen == 13) this->vars[0] = defaultProg;
    }

    byte Get_cur_screen() {
      return (this->cur_screen);
    }

    void Set_selection(byte _sel) {
      this->cur_selection = _sel;
      Debug.print(DBG_INFO, F("-> _sel:%d, screen_lines_available:%d\n")
                  , _sel, this->screen_lines_available);

      if (this->cur_selection < 1) this->cur_selection = this->screen_lines_available;
      if (this->cur_selection > this->screen_lines_available) this->cur_selection = 1;
      Debug.print(DBG_INFO, F("<- this->cur_selection:%d, screen_lines_available:%d\n"), this->cur_selection, this->screen_lines_available);

    }

    byte Get_selection() {
      return (this->cur_selection);
    }

    void Set_vars(uint8_t * _arr) {
      for (int i = 0; i < this->VAR_SIZE; i++) {
        this->vars[i] = _arr[i];
      }
    }

    void Set_vars_by_i(uint8_t _i, uint8_t _val, bool _limit = false) {
      this->vars[_i] = _val;
      if (_limit) {
        if (this->vars[_i] < 0 or this->vars[_i] >= 250) this->vars[_i] = this->var_max;
        if (this->vars[_i] > this->var_max) this->vars[_i] = var_min;
      }
    }

    void set_pressure(float _p) {
      this->pressure = _p;
    }

    uint8_t Get_vars_by_i(byte _i) {
      return (this->vars[_i]);
    }

    const uint8_t* Get_vars() const {
      return (this->vars);
    }

    void Reset_var() {
      for (int i = 0; i < this->VAR_SIZE; i++) this->vars[i] = NULL_VAL;
      for (int i = 0; i < 4; i++) this->time_vars[i] = NULL_VAL;
    }

    void Set_time_vars(uint8_t _t1, uint8_t _t2, uint8_t _t3, uint8_t _t4) {
      this->time_vars[0] = _t1;
      this->time_vars[1] = _t2;
      this->time_vars[2] = _t3;
      this->time_vars[3] = _t4;
    }

    const uint8_t* Get_time_vars() const {
      //      uint8_t time_v[2];
      //      time_v[0] = time_vars[0];
      //      time_v[1] = time_vars[1];
      return (this->time_vars);
    }

    void Set_PulseLen(uint16_t _OnVar, uint16_t _OffVar) {
      //      this->vars[1]=0;
      this->PulseOnLen = _OnVar;
      this->PulseOffLen = _OffVar;
    }

    void Set_pressure_target_and_step(float _trg, float _step) {
      this->pressure_target = _trg;
      this->step_length = _step;
    }

    uint8_t Get_PulseOnLen() {
      return (this->PulseOnLen);
    }


  private:
    uint8_t prev_menu_selection[5] = {0};
    uint8_t cur_screen = 99;
    uint8_t cur_selection = 1;
    uint8_t screen_lines_available = 2;
    uint8_t vars[VAR_SIZE];
    float pressure;
    float pressure_target;

    const uint8_t var_min = 0;
    const uint8_t var_max = 9;
    uint8_t time_vars[4];
    uint16_t PulseOnLen = 0;
    uint16_t PulseOffLen = 0;
    float step_length = 0;

}; //lcd_menu class

#ifdef LCD_ON
lcd_menu menu;
#endif

void lcd_setup()
{
  lcd.init();
  lcd.backlight();
  menu.init();
  // Menu initialization.

}

void menu_change(int _enc_move) {
  //  bool move_val;
  //  if (active_screen != 1) {
  //    if (_enc_move == ENC_UP) move_val = true;
  //    if (_enc_move == ENC_DOWN) move_val = false;
  //  }
  if (menu.Get_cur_screen() == 0) {
    main_screen();
  }
  else if (menu.Get_cur_screen() == 14) {

  }
  else {
    byte menu_screen = menu.Get_cur_screen();
    byte screen_selection = menu.Get_selection();
    uint8_t* menu_vars = menu.Get_vars();
    uint16_t menu_var = menu_vars[0];

    //  Debug.print(DBG_INFO, "menu_screen=%d, screen_selection=%d, _enc_move=%d\n",
    //  menu_screen, screen_selection, _enc_move);
    if (menu_screen != 99) {
      if (menu_screen == 13) {
        if (_enc_move == ENC_UP) menu.Set_vars_by_i(0, ++menu_var, true);
        if (_enc_move == ENC_DOWN) menu.Set_vars_by_i(0, --menu_var, true);
      } else {
        //        Debug.print(DBG_INFO, F("HERE screen_selection%d"), screen_selection);

        if (_enc_move == ENC_UP) menu.Set_selection(++screen_selection);
        if (_enc_move == ENC_DOWN) menu.Set_selection(--screen_selection);
      }
      menu.update_screen();
    }
  }
}

void main_screen() {
  Debug.print(DBG_INFO, F("--MAIN SCREEN--\n"));
  //  var_default[0] = defaultProg;
  menu.Reset_var();
//  menu.set_pressure(2.34);
  menu.Set_vars_by_i(0, defaultProg); // for Pressure value to kick in
  menu.Set_vars_by_i(1, defaultProg);
  menu.print_screen(0);
  timer.cancel(trig_timer);
  trig_timer = timer.every(10, check_trigs);
  pressure_check_timer = timer.every(200, pressure_update_timer);
}



#endif

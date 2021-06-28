struct FunctParameters {
  int16_t p_up_len;
  int16_t p_down_len;
};

void screen_center (char *_str);
void lcd_menu_setup();
void main_screen();
//void decide(byte _cur_screen, byte cur_selection, byte _button_click, int _var=-1);
void global_vars_init();
bool check_trigs();
bool shutoff_prog();
bool pulse_off();
bool pulse_on();
bool run_manual();
void serial_setup();
void pinMode_setup();
void ADC_setup();
void rot_setup();
void read_default_vars();
void button_check();
int enc_check();
void decide(uint8_t _cur_screen, uint8_t cur_selection, uint8_t _button_click, uint8_t _button_event, uint8_t* _vars);
void stop_program_now(bool _stop_all = false);
void write_vars_for_test(bool _zero=false);
//void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);

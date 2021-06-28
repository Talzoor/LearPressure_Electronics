#define LCD_ON
#define _EEPROMex_DEBUG
#include "include_h.h"

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
  button_check();

  if (run_program_comand) {
    run_program_comand = false;
    run_program_now_no_sensor();
  }
  if (stop_program_comand) {
    stop_program_comand = false;
    stop_program_now(true);
  }
#ifdef LCD_ON
  int enc_movement = encoder_loop();
#endif

  if (stringComplete) {
    do_serial_debug(enc_movement);
  }
} //loop

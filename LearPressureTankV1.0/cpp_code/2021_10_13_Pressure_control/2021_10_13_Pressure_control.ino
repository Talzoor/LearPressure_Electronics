#define LCD_ON
#define _EEPROMex_DEBUG
//#define ANALOG_POT_OVERIDE
#include "include_h.h"

void setup() {
  serial_setup();
  pinMode_setup();

#if PRESSURE_SENSOR_HARDWARE == Honeywell_ABP2
  while (0) { // analog pot check
    int a_read = analogRead(PIN_ANALOG_POT);
    float pot_pressure = mapf(a_read, 0, 1023, pmin, pmax);
    Serial.println("A0:" + (String)a_read + "\tp=" + String(pot_pressure));
    delay(100);
  }
#endif

  ADC_setup();
  timer_setup();
  button_setup();
  //  rot_setup();
  EEPROM_setup();
  read_default_vars();
  global_vars_init();
#ifdef LCD_ON
  lcd_setup();
  lcd_messages();
#endif
#if PRESSURE_SENSOR_HARDWARE == TE_MS5803_01BA or PRESSURE_SENSOR_HARDWARE == TE_MS5803_14BA
  MS_5803_setup();
#endif

  //  write_vars_for_test(true);
}

void loop() {
  timer.tick();
  button_check();

  if (run_program_comand) {
    run_program_comand = false;
    if (PressureSensor == 0) run_program_now_no_sensor(); else run_program_now_with_sensor();
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

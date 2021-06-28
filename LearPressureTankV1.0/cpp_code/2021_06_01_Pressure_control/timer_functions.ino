
void tick_func() {
  Debug.timestampOff();
  Debug.print(DBG_INFO, F("."));
  if (millis() % 10000 < 1000) Debug.print(DBG_INFO, F("\n"));
  Debug.timestampOn();
}

bool check_trigs() {

  bool trig_start = digitalRead(TRIG_IN_START_PIN);
  bool trig_stop = digitalRead(TRIG_IN_STOP_PIN);
  bool trig_p_up = digitalRead(TRIG_IN_P_UP_PIN);
  bool trig_p_down = digitalRead(TRIG_IN_P_DOWN_PIN);

  bool t_start_change = trig_start != (bool)trig_data[0];
  bool t_stop_change = trig_stop != (bool)trig_data[2];
  bool t_p_up_change = trig_p_up != (bool)trig_data[4];
  bool t_p_dn_change = trig_p_down != (bool)trig_data[6];

  uint8_t c_menu = menu.Get_cur_screen();
  uint8_t c_prog = menu.Get_vars_by_i(0);

  if (t_start_change) {     //check for any change
    Debug.print(DBG_INFO, "START_CHANGE\n");
    trig_data[0] = trig_start;    //0
    trig_data[1]++; //counter
    if (trig_data[1] >= 100) trig_data[1] = 0;
  }
  if (t_stop_change) {
    Debug.print(DBG_INFO, "STOP_CHANGE\n");
    trig_data[2] = trig_stop;     //2
    trig_data[3]++;
    if (trig_data[3] >= 100) trig_data[3] = 0;
  }
  if (t_p_up_change) {
    trig_data[4] = trig_p_up;     //4
    trig_data[5]++;
    if (trig_data[5] >= 100) trig_data[5] = 0;
  }
  if (t_p_dn_change) {
    trig_data[6] = trig_p_down;   //6
    trig_data[7]++;
    if (trig_data[7] >= 100) trig_data[7] = 0;
  }

  if (c_menu == 0 or c_menu == 90) {
    if (t_start_change == true) {
      if (prog_off_var == true) {
        init_prog_vars(c_prog);
        run_program_comand = true;
      }
    }
    if (t_stop_change) stop_program_comand = true;
    if (t_p_up_change) ;
    if (t_p_dn_change) ;
    return true;
  } //scr=0
  else if (c_menu == 14) {
    //    Debug.print(DBG_INFO, "SCR_14 UPDATE\n");
    for (int i = 0; i < 8; i++)
      var_default[i] = trig_data[i];
    menu.Set_vars(var_default);
    menu.scr14update();
    return true;
  } //scr=14
  //  Debug.print(DBG_INFO, "TIMER\n");

  return false;
} //check_trigs

void timer_setup() {
  //    timer.every(200, print_all_in);
  //  timer.every(1000, tick_func);
  //  trig_timer.every(10, check_trigs);  //every 10ms
  timer.cancel(trig_timer);
  timer.cancel(pulse_on_timer);
  timer.cancel(pulse_off_timer);
  timer.cancel(manual_timer);
  timer.cancel(program_offset_timer);
  timer.cancel(next_p_timer);
  timer.cancel(pressure_check_timer);
  timer.cancel(check_pressure_timer);
  timer.cancel(stop_pressure_fill_timer);
  timer.cancel(pressure_step_wait_timer);

}

bool shutoff_prog() {
  bool ret_val = false;
  stop_program_now();
  return ret_val;
}

bool pressure_update_timer() {
  bool ret_val = true;
  current_pressure_sensor = check_pressure_sensor();
  menu.set_pressure(current_pressure_sensor);
  menu.update_pressure();
  return ret_val;
}

bool pulse_off() { //(void *prog_pulse_len_ms, void *prog_pulse_down_ms) {
  bool ret_val = false;
  Debug.print(DBG_INFO, F("P_OFF"));
  //  timer.cancel(pulse_off_timer);
  pulse_off_now();
  if (prog_off_var == false)
    pulse_on_timer = timer.in(params.p_down_len, pulse_on);
  return ret_val;
}

bool pulse_on() { //void *prog_pulse_len_ms, void *prog_pulse_down_ms) {
  bool ret_val = false;
  Debug.print(DBG_INFO, F("P_ON"));
  //  timer.cancel(pulse_on_timer);
  if (prog_off_var == false) {
    pulse_on_now();
    pulse_off_timer = timer.in(params.p_up_len, pulse_off);
  } else {
    pulse_off_now();
  }
  return ret_val;
}

bool run_manual() {

}

//bool start_prog(void* prog_pulse_len_ms, void *prog_pulse_down_ms) {
bool start_prog() {
  Debug.print(DBG_INFO, F("------------- PROG_START ------------- \n"));
  //  timer.cancel(program_offset_timer);
  prog_off_var = false;
  pulse_on_now();
  Debug.print(DBG_INFO, F("p_up_len:%d\n"), params.p_up_len);
  pulse_off_timer = timer.in(params.p_up_len, pulse_off);
  timer.cancel(program_offset_timer);
  return false;
}

bool start_prog_pressure() {
  Debug.print(DBG_INFO, F("------------- PROG_START_pressure ------------- \n"));
  //  timer.cancel(program_offset_timer);
  prog_off_var = false;
  current_pressure_goal = (pressure_target > current_pressure_sensor) ? \
                          current_pressure_sensor + pressure_step_atm : \
                          current_pressure_sensor - pressure_step_atm;
  current_pressure_goal = constrain(current_pressure_goal, 0, pressure_target);
  check_pressure_and_decide();
  //  current_pressure_goal = pressure_target; //+= pressure_step_atm;
  //  go_to_pressure(current_pressure_goal);

  //  pulse_on_now();
  //  Debug.print(DBG_INFO, F("p_up_len:%d\n"), params.p_up_len);
  //  pulse_off_timer = timer.in(params.p_up_len, pulse_off);
  timer.cancel(program_offset_timer);
  return false;
}

bool out_finish_signal_off() {
  Debug.print(DBG_INFO, F("PROG FINISHED --- PULSE OUT OFF \n"));
  analogWrite(FINISH_OUT_SIGNAL_PIN, PWM_LOW);
}

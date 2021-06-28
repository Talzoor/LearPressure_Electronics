void go_to_pressure(float _current_pressure_goal) {
  float pressure_goal = _current_pressure_goal;
  pulse_on_now();
  stop_pressure_fill_timer = timer.in(basic_ms_for_pulse, stop_and_rest);

  timer.cancel(program_offset_timer);

}

void stop_and_rest() {
  pulse_off_now();
  check_pressure_timer = timer.in(rest_ms, check_pressure_and_decide);
  timer.cancel(stop_pressure_fill_timer);
}

void check_pressure_and_decide() {
  float pressure_now = check_pressure_sensor();
  prog_in_out = (pressure_now < current_pressure_goal) ? FILL_AIR : RELIEF_AIR;
  if (!pressure_in_range(pressure_now, current_pressure_goal)) {
    go_to_pressure(current_pressure_goal);
  } else {
    if (pressure_in_range(pressure_now, pressure_target)) {
      // do nothing -- stop?
    } else {
      current_pressure_goal = (pressure_target > pressure_now) ? current_pressure_goal + pressure_step_atm : current_pressure_goal - pressure_step_atm;
      go_to_pressure(current_pressure_goal);
    }
  }
  char bf1[10], bf2[10], bf3[10];
  Debug.print(DBG_INFO, F("pressure_target:%s pressure_now:%s current_pressure_goal:%s\n"), \
              dtostrf(pressure_target, 2, 2, bf1), \
              dtostrf(pressure_now, 2, 2, bf2), \
              dtostrf(current_pressure_goal, 2, 2, bf3));

  timer.cancel(check_pressure_timer);
}

float check_pressure_sensor() {
  //  float real_pressure = ; // sensor read
  float real_pressure = current_pressure_sensor; //for check
  if (real_pressure >= 1.2 or real_pressure < 0.0) p_inc *= -1;
  real_pressure += p_inc;
  return real_pressure;
}

bool pressure_in_range(float _p_now, float _curr_p_goal) {
  bool tmp_val = true;
  if (abs(_p_now - _curr_p_goal) > pressure_range_ok) tmp_val = false;
  return tmp_val;
}

bool m_update() {
  menu.update_screen();
}

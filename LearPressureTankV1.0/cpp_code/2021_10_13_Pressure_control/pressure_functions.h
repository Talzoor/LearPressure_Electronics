bool go_to_pressure() {
  float pressure_goal = current_pressure_goal;
  pressure_update_on_rest_timer();
  pulse_on_now();
  int _pulse = how_far_pressure(pressure_goal); //will return high, mid or low
  stop_pressure_fill_timer = timer.in(_pulse, stop_and_rest);

  timer.cancel(program_offset_timer);
  return false;
}

unsigned long how_far_pressure(float _p_goal) {
  unsigned long tmp_val = ms_low_pulse;
  float pressure_now = current_pressure_sensor; //check_pressure_sensor();
  float abs_dist = fabs(pressure_now - _p_goal);
  if (abs_dist > 0.2) {
    tmp_val = abs_dist * ms_weight * 1.0;
    //    tmp_val = ms_high_pulse;
  } else if (abs_dist > 0.1) {
    tmp_val = abs_dist * ms_weight * 0.95;
    //    tmp_val = ms_mid_pulse;
  } else {
    tmp_val = abs_dist * ms_weight * 0.92;
  }

  if (prog_in_out == RELIEF_AIR  ) {
    if (pressure_now < 0.25) {
      tmp_val *= 2.6; // going much slower when air goes out on low pressure
    } else if (pressure_now < 0.35) {
      tmp_val *= 3;
    } else if (pressure_now < 0.8) {
      tmp_val *= 2;
    }
  }

  char bf1[10], bf2[10], bf3[10], bf4[10];
  Debug.print(DBG_INFO, F("_p_goal:%s, pressure_now:%s, abs_dist:%s, tmp_val:%s\n"), \
              dtostrf(_p_goal, 2, 2, bf1), \
              dtostrf(pressure_now, 2, 2, bf2), \
              dtostrf(abs_dist, 2, 2, bf3), \
              dtostrf(tmp_val, 2, 2, bf4));

  return tmp_val;
}

void stop_and_rest() {
  pulse_off_now();
  check_pressure_timer = timer.in(rest_ms, check_pressure_and_decide);
  timer.cancel(stop_pressure_fill_timer);
}

bool check_func() {
  Debug.print(DBG_INFO, F("-----CHECK-----\n"));

  return false;
}

void check_pressure_and_decide() {
  float pressure_now = current_pressure_sensor; //check_pressure_sensor();
  bool go_to_p_flag = false;
  bool program_end = false;

  if (!pressure_in_range(pressure_now, current_pressure_goal)) { // small step
    go_to_p_flag = true;
  } else {
    if (pressure_in_range(pressure_now, pressure_target)) { // main pressure goal
      // do nothing -- stop?
      Debug.print(DBG_INFO, F("-----GOAL ACHIEVED----- STOP PROG\n"));
      if (pressure_repeat > 0 and pressure_next_prog != 10) { // if there is next prog and repeat more than 0
        prog_to_run = pressure_next_prog;
        if (pressure_main_prog_runing == prog_to_run) pressure_repeat--;
        if (pressure_repeat != 0) {
          run_program_with_sensor_next_prog();
        } else {
          program_end = true;
        }
      } else {
        program_end = true;
      }
    } else {
      current_pressure_goal = (pressure_target > pressure_now) ? \
                              current_pressure_goal + pressure_step_atm : \
                              current_pressure_goal - pressure_step_atm;
      current_pressure_goal = (pressure_target > pressure_now) ? \
                              constrain(current_pressure_goal, 0, pressure_target) : \
                              constrain(current_pressure_goal, pressure_target, pressure_now);
      //      go_to_p_flag = true;
      timer.cancel(stop_pressure_fill_timer);
      //      pressure_step_wait_timer = timer.in(pressure_time_per_step * 1000, check_func);
      pressure_step_wait_timer = timer.in(pressure_time_per_step * 1000, go_to_pressure);

      Debug.print(DBG_INFO, F("-----STEP WAIT----- pressure_step_atm:%d[sec]\n"), pressure_time_per_step);
      strcpy_P(run_status, run_pr_step_rest);
      menu.update_screen();
    }

    if (program_end) {
      strcpy_P(run_status, run_pr_fin);
      menu.update_screen();
    }
  }
  prog_in_out = (current_pressure_goal > pressure_now) ? FILL_AIR : RELIEF_AIR;
  //  Debug.print(DBG_INFO, "prog_in_out:%d\n", prog_in_out);
  if (go_to_p_flag) {
    go_to_p_flag = false;
    go_to_pressure();
  }

  char bf1[10], bf2[10], bf3[10], msg[100];
  sprintf(msg, "pressure_target:%s pressure_now:%s current_pressure_goal:%s\n", \
          dtostrf(pressure_target, 3, 2, bf1), \
          dtostrf(pressure_now, 2, 3, bf2), \
          dtostrf(current_pressure_goal, 2, 3, bf3));
  Log(LOG_LEVEL_INFO, msg, false);
  CSV(pressure_now);
  Debug.print(DBG_INFO, msg);

  timer.cancel(check_pressure_timer);
}

float check_pressure_sensor() {
  float real_pressure = read_pressure_sensor(); // sensor read
  //  if (real_pressure >= 1.2 or real_pressure < 0.0) p_inc *= -1;
  //  real_pressure += p_inc;
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

#if PRESSURE_SENSOR_HARDWARE == Honeywell_ABP2
float read_pressure_sensor() {
  Wire.beginTransmission(pressure_id);
  int stat = Wire.write (cmd, 3); // write command to the sensor
  stat |= Wire.endTransmission();
  delay(10);
  Wire.requestFrom(pressure_id, 7); // read back Sensor data 7 bytes
  int i = 0;
  for (i = 0; i < 7; i++) {
    data[i] = Wire.read();
  }
  float press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  float temp_counts = data[6] + data[5] * 256 + data[4] * 65536; // calculate digital temperature counts
  float temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
  float percentage = (press_counts / 16777215) * 100; // calculate pressure as percentage of full scale
  float pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin; //calculation of pressure value according to equation 2 of datasheet

  dtostrf(press_counts, 4, 1, cBuff);
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);
  sprintf(printBuffer, "Status Register, 24 - bit Sensor data, Digital Pressure Counts, Percentage of full scale pressure, Pressure Output, Temperature");
  //    Serial.println(printBuffer);
  sprintf(printBuffer, "%x\t%2x %2x %2x\t%s\t%s\t%s\t%s", data[0], data[1], data[2], data[3], cBuff, percBuff, pBuff, tBuff);
  //    Serial.println(printBuffer);
  if (percentage > 99.0) pressure = -1.0;

#ifdef ANALOG_POT_OVERIDE // simulate pressure with analog pot (A0)
  int a_read = analogRead(PIN_ANALOG_POT);
  pressure = mapf(a_read, 0, 1023, pmin, pmax);
#endif

  return pressure;
}// end read_pressure_sensor()

#elif PRESSURE_SENSOR_HARDWARE == TE_MS5803_01BA

void MS_5803_setup() {
  if (sensor.initializeMS_5803()) {
    Serial.println( "MS5803 CRC check OK." );
  }
  else {
    Serial.println( "MS5803 CRC check FAILED!" );
  }
}

float read_pressure_sensor() {
  sensor.readSensor();
  /*
    // Show pressure
    Serial.print("Pressure = ");
    Serial.print(sensor.pressure());
    Serial.println(" mbar");

    // Show temperature
    Serial.print("Temperature = ");
    Serial.print(sensor.temperature());
    Serial.println("C");
  */

  return sensor.pressure();
}// end read_pressure_sensor()

#elif PRESSURE_SENSOR_HARDWARE == TE_MS5803_14BA

void MS_5803_setup() {
  sensor.reset();
  sensor.begin();
  pressure_baseline = sensor.getPressure(ADC_4096);
}

double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return (P / pow(1 - (A / 44330.0), 5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}

double baseline_pressure(double P, double P0)
{
  return (P - P0);
}

float read_pressure_sensor() { // acc. to https://github.com/sparkfun/SparkFun_MS5803-14BA_Breakout_Arduino_Library/blob/master/examples/SparkFun_MS5803_Demo/SparkFun_MS5803_Demo.ino
  //Create variables to store results
  float temperature_c, temperature_f;
  double pressure_abs, pressure_relative, altitude_delta, pressure_delta;

  // Read temperature from the sensor in deg C. This operation takes about
  temperature_c = sensor.getTemperature(CELSIUS, ADC_512);

  // Read temperature from the sensor in deg F. Converting
  // to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);

  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);

  // Let's do something interesting with our data.

  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.

  //double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)
  //pressure_relative = sealevel(pressure_abs, base_altitude);

  // Taking our baseline pressure at the beginning we can find an approximate
  // change in altitude based on the differences in pressure.
  altitude_delta = altitude(pressure_abs , pressure_baseline);
  pressure_delta = baseline_pressure(pressure_abs , pressure_baseline);


  float pressure_abs_atm = pressure_abs * 0.000986923;
  float pressure_baseline_atm = pressure_delta * 0.000986923;
/*
  // Report values via UART
  Serial.print("Temperature C = ");
  Serial.println(temperature_c);

  Serial.print("Temperature F = ");
  Serial.println(temperature_f);

  Serial.print("Pressure abs (mbar)= ");
  Serial.println(pressure_abs);

  Serial.print("Pressure abs (atm)= ");
  Serial.println(pressure_abs_atm);

  Serial.print("Pressure relative (mbar)= ");
  Serial.println(pressure_relative);

  Serial.print("Pressure relative baseline (mbar)= ");
  Serial.println(pressure_delta);

  Serial.print("Altitude change (m) = ");
  Serial.println(altitude_delta);

  Serial.println(" ");//padding between outputs
*/
  return pressure_baseline_atm;
}// end read_pressure_sensor()

#endif

void pressure_update_on_rest_timer() {
  current_pressure_sensor = check_pressure_sensor();
  // more code here?
}

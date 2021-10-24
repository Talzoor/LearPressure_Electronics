void Log_init() {
  sd_file_no = sd_file_no_func(); // global
  Serial.println("file no. to write - " + String(sd_file_no));
  build_file_name(sd_file_name);
  //  strcpy(sd_file_name, file_name);
  logger_time_start = millis(); // time init
  logger_timestamp = true; // just in case
  log_open = true;

  char buff[40];
  sprintf(buff, "LOG START - file:%s\n", sd_file_name);
  Log(LOG_LEVEL_INFO, buff);
  if (write_csv) CSV(current_pressure_sensor);
}

void Log_close() {
  log_open = false;
}

void Log(uint8_t level, const char* message, bool echo)
{
  char buffer[120];
  strcpy(buffer, ""); //init
  if (logger_timestamp) timestamp_func(buffer);
  char msg[100], msg2[100];
  sprintf(msg, "[%s] %s", LOG_LEVEL_STR[level], message);
  sprintf(msg2, "%s->%s", buffer, msg);
  if (log_open) sd_append_to_file(sd_file_name, msg2);
  if (echo) Serial.println("log echo:" + (String)msg2);
}

void CSV(float _pressure_now) {
  char sd_csv_file_name[40];
  char timestamp[40];
  strcpy(timestamp, ""); //init
  timestamp_func(timestamp);
  sprintf(sd_csv_file_name, "%s_%d%s", "csv", sd_file_no, ".csv"); // filename ie. "csv_76.csv"
  if (log_open and write_csv) sd_append_to_csv(sd_csv_file_name, timestamp, _pressure_now);
}

void timestamp_func(char* str) {
  unsigned long ms = millis();
  //  char str[30]; // max for this timestamp
  unsigned long ms_diff = ms - logger_time_start;
  sprintf(str, "%lu", ms_diff);
}

void build_file_name(char* _str) {
  char buff[40];
  sprintf(buff, "%s_%d%s", LOGGER_FILE_PREFIX, sd_file_no, LOGGER_FILE_SUFFIX); // filename ie. "log_76.txt"
  strcpy(_str, buff);
}

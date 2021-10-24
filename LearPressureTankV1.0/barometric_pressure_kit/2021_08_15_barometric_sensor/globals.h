const bool SERIAL_ECHO = false;

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;

//float ema_p = 0.0, ema_ema_p = 0.0, ema_p_alpha = 0.05;
//float ema_h = 0.0, ema_ema_h = 0.0, ema_h_alpha = 0.05;
float alt_params[3] = {0.0, 0.0, 0.001};
//float temp_params[3] = {0.0, 0.0, 0.05};

const unsigned long calibrate_time = 1000L * 10L;
float calibrated_h = -999;

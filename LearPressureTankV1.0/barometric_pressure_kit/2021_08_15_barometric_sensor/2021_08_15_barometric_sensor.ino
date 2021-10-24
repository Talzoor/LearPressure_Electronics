#include "include.h"

int i = 0, j = 0;

void setup() {
  Serial.begin(115200);
  lcd_setup();
  float p = bmp_setup();
  //  filter_setup_p_h(p, Adafruit_BMP085_readAltitude(p));
  filter_setup_alt(alt_params, Adafruit_BMP085_readAltitude(p));
}

void loop() {
  float pressure = bmp.readPressure();
  float altitude = Adafruit_BMP085_readAltitude(pressure);
  float filtered_alt = filter_data(alt_params, altitude);

  //  print_lcd_text("P:" + String((long int)pressure) + "Pa", "h:" + String(altitude) + "m");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  //  Serial.print("Real altitude = ");
  //  Serial.print(bmp.readAltitude(101500));
  //  Serial.println(" meters");

  unsigned long currentMillis = millis();
  if (currentMillis < calibrate_time) {
    int count_down = (unsigned long)(calibrate_time - currentMillis) / 1000;
    print_lcd_text("Calibrate", count_down);
    calibrated_h = filtered_alt;
  } else {
    print_lcd_text("h: " + String(filtered_alt) + "m", "hf:" + String(filtered_alt - calibrated_h) + "m");
  }

  if (currentMillis - previousMillis >= interval) {
    //    Serial.println("Temperature = " + String(bmp.readTemperature()) + "*C\n" );
    //    s_print_f("Pressure = ", pressure, " Pa\t");
    //    s_print_f("Altitude = ", Adafruit_BMP085_readAltitude(pressure), " meters\n");
    s_print_f("Altitude = ", Adafruit_BMP085_readAltitude(pressure), "\t");
    s_print_f("filtered = ", filtered_alt, "\t");
    s_print_f("calibrated = ", filtered_alt - calibrated_h, "\n");

    //    s_print_f("cu-cal = ", count_down , "\n");
    //    Serial.println("Altitude = " + String(Adafruit_BMP085_readAltitude()) + " meters");
    //    Serial.println("Pressure at sealevel (calculated) = " + String(bmp.readSealevelPressure()) + " Pa");
    //    Serial.println();
  }

}

// Function calculation of pressure altitude
long calcPressureAltitude(long _PressureInPascal) {
  long _Temp = (log(101325.0 / _PressureInPascal) * 72383.0);
  long _PressureAltitude = 0;
  if (_Temp % 10 > 5) {
    _PressureAltitude = 1;
  }
  _PressureAltitude = _PressureAltitude + long(_Temp / 10);
  return _PressureAltitude;
}

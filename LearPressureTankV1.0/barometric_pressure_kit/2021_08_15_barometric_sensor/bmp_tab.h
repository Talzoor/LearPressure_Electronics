#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

float Adafruit_BMP085_readAltitude(float pressure = bmp.readPressure(), float sealevelPressure = 101325); // std atmosphere

float bmp_setup() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    print_lcd_text("!!!!!!!!!!!!!!!!!!!!\nBMP085 sensor", "failed, check wiring\n!!!!!!!!!!!!!!!!!!!!", 1);
    delay(5000);
    //while (1) {}
  }
  return (bmp.readPressure());
}

float Adafruit_BMP085_readAltitude(float pressure, float sealevelPressure) {
  float altitude;
  //  if (pressure == -1) pressure = bmp.readPressure();

  altitude = 44330 * (1.0 - pow(pressure / sealevelPressure, 0.1903));

  return altitude;
}

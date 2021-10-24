#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_lcd_text(String _str1, String _str2 = "", int _txt_size = 2);
void print_lcd_text(String _str1, int _str2 = -65000, int _txt_size = 2);
void print_lcd_text(int _str1, int _str2 = -65000, int _txt_size = 2);
void print_lcd_text(int _str1, String _str2 = "", int _txt_size = 2);

void lcd_setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
}

void print_lcd_text(String _str1, String _str2, int _txt_size) {
  display.clearDisplay();

  display.setTextSize(_txt_size); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(_str1);
  display.println(_str2);
  display.display();      // Show initial text

  if (SERIAL_ECHO) {
    Serial.println("[LCD] str1:" + String(_str1) + "\tstr2:" + String(_str2));
  }
}

void print_lcd_text(String _str1, int _str2, int _txt_size) {
  print_lcd_text(_str1, String(_str2), _txt_size);
}
void print_lcd_text(int _str1, int _str2, int _txt_size) {
  print_lcd_text(String(_str1), String(_str2), _txt_size);
}
void print_lcd_text(int _str1, String _str2, int _txt_size) {
  print_lcd_text(String(_str1), _str2, _txt_size);
}

#include "prototypes.h"
#include "src/Arduino_DebugUtils_local/src/Arduino_DebugUtils.h"

#ifdef LCD_ON
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#endif

#include "src/arduino-timer_local/src/arduino-timer.h"

#include "src/EEPROMEx_local/EEPROMex.h"
#include "src/EEPROMEx_local/EEPROMVar.h"
#include "EEPROM_tab.h"
#include "globals.h"
#include "LCD_menu.h"
#include "LCD.h"
#include "src/RotaryEncoder_local/Encoder.h"
#include "src/AceButton_local/src/AceButton.h"
#include "buttons.h"
#include "serial_func.h"


//#include <LibPrintf.h>

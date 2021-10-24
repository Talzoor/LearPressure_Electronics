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
#include "SD_card.h"

#include "LCD_menu.h"
#include "LCD.h"
#include "src/RotaryEncoder_local/Encoder.h"
#include "src/AceButton_local/src/AceButton.h"
#include "buttons.h"
#include "serial_func.h"

#if PRESSURE_SENSOR_HARDWARE == TE_MS5803_01BA
#include "src/MS5803_01/MS5803_01.h"
MS_5803 sensor = MS_5803(512);
#elif PRESSURE_SENSOR_HARDWARE == TE_MS5803_14BA
#include "src/SparkFun_MS5803_14BA_I2C/SparkFun_MS5803_14BA_I2C.h"
//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77
MS5803 sensor(ADDRESS_HIGH);
#endif

#include "pressure_functions.h"
#include "logger.h"

//#include <LibPrintf.h>

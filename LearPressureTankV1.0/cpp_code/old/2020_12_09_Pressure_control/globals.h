
#define RotaryEncPinA 2
#define RotaryEncPinB 3
#define RotaryEncButton 4

const int BUTTON[4] = {29, 30, 31, 4};   //up, down, left, 4=encoder

#define ENC_UP 1
#define ENC_DOWN 2

#define SCREEN_EXIST 1
#define SCREEN_EXIST_W_R 2

#define NO_CLICK 0
#define BUTTON_LEFT 31
#define BUTTON_UP 29
#define BUTTON_DOWN 30
//#define BUTTON_MID 4
//#define BUTTON_RIGHT 9
#define BUTTON_ENC 4

#define TRIG_IN_START_PIN 16
#define TRIG_IN_STOP_PIN 17
#define TRIG_IN_P_UP_PIN 12
#define TRIG_IN_P_DOWN_PIN 11

#define FINISH_OUT_SIGNAL_PIN 6
#define PWM_HIGH 255
#define PWM_LOW 0
#define FINISH_P_OUT_SIGNAL_LEN_MS 1000

#define PIN_IN_SOL 24
#define PIN_OUT_SOL 25

#define ON_VAL  255
#define OFF_VAL 0

#define NULL_VAL 255

#define FILL_AIR 0
#define RELIEF_AIR 1

#define PAUSE_BETWEEN_PROGS  2 //10*1000   // 10 sec or 2ms (None)

/*
  volatile uint8_t aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
  volatile uint8_t bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
  volatile uint8_t encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
  volatile uint8_t oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
  volatile uint8_t reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
*/
long encOldPosition  = -999;

volatile bool prog_off_var = true;
volatile bool run_program_comand = false;
volatile bool stop_program_comand = false;
volatile FunctParameters params;

volatile uint8_t button_click;
volatile uint8_t button_event;

volatile uint8_t prog_to_run = 0;
volatile uint32_t prog_time = 0;
volatile uint8_t prog_pulse_len = 0;
volatile uint8_t prog_in_out = FILL_AIR;
volatile uint8_t prog_next = 10;          //  11=No next, if 10=same prog in rev,
//                                            set to 101 for one more time, 104 for 4 more times.
//                                            100 will stop.
uint8_t prog_run_x_times = 0;

//RotaryEncoder encoder(RotaryEncPinA, RotaryEncPinB);

bool dummy_func() {

}

auto timer = timer_create_default(); // create a timer with default settings
auto trig_timer = timer.every(0, dummy_func);
auto pulse_on_timer = timer.every(0, dummy_func);
auto pulse_off_timer = timer.every(0, dummy_func);
auto manual_timer = timer.every(0, dummy_func);
auto program_offset_timer = timer.every(0, dummy_func);
auto program_shutoff_timer = timer.every(0, dummy_func);
auto next_p_timer = timer.every(0, dummy_func);
auto pulse_out_timer = timer.every(0, dummy_func);


volatile uint8_t trig_data[8] = {0, 0, 0, 0, 1, 0, 1, 0};

//auto trig_timer = timer_create_default();
//byte active_screen = 1;

//static char glob_str[20];

//const char txt1[] PROGMEM = "Pressure Control";
//const char txt2[] PROGMEM = "      V1.0      ";
//const char txt3[] PROGMEM = "PROGRAM ";
//const char txt4[] PROGMEM = "    UP";
//const char txt5[] PROGMEM = "Ready       DOWN";
//const char txt6[] PROGMEM = "1.Configuration";
//const char txt7[] PROGMEM = "2.Manual Op.";
//const char txt8[] PROGMEM = "3.Prog. check";
//const char txt9[] PROGMEM = "1.Set program";
//const char txt10[] PROGMEM = "2.Default prog.";

volatile uint8_t var_default[10];
static char buffer[20];
bool t_prog_sec = false;
char run_status[20];

EEPROMVar<uint8_t> defaultProg(0);
EEPROMVar<uint16_t> ManPulseLen(0);
EEPROMVar<uint8_t> ManFullDownPLenMin(0);
EEPROMVar<uint8_t> ManFullUpPLenMin(0);
EEPROMVar<uint8_t> ManFullDownPLenSec(0);
EEPROMVar<uint8_t> ManFullUpPLenSec(0);

EEPROMVar<uint8_t> ProgTimeMin[10](0);
EEPROMVar<uint8_t> ProgTimeSec[10](0);
EEPROMVar<uint8_t> ProgInOut[10](0);
EEPROMVar<uint8_t> ProgNextProg[10](0);
EEPROMVar<uint8_t> ProgRepeat[10](0);
EEPROMVar<uint16_t> ProgPulseOnLen[10](0);
//EEPROMVar<uint8_t> ProgRatioUpDn[10](0);
EEPROMVar<uint16_t> ProgPulseOffLen[10](0);
EEPROMVar<uint8_t> ProgTrigOffset[10](0);
EEPROMVar<uint8_t> PressureSensor(0);


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

//unsigned long previousMillis = 0;
//const long interval = 10;

const char string_00[] PROGMEM = "scr000,2,R"; // screen 0, 2 lines ahead R for right as well
const char string_01[] PROGMEM = "P=";
const char string_02[] PROGMEM = "Pr";

const char string_34[] PROGMEM = "scr090,2,R"; // screen 0, 2 lines ahead R for right as well
const char string_35[] PROGMEM = "Pr";
const char string_36[] PROGMEM = "";

const char string_03[] PROGMEM = "scr001,4";
const char string_04[] PROGMEM = "Configuration";
const char string_05[] PROGMEM = "Manual op.";
const char string_06[] PROGMEM = "Prog. Check";
const char string_061[] PROGMEM = "Trig check";

const char string_07[] PROGMEM = "scr011,3,R";
const char string_08[] PROGMEM = "Set program";
const char string_09[] PROGMEM = "Default prog";
const char string_091[] PROGMEM = "P.sensor";

const char string_10[] PROGMEM = "scr012,3,R";
const char string_11[] PROGMEM = "Fill";
const char string_12[] PROGMEM = "Relief";
const char string_13[] PROGMEM = "Settings";

const char string_14[] PROGMEM = "scr013,1,R";
const char string_15[] PROGMEM = "Prog";

const char string_16[] PROGMEM = "scr123,3,R";
const char string_17[] PROGMEM = "Pulse[ms]";
const char string_18[] PROGMEM = "Relief";
const char string_19[] PROGMEM = "Fill";

const char string_20[] PROGMEM = "scr099,2";
const char string_21[] PROGMEM = "Pressure Control";
const char string_22[] PROGMEM = "      V1.1      ";

const char string_23[] PROGMEM = "scr111,10,R";
const char string_24[] PROGMEM = "PROG";
const char string_25[] PROGMEM = "T[m:s]";    // 60sec
const char string_251[] PROGMEM = "In/Out";
const char string_252[] PROGMEM = "NextProg";
const char string_253[] PROGMEM = "Repeat";
const char string_26[] PROGMEM = "OPEN[ms]";   // 1sec
const char string_27[] PROGMEM = "CLOSED[ms]";
const char string_28[] PROGMEM = "TrgOfst[s]";
const char string_29[] PROGMEM = "Save prog";
const char string_30[] PROGMEM = "Save all";

const char string_37[] PROGMEM = "scr112,9,R";
const char string_38[] PROGMEM = "PROG";
const char string_39[] PROGMEM = "Ps.trgt";
const char string_40[] PROGMEM = "Step";
const char string_41[] PROGMEM = "Time/stp";
const char string_42[] PROGMEM = "PrgOfst[s]";
const char string_43[] PROGMEM = "NextProg";
const char string_44[] PROGMEM = "Repeat";
const char string_45[] PROGMEM = "Save prog";
const char string_46[] PROGMEM = "Save all";

const char string_31[] PROGMEM = "scr014,2";
const char string_32[] PROGMEM = "Start    pUp";
const char string_33[] PROGMEM = " Stop    pDn";

const char run_pr_offset[] PROGMEM = "->Ofst";
const char run_pr_step_rest[] PROGMEM = "->StepRest";
const char run_pr_in[] PROGMEM = "->ON";
const char run_pr_out[] PROGMEM = "->OFF";
const char run_pr_fin[] PROGMEM = "->FINISH";
const char run_pr_fin_and_next[] PROGMEM = ">F>NEXT_P";


const char *const menu_table[] PROGMEM = {string_00, string_01, string_02, string_03, string_04, string_05, string_06, string_061,
                                          string_07, string_08, string_09, string_091, string_10, string_11, string_12, string_13,
                                          string_14, string_15, string_16, string_17, string_18, string_19, string_20,
                                          string_21, string_22, string_22, string_23, string_24, string_25, string_251, string_252, string_253,
                                          string_26, string_27, string_28, string_29, string_30, string_31, string_32, string_33,
                                          string_34, string_35, string_36,
                                          string_37, string_38, string_39, string_40, string_41, string_42, string_43, string_44, string_45, string_46
                                         };

const byte arrow_down[8] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
};
const byte arrow_up[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
};
const byte selection_sign[8] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
};
const byte round_sign[8] = {
  B00100,
  B00110,
  B11111,
  B10110,
  B10100,
  B10001,
  B11011,
  B01110
};

byte  empty_square[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

byte full_square[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};



const char right_string_0[] PROGMEM = "scr000,2";
const char right_string_1[] PROGMEM = "+";
const char right_string_2[] PROGMEM = "-";

const char right_string_3[] PROGMEM = "scr013,2";
const char right_string_4[] PROGMEM = "Run";
const char right_string_5[] PROGMEM = "Stop";

const char right_string_6[] PROGMEM = "scr011,3";
const char right_string_7[] PROGMEM = "";
const char right_string_8[] PROGMEM = "";
const char right_string_81[] PROGMEM = "";

const char right_string_9[] PROGMEM = "scr012,3";
const char right_string_10[] PROGMEM = "Run";
const char right_string_11[] PROGMEM = "Stop";
const char right_string_12[] PROGMEM = "";

const char right_string_13[] PROGMEM = "scr123,3";
const char right_string_14[] PROGMEM = "";
const char right_string_15[] PROGMEM = "";
const char right_string_16[] PROGMEM = "";

const char right_string_17[] PROGMEM = "scr111,10";
const char right_string_18[] PROGMEM = "";
const char right_string_19[] PROGMEM = "";
const char right_string_20[] PROGMEM = "";
const char right_string_21[] PROGMEM = "";
const char right_string_22[] PROGMEM = "";
const char right_string_23[] PROGMEM = "";
const char right_string_24[] PROGMEM = "";
const char right_string_241[] PROGMEM = "";
const char right_string_242[] PROGMEM = "";
const char right_string_243[] PROGMEM = "";


const char right_string_25[] PROGMEM = "scr090,2";
const char right_string_26[] PROGMEM = "";
const char right_string_27[] PROGMEM = "Stop";




const char OK_str[] PROGMEM = "OK";
const char *const string_OK[] PROGMEM = {OK_str};


//const char left_string_1[] PROGMEM = arrow_down;
//const char left_string_2[] PROGMEM = "";

const char *const right_menu_table[] PROGMEM = {right_string_0, right_string_1, right_string_2, right_string_3, right_string_4
                                                , right_string_5, right_string_6, right_string_7, right_string_8, right_string_81, right_string_9
                                                , right_string_10, right_string_11, right_string_12, right_string_13, right_string_14
                                                , right_string_15, right_string_16, right_string_17, right_string_18, right_string_19
                                                , right_string_20, right_string_21, right_string_22, right_string_23, right_string_24
                                                , right_string_241, right_string_242, right_string_243
                                                , right_string_25, right_string_26, right_string_27
                                               };




//#define WELCOME_SCR 99
//#define INFO_SCR 0
//#define MAIN_MENU_SCR 1
//#define CONF_SCR 11
//#define SET_PROG_SCR 111
//#define MAN_SCR 12
//#define MAN_SEL_LEN_SCR 121
//#define PROG_SCR 13

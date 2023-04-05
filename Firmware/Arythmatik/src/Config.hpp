//#define DebugMode  //Uncomment for debug mode

// -----------------  EEPROM  -----------------
#define EEPROM_TYPE 20 // Start at EEProm 20 as gate is using 1-13
#define EEPROM_INVERT 21


// -----------------  PINS  -----------------
#define RESET_PIN 11
#define BUTTON_PIN 12
#define ENCODER_A_PIN 3
#define ENCODER_B_PIN 2

#define CLK_PIN 13
#define ENCODER_COUNT_PER_ROTATION 4

#define OUT_CH1 5
#define OUT_CH2 6
#define OUT_CH3 7
#define OUT_CH4 8
#define OUT_CH5 9
#define OUT_CH6 10
#define LED_CH1 14
#define LED_CH2 15
#define LED_CH3 16
#define LED_CH4 17
#define LED_CH5 0
#define LED_CH6 1
#define LED_CLK 4

// -----------------  OLED  -----------------
#define BLACK 0
#define WHITE 1

#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// -----------------  ENCODER  -----------------
#define ENCODER_OPTIMIZE_INTERRUPTS 
#define ENCODER_COUNT_PER_CLICK 4
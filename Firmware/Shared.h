#ifndef shared
#define shared


//Oled setting
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<OakOLED.h>

//Encoder setting
#define  ENCODER_OPTIMIZE_INTERRUPTS //エンコーダノイズ対策
#define ENCODER_COUNT_PER_CLICK 4
#include <Encoder.h>


int debug = 0; // 1 =on 0 =off
//INTERNAL CLOCK Setup (not yet implemented)
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds
const byte ledPin = 4;    //using the built in LED
int tempo = 120;
int lasttempo = 120;
int ledState = LOW;
long previousMillis = 0;
long interval = 1000;

#define BLACK 0
#define WHITE 1

#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
OakOLED display;

Encoder myEnc(3, 2);//use 3pin 2pin

int oldPosition  = -999;
int newPosition = -999;
int i = 0;

//push button
bool sw = 0;//push button
bool old_sw;//countermeasure of sw chattering
unsigned long sw_timer = 0;//countermeasure of sw chattering




#endif
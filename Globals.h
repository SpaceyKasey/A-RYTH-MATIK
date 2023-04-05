#ifndef GLOBALS_H
#define GLOBALS_H

#include <EEPROM.h>

//Oled setting
#include<Wire.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Encoder setting

#include <Encoder.h>




inline unsigned int ch1_step = 0x8888; //テスト用
inline byte CH1_output = 0 ;
inline byte CH1_mute = 0 ;//0=ミュートしない、1=ミュートする

inline unsigned int ch2_step = 0x0808; //テスト用
inline byte CH2_output = 0 ;
inline byte CH2_mute = 0 ;//0=ミュートしない、1=ミュートする

inline unsigned int ch3_step = 0xCCCC; //テスト用
inline byte CH3_output = 0 ;
inline byte CH3_mute = 0 ;//0=ミュートしない、1=ミュートする

inline unsigned int ch4_step = 0x2222; //テスト用
inline byte CH4_output = 0 ;
inline byte CH4_mute = 0 ;//0=ミュートしない、1=ミュートする

inline unsigned int ch5_step = 0xFFFF; //テスト用
inline byte CH5_output = 0 ;
inline byte CH5_mute = 0 ;//0=ミュートしない、1=ミュートする

inline unsigned int ch6_step = 0x0000; //テスト用
inline byte CH6_output = 0 ;
inline byte CH6_mute = 0 ;//0=ミュートしない、1=ミュートする


#endif
#pragma once
#include "Common.hpp"

class EuclidSeq
{
public:
    EuclidSeq();
    void UpdateOuput();
    void EncoderUpdate(EncoderDirection direction);
    void UpdateDisplay();
    void KeyPress();
    void Initialize(Adafruit_SSD1306 *display, EEPROMManager *eepromManager);

private:
    EuclidMenuEnum menuState = EuclidMenuEnum::channel;
    Adafruit_SSD1306 *display;
    EEPROMManager *eepromManager;

    byte channel = 0;

    byte hits[6] = {4, 4, 5, 3, 2, 16};        // each channel hits
    byte offset[6] = {0, 2, 0, 8, 3, 9};       // each channele step offset
    bool mutes[6] = {0, 0, 0, 0, 0, 0};        // mute 0 = off , 1 = on
    byte limits[6] = {16, 16, 16, 16, 16, 16}; // eache channel max step

    bool offset_buf[6][16]; // offset buffer , Stores the offset result

    byte bar_now = 1;                // count 16 steps, the bar will increase by 1.
    byte bar_max[4] = {2, 4, 8, 16}; // selectable bar
    byte bar_select = 1;             // selected bar
    byte step_cnt = 0;               // count 16 steps, the bar will increase by 1.
    int ledState = LOW;

    unsigned long gate_timer = 0; // countermeasure of sw chattering

    byte playing_step[6] = {0, 0, 0, 0, 0, 0}; // playing step number , CH1,2,3,4,5,6

    int old_trg_in = 0;
    int old_rst_in = 0;

    int tempo = 120;
    int lasttempo = 120;

    long previousMillis = 0;
    long interval = 1000;

    void SaveSettings();
    void BPM();
    void Random_Change();
};
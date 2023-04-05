#pragma once

#include <Arduino.h>
#include "EuclidSeq.hpp"

EuclidSeq::EuclidSeq()
{
}

void EuclidSeq::Initialize(Adafruit_SSD1306 *display, EEPROMManager *eepromManager)
{
    this->display = display;
    this->eepromManager = eepromManager;
}

void EuclidSeq::EncoderUpdate(EncoderDirection direction)
{
    byte change = direction == EncoderDirection::Up ? 1 : -1;

    switch (this->menuState)
    {
    case EuclidMenuEnum::channel:
        this->channel = (this->channel + change) % 6;
        break;
    case EuclidMenuEnum::hits:
        if (this->channel != 6)
        {
            this->hits[this->channel] = (this->hits[this->channel] + change) % 17;
        }
        else
        {
            this->bar_select = (this->bar_select + change) % 4;
        }
        break;
    case EuclidMenuEnum::offset:
        this->offset[this->channel] = (this->offset[this->channel] + change) % 16;
        break;
    case EuclidMenuEnum::limit:
        this->limits[this->channel] = (this->limits[this->channel] + change) % 17;
        break;
    case EuclidMenuEnum::mute:
        this->mutes[this->channel] = !this->mutes[this->channel];
        break;
    case EuclidMenuEnum::reset:
        for (int ix = 0; ix < 6; ix++)
        {
            this->offset[ix] = 0;
        }
        break;
    case EuclidMenuEnum::save:
        // todo
        break;
    }
}

void EuclidSeq::KeyPress()
{
    if (this->channel == 6)
    {
        this->menuState = (EuclidMenuEnum)((this->menuState + 1) % 2);
    }
    else
    {
        this->menuState = (EuclidMenuEnum)((this->menuState + 1) % (int)(EuclidMenuEnum::last));
    }
}

void EuclidSeq::UpdateDisplay()
{
    this->display->clearDisplay();
    //-------------------------euclidean circle display------------------
    // draw setting menu
    this->display->setCursor(120, 0);
    if (this->channel != 6)
    { // not random mode
        this->display->print(this->channel + 1);
        this->display->setCursor(120, 9);
        this->display->print(F("H"));
        this->display->setCursor(120, 18);
        this->display->print(F("O"));
        this->display->setCursor(0, 36);
        this->display->print(F("L"));
        this->display->setCursor(0, 45);
        this->display->print(F("M"));
        this->display->setCursor(0, 54);
        this->display->print(F("R"));
        switch (this->menuState)
        {
        case EuclidMenuEnum::channel:
            this->display->drawTriangle(113, 0, 113, 6, 118, 3, WHITE);
            break;
        case EuclidMenuEnum::hits:
            this->display->drawTriangle(113, 9, 113, 15, 118, 12, WHITE);
            break;
        case EuclidMenuEnum::offset:
            this->display->drawTriangle(113, 18, 113, 24, 118, 21, WHITE);
            break;
        case EuclidMenuEnum::limit:
            this->display->drawTriangle(12, 36, 12, 42, 7, 39, WHITE);
            break;
        case EuclidMenuEnum::mute:
            this->display->drawTriangle(12, 45, 12, 51, 7, 48, WHITE);
            break;
        case EuclidMenuEnum::reset:
            this->display->drawTriangle(12, 54, 12, 60, 7, 57, WHITE);
            break;
        }
    }
    else
    { // random mode
        this->display->print(F("R"));
        this->display->setCursor(120, 9);
        this->display->print(F("O"));
        this->display->setCursor(120, 18);
        this->display->drawRect(1, 62 - bar_max[bar_select] * 2, 6, bar_max[bar_select] * 2 + 2, WHITE);
        this->display->fillRect(1, 64 - bar_now * 2, 6, bar_max[bar_select] * 2, WHITE);
    }

    // draw step dot
    for (int k = 0; k <= 5; k++)
    { // k = 1~6ch
        for (int j = 0; j <= limits[k] - 1; j++)
        { // j = steps
            this->display->drawPixel(x16[j] + graph_x[k], y16[j] + graph_y[k], WHITE);
        }
    }
    byte line_xbuf[17] = {0}; // Buffer for drawing lines
    byte line_ybuf[17] = {0}; // Buffer for drawing lines

    // draw hits line : 2~16hits
    for (int k = 0; k <= 5; k++)
    { // ch count
        int buf_count = 0;
        for (int m = 0; m < 16; m++)
        {
            if (offset_buf[k][m] == 1)
            {
                line_xbuf[buf_count] = x16[m] + graph_x[k]; // store active step
                line_ybuf[buf_count] = y16[m] + graph_y[k];
                buf_count++;
            }
        }
        int j;
        for (j = 0; j < buf_count - 1; j++)
        {
            this->display->drawLine(line_xbuf[j], line_ybuf[j], line_xbuf[j + 1], line_ybuf[j + 1], WHITE);
        }
        this->display->drawLine(line_xbuf[0], line_ybuf[0], line_xbuf[j], line_ybuf[j], WHITE);
    }

    // draw hits line : 1hits
    for (int k = 0; k <= 5; k++)
    { // ch count

        if (hits[k] == 1)
        {
            this->display->drawLine(15 + graph_x[k], 15 + graph_y[k], x16[offset[k]] + graph_x[k], y16[offset[k]] + graph_y[k], WHITE);
        }
    }

    // draw play step circle
    for (int k = 0; k <= 5; k++)
    { // ch count
        if (this->mutes[k] == 0)
        { // mute on = no display circle
            if (offset_buf[k][playing_step[k]] == 0)
            {
                this->display->drawCircle(x16[playing_step[k]] + graph_x[k], y16[playing_step[k]] + graph_y[k], 2, WHITE);
            }
            if (offset_buf[k][playing_step[k]] == 1)
            {
                this->display->fillCircle(x16[playing_step[k]] + graph_x[k], y16[playing_step[k]] + graph_y[k], 3, WHITE);
            }
        }
    }
    this->display->display();
}

void EuclidSeq::UpdateOuput()
{
    //-----------------offset setting----------------------
    for (int k = 0; k <= 5; k++)
    { // k = 1~6ch
        for (int i = this->offset[k]; i <= 15; i++)
        {
            this->offset_buf[k][i - this->offset[k]] = (pgm_read_byte(&(euc16[this->hits[k]][i])));
        }

        for (int i = 0; i < offset[k]; i++)
        {
            this->offset_buf[k][16 - this->offset[k] + i] = (pgm_read_byte(&(euc16[this->hits[k]][i])));
        }
    }

    //-----------------trigger detect, reset & output----------------------
    int rst_in = digitalRead(11); // external reset in
    if (this->old_rst_in == 0 && rst_in == 1)
    {
        for (int k = 0; k <= 5; k++)
        {
            this->playing_step[k] = 0;
        }
        UpdateDisplay();
    }

    int trg_in = digitalRead(13); // external trigger in
    if (this->old_trg_in == 0 && trg_in == 0 && this->gate_timer + 1000 <= millis())
    {
        void BPM();
    }
    else if (old_trg_in == 0 && trg_in == 1)
    {
        this->gate_timer = millis();
        digitalWrite(4, HIGH);

        for (int i = 0; i <= 5; i++)
        {
            this->playing_step[i]++; // When the trigger in, increment the step by 1.
            if (this->playing_step[i] >= this->limits[i])
            {
                this->playing_step[i] = 0; // When the step limit is reached, the step is set back to 0.
            }
        }

        for (int k = 0; k <= 5; k++)
        { // output gate signal
            if (this->offset_buf[k][playing_step[k]] == 1 && mutes[k] == 0)
            {
                switch (k)
                {
                case 0: // CH1
                    digitalWrite(5, HIGH);
                    digitalWrite(14, HIGH);
                    break;

                case 1: // CH2
                    digitalWrite(6, HIGH);
                    digitalWrite(15, HIGH);
                    break;

                case 2: // CH3
                    digitalWrite(7, HIGH);
                    digitalWrite(16, HIGH);
                    break;

                case 3: // CH4
                    digitalWrite(8, HIGH);
                    digitalWrite(0, HIGH);
                    break;

                case 4: // CH5
                    digitalWrite(9, HIGH);
                    digitalWrite(1, HIGH);
                    break;

                case 5: // CH6
                    digitalWrite(10, HIGH);
                    digitalWrite(17, HIGH);
                    break;
                }
            }

            UpdateDisplay();
        }

        if (this->channel == 6)
        { // random mode setting
            this->step_cnt++;
            if (this->step_cnt >= 16)
            {
                this->bar_now++;
                this->step_cnt = 0;
                if (this->bar_now > this->bar_max[this->bar_select])
                {
                    this->bar_now = 1;
                    Random_Change();
                }
            }
        }

        if (this->gate_timer + 10 <= millis())
        { // off all gate , gate time is 10msec
            // turn off gates 5-10
            for (int i = 5; i <= 10; i++)
            {
                digitalWrite(i, LOW);
            }
        }

        if (this->gate_timer + 100 <= millis())
        { // off all gate , gate time is 10msec
            // turn off gates 0-1 and 4-17
            for (int i = 0; i <= 17; i++)
            {
                if (i != 2 && i != 3)
                {
                    digitalWrite(i, LOW);
                }
            }
        }

        if (this->gate_timer + 100 <= millis())
        { // off all gate , gate time is 10msec
            // turn off gate 4
            digitalWrite(4, LOW);
        }
    }
}

void EuclidSeq::Random_Change()
{
    // when random mode and full of bar_now ,
    for (int k = 1; k <= 5; k++)
    {

        if (hit_occ[k] >= random(1, 100))
        { // hit random change
            this->hits[k] = random(hit_rng_min[k], hit_rng_max[k]);
        }

        if (off_occ[k] >= random(1, 100))
        { // hit random change
            this->offset[k] = random(0, 16);
        }

        if (mute_occ[k] >= random(1, 100))
        { // hit random change
            this->mutes[k] = 1;
        }
        else if (mute_occ[k] < random(1, 100))
        { // hit random change
            this->mutes[k] = 0;
        }
    }
}

void EuclidSeq::BPM()
{
    // int tempo = 120;
    //  algorithm to convert tempo into BPM
    this->interval = (1000 / this->tempo) * 60 / 4;
    unsigned long currentMillis = millis();
    this->lasttempo = tempo;

    if (currentMillis - this->previousMillis > this->interval)
    {
        // save the last time you blinked the LED
        this->previousMillis = currentMillis;
        if (this->ledState == LOW)
        {
            // Todo this seems like a bug i would always be the value of the last channel
            // this->playing_step[i]++;
            this->playing_step[offset[5]]++;
            this->ledState = HIGH;
        }
        else
            this->ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(LED_CLK, this->ledState);
    }
}

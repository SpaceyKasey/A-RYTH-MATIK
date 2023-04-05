#pragma once
#include "Common.hpp"

class ArythmatikSetup
{
public:
    ArythmatikSetup();
    void UpdateOuput();
    void EncoderUpdate(EncoderDirection direction);
    void UpdateDisplay();
    void KeyPress();
    void Initialize(Adafruit_SSD1306 *display, EEPROMManager *eepromManager);

private:
    SetupMenuEnum menuState = SetupMenuEnum::Mode;
    Mode configuredMode;
    bool invertEncoder;
    Adafruit_SSD1306 *display;
    EEPROMManager *eepromManager;

    void SaveSettings();
};
#include "Setup.hpp"

ArythmatikSetup::ArythmatikSetup()
{
}

void ArythmatikSetup::Initialize(Adafruit_SSD1306 *display, EEPROMManager *eepromManager)
{
    this->display = display;
    this->eepromManager = eepromManager;

    // Load settings from EEPROM
    byte *eepromData = this->eepromManager->read();
    // update variables
    this->configuredMode = static_cast<Mode>(eepromData[Config_Mode_EEPROM_Offset]);
    this->invertEncoder = eepromData[Config_Invert_EEPROM_Offset];
}

void ArythmatikSetup::UpdateOuput()
{
    // Nothing to do here
}

void ArythmatikSetup::EncoderUpdate(EncoderDirection direction)
{
    if (direction == EncoderDirection::Up)
    {
        this->menuState = static_cast<SetupMenuEnum>(static_cast<int>(this->menuState) + 1);
        if (this->menuState == SetupMenuEnum::Last)
        {
            this->menuState = static_cast<SetupMenuEnum>(static_cast<int>(0));
        }
    }
    else
    {
        if (this->menuState == static_cast<SetupMenuEnum>(0))
        {
            this->menuState = SetupMenuEnum::Last;
        }
        this->menuState = static_cast<SetupMenuEnum>(static_cast<int>(this->menuState) - 1);
    }
}

void ArythmatikSetup::UpdateDisplay()
{
    this->display->clearDisplay();

    this->display->setCursor(0, 0);
    this->display->print(F("ARYTHMATIK 1.1 SETUP"));
    this->display->setCursor(0, 9);
    this->display->print(F("MODE: "));

    switch (this->configuredMode)
    {
    case Mode::Euclid:
        this->display->print(F("Euclid"));
        break;
    case Mode::Gate:
        this->display->print(F("Gate"));
        break;
    }
    this->display->setCursor(0, 18);
    this->display->print(F("INVERT ENCODER: "));
    this->display->print(invertEncoder ? F("YES") : F("NO"));
    this->display->setCursor(0, 27);
    this->display->print(F("RESET"));

    // Draw < at end of line to indicate the current setting
    switch (this->menuState)
    {
    case SetupMenuEnum::Mode:
        this->display->setCursor(120, 9);
        break;
    case SetupMenuEnum::Invert:
        this->display->setCursor(120, 18);
        break;
    case SetupMenuEnum::Restart:
        this->display->setCursor(120, 27);
        break;
    }

    this->display->print(F("<"));
    this->display->display();
}

void ArythmatikSetup::KeyPress()
{
    switch (this->menuState)
    {
    case SetupMenuEnum::Mode:
        this->configuredMode = static_cast<Mode>(static_cast<int>(this->configuredMode) + 1);
        if (this->configuredMode == Mode::Last)
        {
            this->configuredMode = static_cast<Mode>(static_cast<int>(0));
        }
        break;
    case SetupMenuEnum::Invert:
        this->invertEncoder = !this->invertEncoder;
        break;
    case SetupMenuEnum::Save:
        SaveSettings();
        break;
    case SetupMenuEnum::Restart:
        delay(1000); // Wait for user to release button
        // todo: reset function
        break;
    }
}

void ArythmatikSetup::SaveSettings()
{
    byte *eepromData = this->eepromManager->read();

    eepromData[Config_Mode_EEPROM_Offset] = static_cast<byte>(configuredMode);
    eepromData[Config_Invert_EEPROM_Offset] = invertEncoder ? 1 : 0;

    this->eepromManager->write();
}
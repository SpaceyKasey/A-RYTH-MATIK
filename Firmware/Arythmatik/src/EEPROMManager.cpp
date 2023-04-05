#include "EEPROMManager.hpp"
#include <EEPROM.h>

EEPROMManager::EEPROMManager()
{
}

void EEPROMManager::write()
{
    this->eeprom.version = EEPROM_VERSION;
    this->eeprom.checksum = 0;
    for (int i = 0; i < EEPROM_SIZE - 2; i++)
    {
        this->eeprom.checksum += this->eeprom.data[i];
    }
    EEPROM.put(0, this->eeprom);
}

byte *EEPROMManager::read()
{
    EEPROM.get(0, this->eeprom);
    if (this->eeprom.version == EEPROM_VERSION)
    {
        uint8_t checksum = 0;
        for (int i = 0; i < EEPROM_SIZE - 2; i++)
        {
            checksum += this->eeprom.data[i];
        }
        if (checksum != this->eeprom.checksum)
        {
            Serial.println("EEPROM checksum error");
        }
    }
    else
    {
        Serial.println("EEPROM version error");
    }

    return this->eeprom.data;
}

void EEPROMManager::reset()
{
    this->eeprom.version = EEPROM_VERSION;
    this->eeprom.checksum = 0;
    for (int i = 0; i < EEPROM_SIZE - 2; i++)
    {
        this->eeprom.data[i] = 0;
        this->eeprom.checksum += this->eeprom.data[i];
    }
    EEPROM.put(0, this->eeprom);
}
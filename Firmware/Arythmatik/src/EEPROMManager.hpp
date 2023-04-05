#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define EEPROM_VERSION 1

// EEPROM memory map
#define EEPROM_VERSION_ADDRESS 0
#define EEPROM_CHECKSUM_ADDRESS 1
#define EEPROM_DATA_ADDRESS 2

#define EEPROM_DATA_SIZE (EEPROM_SIZE - 2)

#define Gates_EEPROM_Offset 0
#define Euclid_EEPROM_Offset 100
#define Config_EEPROM_Offset 200

// Gates
#define Gates_Lane1High_EEPROM_Offset (Gates_EEPROM_Offset + 0)
#define Gates_Lane1Low_EEPROM_Offset (Gates_EEPROM_Offset + 1)
#define Gates_Lane2High_EEPROM_Offset (Gates_EEPROM_Offset + 2)
#define Gates_Lane2Low_EEPROM_Offset (Gates_EEPROM_Offset + 3)
#define Gates_Lane3High_EEPROM_Offset (Gates_EEPROM_Offset + 4)
#define Gates_Lane3Low_EEPROM_Offset (Gates_EEPROM_Offset + 5)
#define Gates_Lane4High_EEPROM_Offset (Gates_EEPROM_Offset + 6)
#define Gates_Lane4Low_EEPROM_Offset (Gates_EEPROM_Offset + 7)
#define Gates_Lane5High_EEPROM_Offset (Gates_EEPROM_Offset + 8)
#define Gates_Lane5Low_EEPROM_Offset (Gates_EEPROM_Offset + 9)
#define Gates_Lane6High_EEPROM_Offset (Gates_EEPROM_Offset + 10)
#define Gates_Lane6Low_EEPROM_Offset (Gates_EEPROM_Offset + 11)

// Euclid

// Config
#define Config_Mode_EEPROM_Offset (Config_EEPROM_Offset + 0)
#define Config_Invert_EEPROM_Offset (Config_EEPROM_Offset + 1)


struct ArythmaticEEPROM
{
    uint8_t version;
    uint8_t checksum;
    uint8_t data[EEPROM_SIZE - 2];
};

class EEPROMManager
{
public:
    EEPROMManager();
    void write();
    byte* read();
    void reset();
private:
    bool initialized = false;
    ArythmaticEEPROM eeprom;
};



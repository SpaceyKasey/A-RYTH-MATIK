#ifndef GATESEQ_H
#define GATESEQ_H



class GateSeq
{
    public:
    GateSeq();
    void UpdateOuput();
    void EncoderUpdate(EncoderDirection direction);
    void UpdateDisplay();
    void KeyPress();
    void Initialize(display *display, EEPROMManager *eepromManager);

    private:
    inline SetupMenuEnum menuState = SetupMenuEnum::Mode;
    inline Mode configuredMode;
    inline bool invertEncoder;
    inline display *display;
    inline EEPROMManager *eepromManager;

    void SaveSettings();
}

#endif
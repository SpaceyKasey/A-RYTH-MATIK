#include "src/Common.hpp"
#include "src/Config.hpp"
#include "src/EEPROMManager.hpp"
#include "src/Setup.hpp"
#include "src/EuclidSeq.hpp"
// #include "src/GateSeq.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Encoder myEnc(ENCODER_A_PIN, ENCODER_B_PIN);

EEPROMManager eepromManager = EEPROMManager();
ArythmatikSetup aSetup = ArythmatikSetup();
//EuclidSeq euclidSeq = EuclidSeq();
// GateSeq gateSeq = GateSeq();

Mode configuredMode;
bool invertEncoder;

byte encD = 0;
byte encU = 0;

byte sw = 0;
unsigned long sw_timer = 0; // countermeasure of sw chattering

unsigned long enc_timer = 0;
int oldPosition = -999;
int newPosition = -999;

bool debounceSetup = false;

byte screen_update = 0;

void setup()
{
    // Set up the display
    delay(100); // Screen needs time to initialize before sending commands
    display.begin();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(F("ARYTHMATIK 1.1"));
    display.setCursor(0, 9);
    display.print(F("LOADING..."));
    display.display();

    // Configure the pins
    pinMode(LED_CLK, OUTPUT);             // LED_CLK
    pinMode(LED_CH1, OUTPUT);             // LED_CH1
    pinMode(LED_CH2, OUTPUT);             // LED_CH2
    pinMode(LED_CH3, OUTPUT);             // LED_CH3
    pinMode(LED_CH4, OUTPUT);             // LED_CH4
    pinMode(LED_CH5, OUTPUT);             // LED_CH5
    pinMode(LED_CH6, OUTPUT);             // LED_CH6
    pinMode(OUT_CH1, OUTPUT);             // OUT_CH1
    pinMode(OUT_CH2, OUTPUT);             // OUT_CH2
    pinMode(OUT_CH3, OUTPUT);             // OUT_CH3
    pinMode(OUT_CH4, OUTPUT);             // OUT_CH4
    pinMode(OUT_CH5, OUTPUT);             // OUT_CH5
    pinMode(OUT_CH6, OUTPUT);             // OUT_CH6
    pinMode(ENCODER_A_PIN, INPUT_PULLUP); // ENCODER A?
    pinMode(ENCODER_B_PIN, INPUT_PULLUP); // ENCODER B?
    pinMode(RESET_PIN, INPUT_PULLUP);     // RST
    pinMode(BUTTON_PIN, INPUT_PULLUP);    // BUTTON
    pinMode(CLK_PIN, INPUT_PULLUP);       // BUTTON

    byte *settings = eepromManager.read();
    configuredMode = static_cast<Mode>(settings[Config_Mode_EEPROM_Offset]);
    invertEncoder = static_cast<bool>(settings[Config_Invert_EEPROM_Offset]);

    if (static_cast<int>(configuredMode) < 0 || static_cast<int>(configuredMode) > static_cast<int>(Mode::Last))
    {
        configuredMode = static_cast<Mode>(0);
    }

    // Check to see if the encoder button is pressed and if so, enter aSetup mode
    if (digitalRead(BUTTON_PIN) == false)
    {
        aSetup.Initialize(&display, &eepromManager);
        configuredMode = Mode::Setup;
        debounceSetup = true;
    }
    else
    {
        // Switch to correct module here

        switch (configuredMode)
        {
        case Mode::Euclid:
           // euclidSeq.Initialize(&display, &eepromManager);
            break;
        case Mode::Gate:
            // gateSeq.Initialize(&display, &eepromManager);
            break;
        }
    }
}

void loop()
{

    //-----------------push button----------------------

    sw = 1;
    if ((digitalRead(12) == 0) && (sw_timer + 300 <= millis()))
    { // push button on ,Logic inversion , sw_timer is countermeasure of chattering
        sw_timer = millis();
        sw = 0;
    }
    else
    {
        debounceSetup = false;
    }

    if (sw == 0 && debounceSetup == false)
    {
        switch (configuredMode)
        {
        case Mode::Euclid:
           // euclidSeq.KeyPress();
            break;
        case Mode::Gate:
            // gateSeq.KeyPress();
            break;
        case Mode::Setup:
            aSetup.KeyPress();
            break;
        }
    }

    //-----------------Rotary encoder read----------------------
    newPosition = myEnc.read() / ENCODER_COUNT_PER_CLICK;
    if (newPosition < oldPosition && enc_timer + 200 < millis())
    { // turn left
        enc_timer = millis();
        oldPosition = newPosition;
        encD = 1;
    }
    else
    {
        encD = 0;
    }

    if (newPosition > oldPosition && enc_timer + 200 < millis())
    { // turn right
        enc_timer = millis();
        oldPosition = newPosition;
        encU = 1;
    }
    else
    {
        encU = 0;
    }

    if (encU || encD)
    {
        switch (configuredMode)
        {
        case Mode::Euclid:
          //  euclidSeq.EncoderUpdate((invertEncoder ? encU : encD) ? EncoderDirection::Up : EncoderDirection::Down);
            break;
        case Mode::Gate:
            // gateSeq.EncoderUpdate((invertEncoder?encU:encD)?EncoderDirection::Up:EncoderDirection::Down);
            break;
        case Mode::Setup:
            aSetup.EncoderUpdate((invertEncoder ? encU : encD) ? EncoderDirection::Up : EncoderDirection::Down);
            break;
        }
    }

    //-----------------LED update----------------------
    switch (configuredMode)
    {
    case Mode::Euclid:
        //euclidSeq.UpdateOuput();
        break;
    case Mode::Gate:
        // gateSeq.UpdateOuput();
        break;
    case Mode::Setup:
        aSetup.UpdateOuput();
        break;
    }

    //-----------------Screen update----------------------

    if (screen_update++ == 0)
    { // Update when the byte rolls over
        switch (configuredMode)
        {
        case Mode::Euclid:
           // euclidSeq.UpdateDisplay();
            break;
        case Mode::Gate:
            // gateSeq.UpdateDisplay();
            break;
        case Mode::Setup:
            aSetup.UpdateDisplay();
            break;
        }
    }
}

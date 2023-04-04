void(* resetFunc) (void) = 0;
#include "State.h"
#include "Shared.h"

#include "ARYTHMATIK_Euclid/ARYTHMATIK_Euclid.h"
#include "ARYTHMATIK_Gate-seq/ARYTHMATIK_Gate-seq.h"

#define EEPROM_TYPE 20 // Start at EEProm 20 as gate is using 1-13
#define EEPROM_INVERT 21

#define RESET_PIN 11
#define BUTTON_PIN 12
#define ENCODER_A_PIN 3
#define ENCODER_B_PIN 2
void OLED_display();
void HandleKeypress();
String getConfiguredMode();
void Save_EEProm();
void UpdateMenu(EncoderDirection direction);



void setup() {


    // Ok, what we want to do here is check the EEPROM to see which module is enabled, then switch to that module.
    // We also want to check the state of the encoder button to see if it is pressed in, if it is we want to go into setup mode.
    



    //pin mode setting
    pinMode(RESET_PIN, INPUT_PULLUP); //RST
    pinMode(BUTTON_PIN, INPUT_PULLUP); //BUTTON
    pinMode(ENCODER_A_PIN, INPUT_PULLUP); //ENCODER A?
    pinMode(ENCODER_B_PIN, INPUT_PULLUP); //ENCODER B?

    configuredMode = static_cast<Mode>(EEPROM.read(EEPROM_TYPE));
    invertEncoder =  static_cast<bool>(EEPROM.read(EEPROM_INVERT));

    if(digitalRead(BUTTON_PIN) == true){
      setup_mode = true;
              // OLED setting
      delay(1000); // Screen needs a sec to initialize
      display.begin();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      OLED_display();

      enc_timer = 0;
    }
}

void loop() {

  if(setup_mode == false){
    //Switch to correct module here

    switch (configuredMode){
      case Mode::Euclid:
        EuclidSetup();
        EuclidLoop();
      break;
      case Mode::Gate:
        GateSetup();
        GateLoop();
      break;
    }

  }

  //-----------------push button----------------------

  sw = 1;
  if ((digitalRead(12) == 0 ) && ( sw_timer + 300 <= millis() )) { //push button on ,Logic inversion , sw_timer is countermeasure of chattering
    sw_timer = millis();
    sw = 0;
    Serial.print(F("button"));
  }
  if (sw == 0) {
    HandleKeypress();
  }


  //-----------------Rotary encoder read----------------------
  newPosition = myEnc.read()/ENCODER_COUNT_PER_CLICK;
  if ( newPosition < oldPosition && enc_timer + 200 < millis()) { //turn left
    enc_timer = millis();
    oldPosition = newPosition;
    encD = 1;
    Serial.print(F("<<< encoder <<<"));
  } else {
    encD = 0;
  }

  if ( newPosition > oldPosition && enc_timer + 200 < millis()) { //turn right
    enc_timer = millis();
    oldPosition = newPosition;
    encU = 1;
    Serial.print(F(">>> encoder >>>"));
  } else {
    encU = 0;
  }

  if(encU || encD){
    UpdateMenu(encU?EncoderDirection::Up:EncoderDirection::Down);
  }
  
  if (screen_update++ == 0) { //Update when the byte rolls over
    OLED_display(); //refresh display
  }
}

void HandleKeypress(){
  switch(menuState){
    case MenuSelection::Mode:
      configuredMode = static_cast<Mode>(static_cast<int>(configuredMode) + 1);
      if(configuredMode == Mode::Last){
        configuredMode = static_cast<Mode>(static_cast<int>(0));
      }
      EEPROM.write(EEPROM_TYPE, static_cast<int>(configuredMode));
    break;
    case MenuSelection::Invert:
      invertEncoder = !invertEncoder;
      EEPROM.write(EEPROM_INVERT, invertEncoder);
    break;
    case MenuSelection::Reset://Restarts the arduino
      delay(1000); // Wait for user to release button
      resetFunc();
    break;
  }

}

void UpdateMenu(EncoderDirection direction){
  if(direction == EncoderDirection::Up){
    configuredMode = static_cast<Mode>(static_cast<int>(configuredMode) + 1);
    if(configuredMode == Mode::Last){
      configuredMode = static_cast<Mode>(static_cast<int>(0));
    }
  }else{
    if(configuredMode == static_cast<Mode>(0)){
      configuredMode = Mode::Last;
    }
    configuredMode = static_cast<Mode>(static_cast<int>(configuredMode) - 1);
  }
}

void Save_EEProm(){
  EEPROM.write(EEPROM_TYPE, static_cast<int>(configuredMode));
  EEPROM.write(EEPROM_INVERT, invertEncoder);
}

String getConfiguredMode(){
  switch (configuredMode){
    case Mode::Euclid:
      return F("Euclid");
    break;
    case Mode::Gate:
      return F("Gate");
    break;
  }
}




void OLED_display() {
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print(F("ARYTHMATIK 1.0 SETUP"));
  display.setCursor(0, 9);
  display.print(F("MODE: "));
  display.print(getConfiguredMode());
  display.setCursor(0, 18);
  display.print(F("INVERT ENCODER: "));
  display.print(invertEncoder ? F("YES") : F("NO"));
  display.setCursor(0, 27);
  display.print(F("RESET"));

  // Draw < at end of line to indicate the current setting
  switch (menuState){
    case MenuSelection::Mode:
      display.setCursor(120, 9);
    break;
    case MenuSelection::Invert:
      display.setCursor(120, 18);
    break;
    case MenuSelection::Reset:
      display.setCursor(120, 27);
    break;
  }


  display.print(F("<"));
  

  display.display();
  

}


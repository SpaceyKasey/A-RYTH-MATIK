#pragma includeonce

enum class Mode {
  Euclid = 0,
  Gate,
  Last
};

enum class MenuSelection{
  Mode = 0,
  Invert,
  Reset,
  Last
};

enum class EncoderDirection{
  Up,
  Down
}; 


Mode configuredMode = Mode::Euclid;
MenuSelection menuState = MenuSelection::Mode;
EncoderDirection encDirection = EncoderDirection::Up;


bool setup_mode = false;
bool invertEncoder = false;

byte screen_update = 0;
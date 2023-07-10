/*****
  This software is licensed un the GNU General Public License, Version 3, as presetned in
  https://www.gnu.org/licenses/gpl-3.0.txt. You may use this software for non-commercial use. 
  For commercial use, contact the authors:
  Jack Purdum, W8TEE
  Al Peter, AC8GY

  This header file must be copied into all copies and deriviative works of this software
  and remain the very first thing in the primary source code file.
  
*****/

//                          Compiled with ESP32-WROOM-32
/*
  Version 013, Nov.22, 2019, Jack Purdum, W8TEE, minor changes to speed up polling on the paddle keys.
  Version 013, Nov.18, 2019, Jack Purdum, W8TEE, change wpm encoder to use interrupts.
  Version 012, Nov.13, 2019, Jack Purdum, W8TEE, work on messages switches and encoder
  Version 010, Nov.11, 2019, Jack Purdum, W8TEE, wired the encoder control and added software
  Version 008, Nov. 4, 2019, Jack Purdum, W8TEE, moved protoboard and changed screen setup and main menu
  Version 007, June 9, 2019, Jack Purdum, W8TEE, added volume control for sidetone, keyer bypass, minor bug fixes
  Version 006, June 4, 2019, Jack Purdum, W8TEE, added code to flash LED and buzzer when message switch pressed
  Version 005, May 28, 2019, Jack Purdum, W8TEE, added start of 4N25 code
  Version 004, May 19, 2019, Jack Purdum, W8TEE, added message editing
  Version 003, May 15, 2019, Jack Purdum, W8TEE, worked on message scrolling
*/

#include "CWM.h"


// Global; used in several places

// These are globals used in more than one source file
uint8_t switchIndexes[] = {1, 2, 3};
char inputBuffer[MAXMESSAGELENGTH + 1];
uint8_t numberIndex = 0;
int wordsPerMinute;
int ditlen;



//This constructor uses Hardware SPI pins for SPI Clock, MOSI and MISO
// Previously #defined TFT_WIDTH and TFT_HEIGHT values define the geometry
// Previously #defined pin values TFT_CS and TFT_DC are used for control signals
// If you want to connect the display RES pin, #define TFT_RST to be that pin number
// If you want to connect the display LED pin, #define TFT_BL and TFT_BACKLIGHT_ON
TFT_eSPI tft = TFT_eSPI();

TFT_eSPI_Button key[40];
TFT_eSPI_Button clearText;
TFT_eSPI_Button spaceBar;
TFT_eSPI_Button store;
TFT_eSPI_Button cancel;
TFT_eSPI_Button arrowUp;
TFT_eSPI_Button arrowDown;
TFT_eSPI_Button switch1;
TFT_eSPI_Button switch2;
TFT_eSPI_Button switch3;
TFT_eSPI_Button done;
TFT_eSPI_Button selectEditMessage;

TFT_eSPI_Button NewMessage;
TFT_eSPI_Button ReviewMessages;
TFT_eSPI_Button AssignMessages;
TFT_eSPI_Button EditMessages;

Rotary rotary = Rotary(ENCODER1PINA, ENCODER1PINB);


char letterTable[] = {                 // Morse coding: dit = 0, dah = 1
  0b101,              // A                first 1 is the sentinel marker
  0b11000,            // B
  0b11010,            // C
  0b1100,             // D
  0b10,               // E
  0b10010,            // F
  0b1110,             // G
  0b10000,            // H
  0b100,              // I
  0b10111,            // J
  0b1101,             // K
  0b10100,            // L
  0b111,              // M
  0b110,              // N
  0b1111,             // O
  0b10110,            // P
  0b11101,            // Q
  0b1010,             // R
  0b1000,             // S
  0b11,               // T
  0b1001,             // U
  0b10001,            // V
  0b1011,             // W
  0b11001,            // X
  0b11011,            // Y
  0b11100             // Z
};

char numberTable[] = {
  0b111111,           // 0
  0b101111,           // 1
  0b100111,           // 2
  0b100011,           // 3
  0b100001,           // 4
  0b100000,           // 5
  0b110000,           // 6
  0b111000,           // 7
  0b111100,           // 8
  0b111110            // 9
};

char punctuationTable[] = {
  0b01101011,         // exclamation mark 33
  0b01010010,         // double quote 34
  0b10001001,         // dollar sign 36
  0b00101000,         // ampersand 38
  0b01011110,         // apostrophe 39
  0b01011110,         // parentheses (L) 40, 41
  0b01110011,         // comma 44
  0b00100001,         // hyphen 45
  0b01010101,         // period  46
  0b00110010,         // slash 47
  0b01111000,         // colon 58
  0b01101010,         // semi-colon 59
  0b01001100,         // question mark 63
  0b01001101,         // underline 95
  0b01101000,         // paragraph
};
int ASCIIForPunctuation[] = {33, 34, 36, 39, 41, 44, 45, 46, 47, 58, 59, 63, 95};  // Indexes into code


/*****
  Purpose: Create menu button objects

  Paramter list:
    void

  Return value:
    void
*****/
void InitStartupMenu()
{
  // graph object, x,   y,    w,  h,   outline,      fill,      text,   lable,   font size
  ReviewMessages.initButton(&tft, 160,  55, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, (char *)"Review", KEY_TEXTSIZE);
  NewMessage.initButton(&tft,     160, 100, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, (char *)"Create", KEY_TEXTSIZE);
  AssignMessages.initButton(&tft, 160, 145, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, (char *)"Assign", KEY_TEXTSIZE);
  EditMessages.initButton(&tft,   160, 195, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, (char *)"Edit",   KEY_TEXTSIZE);
}

/*****
  Purpose: Display menu button objects

  Paramter list:
    void

  Return value:
    void
*****/
void DrawStartupMenu()
{
  tft.fillScreen(TFT_BLACK);
  //  tft.drawLine(160, 5, 160, 235, TFT_RED);
  //  tft.drawRect(170, 155, 150, 80, TFT_RED);

  tft.setFreeFont(FSB9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 30);
  tft.print("Messages:");

  ReviewMessages.drawButton();
  NewMessage.drawButton();
  AssignMessages.drawButton();
  EditMessages.drawButton();

  tft.setCursor(10, 230);
  tft.print("WPM: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(wordsPerMinute);
}


/*****
  Purpose: to cause a delay in program execution

  Parameter list:
    unsigned long millisWait    // the number of millseconds to wait

  Return value:
    void
*****/


/*****
  Purpose: to cause a delay in program execution

  Parameter list:
    unsigned long millisWait    // the number of millseconds to wait

  Return value:
    void
*****/
void Splash()
{
  // Rather than tedious calculations of x origin, use drawString's ability
  // to use a TextDatum to center the strings horizontally.
  
  tft.fillScreen(TFT_BLACK);
  
  // Y-origin is always at the bottom, will center them up around horizontal middle
  tft.setTextDatum(BC_DATUM);
  
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.setFreeFont(FSB18);
  tft.drawString("CW Messenger", DISPLAYWIDTH/2, 70);

  tft.setFreeFont(FSB12);
  // VERSIONNUMBER is a string literal, so concatenation is automatic
  tft.drawString("Ver  00" VERSIONNUMBER, DISPLAYWIDTH/2, 97);
  
  tft.setFreeFont(FSB9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("by", DISPLAYWIDTH/2, 122);
  
  tft.setFreeFont(FSB12);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Jack Purdum, W8TEE", DISPLAYWIDTH/2, 160);
  tft.drawString(" Al Peter, AC8GY", DISPLAYWIDTH/2, 190);
  // For the  finished product, give the user time to admire
  // the splash page.
  //delay(20000);

  // During development, it's OK just to get a glimpse...
  delay(2000);
  
  // I'm not sure this is really necessary
  tft.setFreeFont(FSB9);
#ifdef DEBUG
  Serial.println("Exiting Splash()");
#endif
}


#if 0
// A little exercise to see what EEPROM.begin() does with the size given.
// Note that EEPROM.WriteBytes won't write anything if the number of
// bytes you are trying to write exceeds the value of the parameter
// used in EEPROM.begin()
//
#include <nvs.h>
#include <esp_partition.h>
#include <esp_log.h>

// This shows key_size = 4
void show_eeparams(void)
{
  uint32_t handle = 0;
  const char *name = EEPROM_FLASH_PARTITION_NAME;
  size_t key_size = 0;
  esp_err_t res = nvs_open(name, NVS_READWRITE, &handle);
  if (res != ESP_OK) {
      `("Unable to open NVS namespace: %d", res);
      //return false;
  }

  res = nvs_get_blob(handle, name, NULL, &key_size);
  if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
      printf("Unable to read NVS key: %d", res);
  }
  else {
    printf("res = %d\n", res);
    printf("key_size = %u\n", key_size);
  }

}
#endif

void setup()
{
  tft.fillScreen(TFT_BLACK);
  Serial.begin(115200);
  // It needs some time to settle down before using Serial
  while (!Serial)
    ;
  delay(100);
  Serial.println("\nDave's CW Messenger compiled on " __DATE__ " at " __TIME__);
  //show_eeparams();
  //Serial.print("Size of tft = "); Serial.println(sizeof(tft)); // 276 

// Need four bytes of EEPROM for switches and wpm
  //while (!EEPROM.begin(3)) {              //Allocate EEPROM memory
    //true;
  //}
  // If it can't initialze EEPROM, something is dreadfully wrong
  // but most of the  program still works.  Just can't change
  // WPM or Switch assignments.
  //
  uint8_t ee_block_size = 4;
  EEPROM.begin(ee_block_size);
  if (!EEPROM.begin(ee_block_size)) {
    Serial.print("Error: Can't initialize EEPROM block with size = ");
    Serial.println(ee_block_size);
  }
  //else {
    //Serial.print("Initialized EEPROM with block size = ");Serial.println(ee_block_size);
  //}

// Uses TFT_CS and TFT_DC defined in TFT_eSPI setup.  Hardware SPI clock and data pins
// are fixed
  tft.begin();
  tft.init();
  
// Set the rotation before doing anything that depends on geometry
  tft.setRotation(1); // Rotation 1 for display pins on right, Rotation 3 for pins on left
#ifdef DEBUG
  Serial.print("User message count = ");
  Serial.println(fetchUserssageCount()); // 11
#endif

  pinMode(DITPADDLE,           INPUT_PULLUP);
  pinMode(DAHPADDLE,           INPUT_PULLUP);

  pinMode(MSGSWITCH1,           INPUT_PULLUP);    // Message switches
  pinMode(MSGSWITCH2,           INPUT_PULLUP);
  pinMode(MSGSWITCH3,           INPUT_PULLUP);

  pinMode(OPTOINPUT,            OUTPUT);
  digitalWrite(OPTOINPUT, LOW);

  InitKeyboardButtons();

  pinMode(ENCODER1PINA,         INPUT_PULLUP);
  pinMode(ENCODER1PINB,         INPUT_PULLUP);
  pinMode(ENCODERSWITCH1,       INPUT_PULLUP);

  digitalWrite(ENCODER1PINB,    HIGH);
  digitalWrite(ENCODERSWITCH1,  HIGH);

  touch_calibrate();
  InitKeypad();         // Initialize all button objects
  InitStartupMenu();

  ReadSwitchesArray();                // Previously-stored messages indexes for switches from EEPROM
  ReadWordsPerMinute();               // Read wpm from EEPROM
  SetDitLength(MESSAGEWPM);
  Splash();

 /*
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH1), ReadMessageSwitch1, CHANGE);   // Message to send
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH2), ReadMessageSwitch2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH3), ReadMessageSwitch3, CHANGE);
 
    attachInterrupt(digitalPinToInterrupt(DITPADDLE),    Dit,    CHANGE);   // Keyer paddles
    attachInterrupt(digitalPinToInterrupt(DAHPADDLE),    Dah,    CHANGE);
  */
  attachInterrupt(digitalPinToInterrupt(ENCODER1PINA), encoderISR, CHANGE);   // WPM encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER1PINB), encoderISR, CHANGE);

  // Get ready to loop()
  DrawStartupMenu();      // Add appropriate code

} // End of setup();

// File scope variables are automatically initialized to zero, but I'll
// show it just to make sure we all know it.
//
//static int whichOne = 0;
//
void loop() 
{
  uint8_t touchPressed;
  uint16_t touchX, touchY;
  int userMessageCount;
  int indexOfEditMessage;
  int menuSelect;
  int msgSwitchPressed;
 
  // TODO (maybe): Make a "real" keyer (iambic, dot memory, etc...)
  while (digitalRead(DAHPADDLE) == LOW) {   // Send atom until released...
    Dah();
  }
  while (digitalRead(DITPADDLE) == LOW) {   // Send atom until released...
    Dit();
  }

  // Check pushbuttons
  msgSwitchPressed = ReadMessageSwitches();  // Returns -1 if no switch pressed
  if (msgSwitchPressed != -1) {
    #ifdef DEBUG
      Serial.print("Sending message #");Serial.println(msgSwitchPressed);
    #endif
    SendMessage(msgSwitchPressed);
    DrawStartupMenu();
  }

  // Check touchpad activity
  touchPressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the scree
  if (touchPressed) {
    delay(300);
    #ifdef DEBUG
       Serial.print("touchX,touchY = ");
       Serial.print(touchX);Serial.print(", ");Serial.println(touchY);
    #endif
    menuSelect = GetMainMenuChoice(touchX, touchY);
    switch (menuSelect) {
      case -1:                              // No menu selected
        break;

      case REVIEWMESSAGESPAGE:
        userMessageCount = fetchUserMessageCount();
          #ifdef DEBUG
            Serial.print("messageCount = ");Serial.println(messageCount);
          #endif
        if (userMessageCount == 0) {
          // Can't happen with current setup since they were
          // initialized with a certain number of messages
          // and you can't delete any, but...
          tft.setTextColor(TFT_RED, TFT_BLACK);
          tft.setCursor(80, 220);
          tft.print("No messages stored");
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        } 
        else {
          ShowMessages(userMessageCount);
          done.drawButton();
          while (true) {
            touchPressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
            if (touchPressed) {
              delay(300);
              #ifdef DEBUG
                Serial.print("touchX,touchY = ");
                Serial.print(touchX);Serial.print(", ");Serial.println(touchY);
              #endif
              if (done.contains(touchX, touchY)) {
                break;
              }
            }
          }
          DrawStartupMenu();
        }
        // Do we really need to break here?
        // Maybe just fall through...
        break;

      case CREATEMESSAGEPAGE:
        userMessageCount = fetchUserMessageCount();
        printf("in loop(): userMessageCount = %d\n", userMessageCount);
        // Don't do this:  If, for example, there are
        // ten messages, then FetchMessageCount() returns 11
        // and we want to add message number 11, not 12 (!)
        //messageCount++;                               // New message index
        if (userMessageCount >= MAXMESSAGES) {      // Out of message space
          tft.setTextColor(TFT_RED, TFT_BLACK);
          tft.setCursor(80, 220);
          tft.print("Out of message space");
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        } 
        else {                                      // OK to store new message
          tft.fillScreen(TFT_BLACK);
          DrawKeypad();
          // Probably would be better to define something more descriptive
          // to be used as first argument
          readKeyPad(-1, userMessageCount); // -1 means  we are not creating, not editing existing
          DrawStartupMenu();
        }
        break;

      case ASSIGNMESSAGESPAGE:
        userMessageCount = fetchUserMessageCount();
        tft.fillScreen(TFT_BLACK);
        ShowMessages(userMessageCount);
        ShowSwitchSelectionButtons();
        MakeSwitchSelections();
        DrawStartupMenu();
        break;

      case EDITMESSAGEPAGE:                                             // Edit existing message
        userMessageCount = fetchUserMessageCount();
        tft.fillScreen(TFT_BLACK);
        ShowMessages(userMessageCount);
        indexOfEditMessage = DrawPickEditMessage();
        EnterReplacementMessage(indexOfEditMessage);
        DrawStartupMenu();
        break;

      default:
        Serial.print("switch() in loop(): I shouldn't be here, mrnuSelect = ");
        Serial.println(menuSelect);
        break;
    } // End of switch(menuSelect) that handles screen touchpad
  }// End of if (touchPressed)
} // End of loop()

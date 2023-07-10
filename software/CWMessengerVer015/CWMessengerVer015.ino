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

boolean pressed;
byte switchIndexes[] = {1, 10, 9};

char final2[12];
char buffer[MAXMESSAGELENGTH] = {0};
char inputBuffer[MAXMESSAGELENGTH + 1];
extern char contestExchanges[][MAXMESSAGELENGTH + 1];

uint8_t numberIndex = 0;
uint16_t touchX = 0, touchY = 0;

int displayPage;              // 0 = startup menu, 1 = create message, 2 = set message
int ditlen;
int final_length;
volatile int encoderDirection;
int nextEEPROMMessage;
int rotationDirection;
volatile int whichOne;
int wordsPerMinute;

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
  ReviewMessages.initButton(&tft, 160,  55, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, "Review", KEY_TEXTSIZE);
  NewMessage.initButton(&tft,     160, 100, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, "Create", KEY_TEXTSIZE);
  AssignMessages.initButton(&tft, 160, 145, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, "Assign", KEY_TEXTSIZE);
  EditMessages.initButton(&tft,   160, 195, 100, 30, TFT_WHITE, TFT_BLACK, TFT_GREEN, "Edit",   KEY_TEXTSIZE);
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
  tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen

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

  //  arrowUp.drawButton();
  //  arrowDown.drawButton();

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
void MyDelay(unsigned long millisWait)
{
  unsigned long now = millis();

  while (millis() - now < millisWait)
    ;     // Twiddle thumbs...
}

/*****
  Purpose: to cause a delay in program execution

  Parameter list:
    unsigned long millisWait    // the number of millseconds to wait

  Return value:
    void
*****/
void Splash()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.setFreeFont(FSB18);
  tft.setCursor(DISPLAYWIDTH / 2 - 130, 70);
  tft.print("CW Messenger");
  tft.setFreeFont(FSB12);
  tft.setCursor(DISPLAYWIDTH / 2 - 50, 90);
  tft.print("Ver 00");
  tft.print(VERSIONNUMBER);
  tft.setFreeFont(FSB9);
  tft.setCursor(DISPLAYWIDTH / 2 - 16, 120);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("by");
  tft.setFreeFont(FSB12);
  tft.setCursor(DISPLAYWIDTH / 2 - 125, 160);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("Jack Purdum, W8TEE");
  tft.setCursor(DISPLAYWIDTH / 2 - 110, 190);
  tft.print(" Al Peter, AC8GY");
  MyDelay(20000L);
  tft.setFreeFont(FSB9);

}

/*****
  Purpose: to update the words per minute data on the display when the encoder is rotated

  Parameter list:
    unsigned long millisWait    // the number of millseconds to wait

  Return value:
    void
*****/
void UpdateWPM()
{
  wordsPerMinute += encoderDirection;
  SetDitLength(wordsPerMinute);
  tft.fillRect(10, 215, 100, 20, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(10, 230);
  tft.print("WPM: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(wordsPerMinute);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  encoderDirection = 0;
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif

  while (!EEPROM.begin(3)) {              //Allocate EEPROM memory
    true;
  }

  tft.begin();
  tft.init();
  tft.setRotation(1);

#ifdef DEBUG
  Serial.print("message count = ");
  Serial.println(FetchMessageCount());
#endif

  pinMode(DITPADDLE,           INPUT_PULLUP);
  pinMode(DAHPADDLE,           INPUT_PULLUP);


  pinMode(MSGSWITCH1,           INPUT_PULLUP);    // Message switches
  //digitalWrite(MSGSWITCH1,      HIGH);            // Done for documentation
  pinMode(MSGSWITCH2,           INPUT_PULLUP);
  //digitalWrite(MSGSWITCH2,      HIGH);
  pinMode(MSGSWITCH3,           INPUT_PULLUP);
  //digitalWrite(MSGSWITCH3,      HIGH);

  pinMode(OPTOINPUT,            OUTPUT);

  InitKeyboardButtons();

  pinMode(ENCODER1PINA,         INPUT_PULLUP);
  pinMode(ENCODER1PINB,         INPUT_PULLUP);
  pinMode(ENCODERSWITCH1,       INPUT_PULLUP);

  digitalWrite(ENCODER1PINB,    HIGH);
  digitalWrite(ENCODERSWITCH1,  HIGH);

  // Set the rotation before we calibrate

  InitKeypad(tft);         // Initialize all button objects
  InitStartupMenu();

  ReadSwitchesArray();                // Previously-stored messages indexes for switches from EEPROM
  ReadWordsPerMinute();               // Read wpm from EEPROM

  SetDitLength(MESSAGEWPM);

  displayPage = MENUDISPLAYPAGE;      // Start menu

  tft.fillScreen(TFT_BLACK);
  Splash();

  DrawStartupMenu();

  SetDitLength(MESSAGEWPM);

 /*
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH1), ReadMessageSwitch1, CHANGE);   // Message to send
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH2), ReadMessageSwitch2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MSGSWITCH3), ReadMessageSwitch3, CHANGE);
 
    attachInterrupt(digitalPinToInterrupt(DITPADDLE),    Dit,    CHANGE);   // Keyer paddles
    attachInterrupt(digitalPinToInterrupt(DAHPADDLE),    Dah,    CHANGE);
  */
  attachInterrupt(digitalPinToInterrupt(ENCODER1PINA), rotate, CHANGE);   // WPM encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER1PINB), rotate, CHANGE);

  touch_calibrate(tft);

}


void loop() 
{
  int dir, val = 0;
  int messageCount;
  int indexOfEditMessage;
  int menuSelect;
  int messageSwitch;

  while (digitalRead(DAHPADDLE) == LOW) {   // Same here
    Dah();
  }
  while (digitalRead(DITPADDLE) == LOW) {   // Send atom until released...
    Dit();
  }

  if (whichOne != -1) {
    DrawStartupMenu();      // Add appropriate code
    displayPage = MENUDISPLAYPAGE;
    whichOne = -1;    
  }

  ReadMessageSwitches();      // Returns -1 if no switch pressed

  if (whichOne != -1) {
    SendMessage(tft, whichOne);
    MyDelay(1000L);                           // Give then time to read it
    DrawStartupMenu();      // Add appropriate code
    displayPage = MENUDISPLAYPAGE;
    whichOne = -1;
  }

  touch_calibrate(tft);
  touchX = 0, touchY = 0; // To store the touch coordinates

  pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
  if (pressed && touchX != 0 && touchY != 0) {
    MyDelay(100L);

    menuSelect = GetMainMenuChoice();

    switch (menuSelect) {
      case -1:                              // No menu selected
        break;

      case MENUDISPLAYPAGE:
        messageCount = FetchMessageCount();
        if (messageCount == 0) {
          tft.setTextColor(TFT_RED, TFT_BLACK);
          tft.setCursor(80, 220);
          tft.print("No messages stored");
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        } else {
          ShowMessages(tft, messageCount);
          done.drawButton();
          while (true) {
            pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
            if (pressed) {
              MyDelay(300L);
              touchX = DISPLAYWIDTH - touchX;           // Need to reverse X coordinate
              if (done.contains(touchX, touchY)) {      // Write current entry to EEPROM
                break;
              }
            }
          }
          DrawStartupMenu();
        }
        displayPage = KEYPADINPUTPAGE;
        break;

      case KEYPADINPUTPAGE:
        messageCount = FetchMessageCount();
        messageCount++;                               // New message index
        if (messageCount > MAXMESSAGES) {      // Out of message space
          tft.setTextColor(TFT_RED, TFT_BLACK);
          tft.setCursor(80, 220);
          tft.print("Out of message space");
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        } else {                                      // OK to store new message
          tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen
          DrawKeypad(tft);
          readKeyPad(tft, -1, messageCount);
          //          displayKbdOut(tft);
          DrawStartupMenu();
          displayPage = MESSAGESELECTPAGE;
        }
        break;

      case ASSIGNMESSAGES:
        messageCount = FetchMessageCount();
        tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen
        ShowMessages(tft, messageCount);
        ShowSwitchSelectionButtons(tft);
        MakeSwitchSelections(tft);
        DrawStartupMenu();
        break;

      case MESSAGESELECTPAGE:
        DrawStartupMenu();      // Add appropriate code
        displayPage = MENUDISPLAYPAGE;
        break;

      case EDITMESSAGE:                                             // Edit existing message
        messageCount = FetchMessageCount();
        tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen
        ShowMessages(tft, messageCount);
        indexOfEditMessage = DrawPickEditMessage(tft);
        EnterReplacementMessage(tft, indexOfEditMessage);
        DrawStartupMenu();
        break;

      default:
        Serial.print("I shouldn't be here, displayPage = ");
        Serial.println(displayPage);
        break;
    }
  }
}

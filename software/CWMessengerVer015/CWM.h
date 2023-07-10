
#define TOUCH_CS                // Needed for extensions

#include <EEPROM.h>
#include "Free_Fonts.h"         // Include the header file attached to this sketch. See: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include "FS.h"
#include <TFT_eSPI.h>           // https://github.com/Bodmer/TFT_eSPI  Hardware-specific library
#include <rom/gpio.h>
#include <Rotary.h>             // https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/examples/interrupt/interrupt.ino

//#define DEBUG                   // Comment out when not debugging

#define VERSIONNUMBER           14

#define TFT_MISO                19    // From ESP32 TFT User_Setup.h
#define TFT_MOSI                23
#define TFT_SCLK                18
#define TFT_CS                  15    // Chip select control pin
#define TFT_DC                   2    // Data Command control pin
#define TFT_RST                  4    // Reset pin (could connect to RST pin)


//#define TFT_RST                 -1    // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define MESSAGEWPM              15    // Default sending speed
#define MAXWPM                  60
#define MINWPM                  5

#define OPTOINPUT               32    // Keyed circuit, via optoisolator

#define MSGSWITCH1              22    // The message NO pushbutton switches
#define MSGSWITCH2              17
#define MSGSWITCH3              16

#define ENCODERSWITCH1          27    // Encoder pins
#define ENCODER1PINA            25     
#define ENCODER1PINB            26 

#define DITPADDLE               13    // I/O pins for paddle
#define DAHPADDLE               33

#define ELEMENTCOUNT(x)  (sizeof(x) / sizeof(x[0]))

#define MENUDISPLAYPAGE         1
#define KEYPADINPUTPAGE         2
#define ASSIGNMESSAGES          3
#define EDITMESSAGE             4

#define UPARROWBUTTON           0
#define DOWNARROWBUTTON         1
#define SWITCH1BUTTON           2
#define SWITCH2BUTTON           3
#define SWITCH3BUTTON           4
#define DONEBUTTON              5

#define SELECTEDITMESSAGE       6

#define MESSAGESELECTPAGE       6

#define MAXMESSAGELENGTH       30                             // Max size for each message
#define MAXMESSAGES            15

#define KEY_X                   20// Centre of key Keypad start position, key sizes and spacing
#define KEY_Y                   100
#define KEY_W                   21 // Width and height
#define KEY_H                   25
#define KEY_SPACING_X           7 // X and Y gap
#define KEY_SPACING_Y           6
#define KEY_TEXTSIZE            1   // Font size multiplier
#define LABEL1_FONT             &FreeSansOblique12pt7b // Key label font 1 Using two fonts since numbers are nice when bold
#define LABEL2_FONT             &FreeSansBold12pt7b    // Key label font 2
#define KEYPADXOFFSET           5
#define KEYPADYOFFSET          -5
//#define NUM_LEN               20  // Number length, buffer for storing it and character index
#define INPUTX                  1   // Numeric display box size and location
#define INPUTY                  10
#define INPUTWIDTH              198
#define INPUTHEIGHT             50
#define DISPLAYWIDTH            480   // 320
#define DISPLAYHEIGHT           320   // 240
#define DISP_TSIZE              3
#define DISP_TCOLOR             TFT_CYAN

#define CALIBRATION_FILE  "/TouchCalData2"
#define REPEAT_CAL        false

extern boolean pressed;

extern uint8_t numberIndex;
extern byte switchIndexes[];

extern int ditlen;
extern int rotationDirection;
extern uint16_t keyColor1[];

extern char buffer[];
extern char contestExchanges[][MAXMESSAGELENGTH + 1];
extern char final2[];
extern char inputBuffer[];
extern char letterTable[];
extern char numberTable[];
extern char punctuationTable[];
extern char testString[];

extern uint16_t touchX, touchY;

extern int ASCIIForPunctuation[];
extern int displayPage;              // 0 = startup menu, 1 = create message, 2 = set message
extern volatile int encoderDirection;
extern int final_length;
extern int nextEEPROMMessage;
extern int rotationDirection;
extern volatile int whichOne;
extern int wordsPerMinute;


extern TFT_eSPI tft;
extern TFT_eSPI_Button key[];
extern TFT_eSPI_Button clearText;
extern TFT_eSPI_Button spaceBar;
extern TFT_eSPI_Button store;
extern TFT_eSPI_Button cancel;
extern TFT_eSPI_Button arrowUp;
extern TFT_eSPI_Button arrowDown;
extern TFT_eSPI_Button switch1;
extern TFT_eSPI_Button switch2;
extern TFT_eSPI_Button switch3;
extern TFT_eSPI_Button switch3;
extern TFT_eSPI_Button done;
extern TFT_eSPI_Button selectEditMessage;

extern TFT_eSPI_Button NewMessage;
extern TFT_eSPI_Button ReviewMessages;
extern TFT_eSPI_Button AssignMessages;
extern TFT_eSPI_Button EditMessages;

extern Rotary rotary;

//==================================== Function Prototypes ======================================
void Dah();
void displayKbdOut(TFT_eSPI tft);
void DisplayMessages(int messageCount);
void Dit();
void DrawGraphGrid(TFT_eSPI tft);
void DrawKeypad(TFT_eSPI tft);
int  DrawPickEditMessage(TFT_eSPI tft);
void DrawStartupMenu();
void EnterReplacementMessage(TFT_eSPI tft, int messageToReplaceIndex);
int  FetchMessageCount();
int  GetMainMenuChoice();
void InitKeyboardButtons();
void InitKeypad(TFT_eSPI tft);
void LetterSpace();
void MakeSwitchSelections(TFT_eSPI tft);
void MyDelay(unsigned long millisWait);
//int  ProcessEncoder();
void readKeyPad(TFT_eSPI tft, int editFlag, int messageCount);
void ReadMessageSwitches();
void ReadMessageSwitch1();
void ReadMessageSwitch2();
void ReadMessageSwitch3();
void ReadSwitchesArray();
int  ReadWordsPerMinute();
void rotate();
void SaveEditMessage(char *inputBuffer, int editFlag);
void Send(char myChar);
void SendCode(char code);
void SendLetters(TFT_eSPI tft, char *msg);
void SendLettersAndNumbers(TFT_eSPI menu);
void SendMessage(TFT_eSPI tft, int messageSwitch);
void SendNumbers(TFT_eSPI menu);
void SendPunctuation(TFT_eSPI menu);
void SetDitLength(int wpm);
void ShowMessages(TFT_eSPI tft, int messageCount);
void ShowSwitchMessageNumbers(TFT_eSPI tft);
void ShowSwitchSelectionButtons(TFT_eSPI tft);
void touch_calibrate(TFT_eSPI tft);
void UpdateSwitchesArray();
void UpdateWPM();
int  WhichMessageButtonPressed(uint16_t touchX, uint16_t touchY);
void WordSpace();
void WriteWordsPerMinute();

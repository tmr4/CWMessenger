
//#define TOUCH_CS                // Define this in designated user setup header. Needed for extensions

#include <EEPROM.h>
#include "Free_Fonts.h"         // Include the header file attached to this sketch. See: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include "FS.h"
#include <TFT_eSPI.h>           // https://github.com/Bodmer/TFT_eSPI  Hardware-specific library
#include <rom/gpio.h>
#include <Rotary.h>             // https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/examples/interrupt/interrupt.ino


//#define DEBUG                   // Comment out when not debugging

// To calibrate the touch screen, #define REPEAT_CAL true and run
// the program.  Then come back here and #define REPEAT_CAL false
// for normal program execution.
#define REPEAT_CAL    false

// From Dave, KW7X: Al's version number and my point release
#define VERSIONNUMBER           "15.1" //14

// TFT Stuff is defined in the TFT setup file, so I won't repeat it here
// I'll leave the commented-out lines  for reference
//#define TFT_MISO                19    // Hardware SPI data input from Touch pad
//#define TFT_MOSI                23    // Hardware SPI data to LCD and Touch pad
//#define TFT_SCLK                18    // Hardware SPI clock to LCD and Touch pad
//#define TFT_CS                  15    // Chip select control pin to LCD
//#define TFT_DC                   2    // Data/Command control pin to LCD
//#define TFT_RST                  4    // Reset pin (could connect to RST pin)
//  or
//#define TFT_RST                 -1    // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

// I/O pin definitions
#define OPTOINPUT               32    // Keyed circuit output, via optoisolator

// I wired it as shown on the schematic in the book.
// These things are different.  I'll leave the commented-out
// originals for reference
//#define MSGSWITCH1              22    // The message NO pushbutton switches
//#define MSGSWITCH2              17
//#define MSGSWITCH3              16
#define MSGSWITCH1              33    // The message NO pushbutton switches
#define MSGSWITCH2              25
#define MSGSWITCH3              26

// Same for encoder.  I'll  change to use pins
// shown on schematic
//#define ENCODERSWITCH1          27    // Encoder pins
//#define ENCODER1PINA            25
//#define ENCODER1PINB            26

// Note that pins 34-39 do NOT have pullups.  External pullups are required
#define ENCODERSWITCH1          39    // Encoder pins
#define ENCODER1PINA            34
#define ENCODER1PINB            35

// I have the dit paddle connected to the Ring contact
// of the Phone Jack.  So I'll leave the original
// definition alone
#define DITPADDLE               13    // I/O pins for paddle

// The DAH is connected to the tip, which is GPIO27 on the
// schematic, so I'll change it:
//#define DAHPADDLE               33
#define DAHPADDLE               27

// TODO (maybe): Add a "straight key" input.
// In addition to have ability to use a straight key, could
// use output from your favorite keyer or from the transmitter
// key output from your favorite radio with builtin keyer.

// Manifest constants used in the program
#define MESSAGEWPM              15    // Default sending speed
#define MAXWPM                  60
#define MINWPM                  5

// Dangerous because some people might erroneously think they
// can use it in a function whose parameter was a pointer!
// Not used in  this program anyhow...
//#define ELEMENTCOUNT(x)  (sizeof(x) / sizeof(x[0]))

// Might be more elegant (and, in C++, more safe) tp
// make these part of an enum
// These values are returned by GetMainMenuChoice() and
// are used in loop() to go to the selected submenu
#define REVIEWMESSAGESPAGE      1
#define CREATEMESSAGEPAGE       2
#define ASSIGNMESSAGESPAGE      3
#define EDITMESSAGEPAGE         4

// These are areturned by WhichMessageButtonPressed(), which
// is called by DrawPickEditMessage() when editing a message
#define UPARROWBUTTON           0
#define DOWNARROWBUTTON         1
#define SWITCH1BUTTON           2
#define SWITCH2BUTTON           3
#define SWITCH3BUTTON           4
#define DONEBUTTON              5
#define SELECTEDITMESSAGE       6


#define MAXMESSAGELENGTH       30     // Max size for each message

// I changed MAXMESSAGES to 14 since that's all I can get on the screen
// TODO: Make a scroll routine when editing so that can get more messages
#define MAXMESSAGES             14 //15

#define KEY_X                   20 // Centre of key Keypad start position, key sizes and spacing
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
#define DISPLAYWIDTH            320  // For my 320x240 display //480 For 480x320 display
#define DISPLAYHEIGHT           240  // For my 320x240 display //320 For 480x320 display
#define DISP_TSIZE              3

#define CALIBRATION_FILE  "/TouchCalData2"


// Global variables

// These could be un-globalized by putting getter/setter
// functions in RotaryCode.cpp, but I'll leave them
// alone for now
extern int ditlen;
extern int wordsPerMinute;

extern byte switchIndexes[];
extern char contestExchanges[][MAXMESSAGELENGTH + 1];
extern char letterTable[];
extern char numberTable[];

// Make objects global.  There is no possibility of confusion  since each is unique,
// and dragging them along as function arguments is more trouble than it's worth
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

// All of the functions  with TFT_eSPI were changed to call-by-referenct
// In principle they could all be "const TFT_sPI &" but many of the
// library functions are not const-correct.
void Dah(void);
void displayKbdOut(char *inputBuffer);
void Dit(void);
void DrawGraphGrid(void);
void DrawKeypad(void);
int  DrawPickEditMessage(void);
void DrawStartupMenu(void);
void EnterReplacementMessage(int messageToReplaceIndex);
int  fetchUserMessageCount(void);
int  GetMainMenuChoice(uint16_t touchX, uint16_t touchY);
void InitKeyboardButtons(void);
void InitKeypad(void);
void LetterSpace(void);
void MakeSwitchSelections(void);

//int  ProcessEncoder(void);
void readKeyPad(int editFlag, int messageCount);
int ReadMessageSwitches(void); // Was void return type and set global variable
void ReadMessageSwitch1(void);
void ReadMessageSwitch2(void);
void ReadMessageSwitch3(void);
void ReadSwitchesArray(void);
int  ReadWordsPerMinute(void);
void encoderISR(void);
void SaveEditMessage(char *inputBuffer, int editFlag);
void Send(char myChar);
void SendCode(char code);
void SendLetters(char *msg);
// Not implemented and not used
//void SendLettersAndNumbers(void);
void SendMessage(int messageSwitch);
// Not implemented and not used
//void SendNumbers(void);
//void SendPunctuation(void);
void SetDitLength(int wpm);
void ShowMessages(int messageCount);

// Only used in MessageProcessing.cpp
//void ShowSwitchMessageNumbers(void);
void ShowSwitchSelectionButtons(void);
void touch_calibrate(void);
void UpdateSwitchesArray(void);
void UpdateWPM(void);
int  WhichMessageButtonPressed(uint16_t touchX, uint16_t touchY);
int  WhichEditScreenButtonPressed(uint16_t touchX, uint16_t touchY);
// Only used in ProcessCode
//void WordSpace(void);
// Only used in ProcessEEPROM
//void WriteWordsPerMinute(void);

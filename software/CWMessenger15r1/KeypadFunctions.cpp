#include "CWM.h"
#include "Messages.h"           // Stored CW messages


uint16_t keyColor1[40] = {TFT_PURPLE, TFT_PURPLE, TFT_PURPLE, TFT_PURPLE,
                          TFT_PURPLE, TFT_PURPLE, TFT_PURPLE, TFT_PURPLE,
                          TFT_PURPLE, TFT_PURPLE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         };
/*
  uint16_t keyColor2[40] = {TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                         };
*/
unsigned int keyColor2[40];

char *keyLabel[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
                    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
                    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";",
                    " ", "Z", "X", "C", "V", "B", "N", "M", ",", "."
                   };

/*****
  Purpose: To clear buttons to black.

  Parameter list:
    TFT_eSPI tft      display object

  Return value:
    void
*****/
void InitKeyboardButtons(void)
{
  memset(keyColor2, 0xFF, sizeof(keyColor2));
}

/*****
  Purpose: To instantiate button objects

  Parameter list:
    TFT_eSPI tft      display object

  Return value:
    void
*****/
void InitKeypad(void)
{
  int b;
  int col, row;
  int spaceOffX, spaceOffY, xOff, yOff;

  tft.setFreeFont(FSB9);

  xOff = KEYPADXOFFSET - 3 + KEY_X;
  yOff = KEYPADYOFFSET + KEY_Y - 12;
  spaceOffX = (KEY_W + KEY_SPACING_X + 3);
  spaceOffY = (KEY_H + KEY_SPACING_Y - 3);
  for (row = 0; row < 4; row++) {
    for (col = 0; col < 10; col++) {
      b = col + row * 10;
      key[b].initButton(&tft, xOff + col * spaceOffX,
                        yOff + row * spaceOffY,
                        KEY_W + 6, KEY_H, TFT_WHITE, keyColor1[b], keyColor2[b],
                        keyLabel[b], KEY_TEXTSIZE);// x, y, w, h, outline, fill, text
    }
  }
  clearText.initButton(&tft, 50, 215, 70, 30, TFT_WHITE, TFT_RED, TFT_WHITE,
                       "Clear",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text

  spaceBar.initButton(&tft, 155, 195, 125, KEY_H - 5, TFT_WHITE, TFT_BLUE, TFT_WHITE,
                      " ",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text

  store.initButton(&tft, 270, 215, 70, 30, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE,
                   "Store",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text

  cancel.initButton(&tft, 160, 218, 95, KEY_H - 1, TFT_WHITE, TFT_YELLOW, TFT_BLACK,
                    "Cancel", KEY_TEXTSIZE);

  arrowUp.initButton(&tft, 290, 175, 40, KEY_H + 5, TFT_WHITE, TFT_BLACK, TFT_GREEN,
                     "^", KEY_TEXTSIZE);

  arrowDown.initButton(&tft, 290, 215, 40, KEY_H + 5, TFT_WHITE, TFT_BLACK, TFT_GREEN,
                       "v", KEY_TEXTSIZE);

  switch1.initButton(&tft, 50, 220, 50, KEY_H - 1, TFT_WHITE, TFT_MAGENTA, TFT_BLACK,
                     "SW1", KEY_TEXTSIZE);
  switch2.initButton(&tft, 110, 220, 50, KEY_H - 1, TFT_WHITE, TFT_MAGENTA, TFT_BLACK,
                     "SW2", KEY_TEXTSIZE);
  switch3.initButton(&tft, 170, 220, 50, KEY_H - 1, TFT_WHITE, TFT_MAGENTA, TFT_BLACK,
                     "SW3", KEY_TEXTSIZE);
  done.initButton(&tft, 239, 220, 50, KEY_H - 1, TFT_WHITE, TFT_GREEN, TFT_BLACK,
                  "Done", KEY_TEXTSIZE);
  //selectEditMessage.initButton(&tft, 110, 190, 190, 30, TFT_WHITE, TFT_YELLOW, TFT_BLACK,
                               //"Edit highlighted",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text
  // Can't use strings longer than 9 chars in button label.  I'll just use "Edit" 
  // Also, I'll make it a little skinnier and place it a little further down so
  // that I can display another message (in case one was added).                              
  selectEditMessage.initButton(&tft, 160, 218, 95, KEY_H - 1, TFT_WHITE, TFT_YELLOW, TFT_BLACK,
                               "Edit Hi",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text
}

/*****
  Purpose: Draw on-screen numeric keypad

  Paramter list:
    TFT_eSPI tft      display object

  Return value:
    void
*****/
void DrawKeypad(void)
{
  int b;
  int col, row;
  int spaceOffX, spaceOffY, xOff, yOff;

  tft.setFreeFont(FSB9);

  xOff = KEYPADXOFFSET - 3 + KEY_X;
  yOff = KEYPADYOFFSET + KEY_Y - 12;
  spaceOffX = (KEY_W + KEY_SPACING_X + 3);
  spaceOffY = (KEY_H + KEY_SPACING_Y - 3);
  for (row = 0; row < 4; row++) {
    for (col = 0; col < 10; col++) {
      b = col + row * 10;
      key[b].drawButton();
    }
  }
  clearText.drawButton();
  spaceBar.drawButton();
  store.drawButton();
  cancel.drawButton();
}

/*****
  Purpose: Read the key that was pressed and take appropriate action

  Paramter list:
    TFT_eSPI tft        a display object
    int editFlag        -1 if not editing existing entry, 
                        if editFlag is zero, messageCount indicates which message we are editing

    int messageCount    the number of lines in the array. including the blank  line at the beginning

  Return value:
    void
*****/
void readKeyPad(int editFlag, int userMessageCount)
{
  uint8_t pressed;
  uint16_t touchX, touchY;
  char inputBuffer[MAXMESSAGELENGTH + 1];
  int numberIndex = 0;
  #ifdef DEBUG
    Serial.print("readKeyPad: userMessageCount = ");Serial.println(userMessageCount);
  #endif
  strcpy(inputBuffer,contestExchanges[0]); // Blankline (Is this really necessary?)
  if (editFlag == 0) {
    tft.setCursor(5, 15);
    tft.print(contestExchanges[0]); // Blank line
  }
  else {
    // The calling program should have checked, but...
    if (userMessageCount >= MAXMESSAGES) {
      tft.print("Can't add more messages. Message buffer is full!");
      delay(5000);
      return;
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 15);
    tft.print("Adding message ");
    tft.print(++userMessageCount);
    tft.setCursor(5, 15);
    tft.print(contestExchanges[0]); // Blank line
  }
  while (true) {
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      delay(300);
      for (uint8_t b = 0; b < 40; b++) {
        if (key[b].contains(touchX, touchY)) {
          #ifdef DEBUG          
            Serial.print("touchX = "); Serial.print(touchX);
            Serial.print("   touchY = "); Serial.print(touchY);
            Serial.print("  keyLabel["); Serial.print(b); Serial.print("] = ");
            Serial.println(keyLabel[b]);
          #endif          
          key[b].press(true);  // tell the button it is pressed
        }
        else {
          key[b].press(false);  // tell the button it is NOT pressed
        }
      }

      // Check if any key has changed state
      for (uint8_t b = 0; b < 40; b++) {
        if (key[b].justReleased()) {
          key[b].drawButton();     // draw normal
        }
        if (key[b].justPressed()) {
          key[b].drawButton(true);  // draw invert
          if (numberIndex < MAXMESSAGELENGTH) {
            inputBuffer[numberIndex++] = keyLabel[b][0];
            inputBuffer[numberIndex] = '\0'; // zero terminate
          }
          displayKbdOut(inputBuffer);
          key[b].drawButton();
          key[b].press(false);  // tell the button it is no longer pressed; button has been processed.
        }
      }
      if (clearText.contains(touchX, touchY)) {                                       // Erase previous entry
        clearText.press(true);              // tell the button it is pressed
      }
      else {
        clearText.press(false);             // tell the button it is NOT pressed
      }
      if (clearText.justReleased())         // button release
        clearText.drawButton();

      if (clearText.justPressed()) {        // If they pressed it, clear entry
        clearText.drawButton(true);         // draw invert
        numberIndex = 0; // Reset index to 0
        // Or you could just copy contestExchanges[0] since it is a blank line!
        memset(inputBuffer, ' ', MAXMESSAGELENGTH);
        // But dimension of inputBuffer is MAXMESSAGELENGTH+1,
        // so last element has indes MAXMESSAGELENGTH
        //inputBuffer[MAXMESSAGELENGTH - 1] = '\0';       // Place null in buffer
        inputBuffer[MAXMESSAGELENGTH] = '\0';       // Place null in buffer
        
        displayKbdOut(inputBuffer);
      }

      if (spaceBar.contains(touchX, touchY)) {                                        // Process spacebar insert
        spaceBar.press(true);                  // tell the button it is pressed
      }
      else {
        spaceBar.press(false);                 // tell the button it is NOT pressed
      }
      if (spaceBar.justReleased())
        spaceBar.drawButton();
      if (spaceBar.justPressed()) {
        spaceBar.drawButton(true);            // draw invert
        inputBuffer[numberIndex] = ' ';       // Place space in buffer
        numberIndex++;
        inputBuffer[numberIndex] = '\0';      // zero terminate
        displayKbdOut(inputBuffer);
      }
      if (pressed && cancel.contains(touchX, touchY)) {
        cancel.press(true);
        return;
      }

      if (pressed && store.contains(touchX, touchY)) {                      // Write current entry to EEPROM
        store.press(true);  // tell the button it is pressed
      } 
      else {
        store.press(false);  // tell the button it is NOT pressed
      }
      if (store.justReleased()) {                                             // The want to save it
        store.drawButton();
      }
      if (store.justPressed()) {
        store.drawButton(true);
        #ifdef DEBUG
          Serial.print("SaveEditMessage(<");Serial.print(inputBuffer);Serial.print(">,");
          Serial.print(userMessageCount);Serial.println(")");
        #endif
        // NO!  You gotta give the address of the message you are replacing!
        // With editFlag = 0 it writes over the blank line.
        //SaveEditMessage(inputBuffer, editFlag);
        // If the user pressed "Save" before entering enthing, it screws the pooch
        // if you put this empty line in the messages array!
        if (strlen(inputBuffer) > 0) {
          SaveEditMessage(inputBuffer, userMessageCount);
        }
        store.press(false);
        return;
      }
    }
  }
}


/*****
  Purpose: Calibrate Touch Screen

  Paramter list:
    TFT_eSPI tft    display object

  Return value:
    void
*****/
void touch_calibrate(void)
{
  // Must make this seven, not five, since we read and write 14 bytes!
  uint16_t calData[5];
  uint8_t calDataOK = 0;
  int num;
  // check file system exists
  if (!SPIFFS.begin()) {
#ifdef DEBUG    
    Serial.println("Formating file system");
#endif    
    SPIFFS.format();
    SPIFFS.begin();
  }
  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        // calibrateTouch fills in five 16-bit integers
        if ((num = f.readBytes((char *)calData, 10)) == 10) {
          calDataOK = 1;
        }
        else {
          Serial.print("Error reading calData, readbytes returned ");
          Serial.println(num);
        }
        f.close();
      }
    }
  }
  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
        tft.setTouch(calData);
  } 
  else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();
    // calibrateTouch fills in five 16-bit integers to the calData array
    
    tft.calibrateTouch(calData, TFT_PURPLE, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Calibration complete!");
    // store data but don't start too soon!
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 10);
      f.close();
    }
    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("\n\nSet REPEAT_CAL in CWM.h to");
      tft.println("false then recompile to");
      tft.println("stop this from running");
      tft.println("again!");
      while (1)
        ;
    }
  }
} // End of touch_calibrate()


/*****
  Purpose: Display Keypad selection in Keypad display
           Uses global inputBuffer[]

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    void
*****/
/* It actually should be const TFT_eSPI, but the library print function
 *  is not const-correct.  Pity
 */
void displayKbdOut(char *inputBuffer)
{

  tft.fillRect(KEYPADXOFFSET + INPUTX + 3, KEYPADYOFFSET + INPUTY + 12, INPUTWIDTH - 38, INPUTHEIGHT - 15, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(FSB9);
  tft.setCursor(KEYPADXOFFSET + 5, KEYPADYOFFSET + INPUTY + 40);
  tft.print(inputBuffer);
}

/*****
  Purpose: qwhich of the main menu choices was selected

  Paramter list:
    void

  Return value:
    int               which was selected
*****/
int GetMainMenuChoice(uint16_t touchX, uint16_t touchY)
{
  #ifdef DEBUG
    Serial.print("touchX = "); Serial.print(touchX);Serial.print(", touchY = ");Serial.println(touchY);
  #endif
  if (ReviewMessages.contains(touchX, touchY)) {
    #ifdef DEBUG
      Serial.println("ReviewMessages");
    #endif
    return REVIEWMESSAGESPAGE;
  }
  if (NewMessage.contains(touchX, touchY)) {
    #ifdef DEBUG
      Serial.println("NewMessage");
    #endif
    return CREATEMESSAGEPAGE;
  }
  if (AssignMessages.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    #ifdef DEBUG
      Serial.println("AssignMessages");
    #endif
    return ASSIGNMESSAGESPAGE;
  }
  if (EditMessages.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    #ifdef DEBUG
      Serial.println("EditMessages");
    #endif
    return EDITMESSAGEPAGE;
  }
  return -1;                                                      // Nothing found
}



/*****
  Purpose: Function to get the message number to be edited.

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    int           edit message index
*****/
int DrawPickEditMessage(void)
{
  uint8_t pressed;
  uint16_t touchX, touchY;
  int flag, index, whichButton, row;
  static int highlightedRow, oldRow;
  int numUserMessages = fetchUserMessageCount();
  
  row = switchIndexes[0] * 15 + 15;
  index = switchIndexes[0];
  highlightedRow = row;
  selectEditMessage.drawButton();
  arrowUp.drawButton();
  arrowDown.drawButton();

  // I'll stay here until the user presses one of the buttons
  while (true) { // Main loop
    // / Check if any key coordinate boxes contain the touch coordinates
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      delay(300);
      whichButton = WhichEditScreenButtonPressed(touchX, touchY);
      switch (whichButton) {
        case UPARROWBUTTON:
          tft.setCursor(5, highlightedRow);         // Redraw old message to unselected color
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          if (index < 10) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);

          // Instead of wrapping, just leave it  on the first row
          if (index > 1) {
            --index;
          }
          // Assumes row spacing is 15 pixels
          highlightedRow = index * 15 + 15;
          tft.setCursor(5, highlightedRow);         // Draw new message
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          if (index < 10) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);
          break;

        case DOWNARROWBUTTON:
          #ifdef DEBUG
            Serial.print("index = ");Serial.print(index);Serial.print(", msgcnt = ");Serial.println(msgcnt);
            Serial.print("highlightedRow = ");Serial.println(highlightedRow);
          #endif
          // If we are already as low as we can go, don't do anything
          // If not, then we will unhighlight the previous message
          // and draw the next one highlighted
          if (index < numUserMessages) {
            tft.setCursor(5, highlightedRow);         // Redraw old message to unselected color
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            if (index < 10) {
              tft.print("  ");
            }
            tft.print(index);
            tft.print("  ");
            tft.print(contestExchanges[index]);
            ++index;
            highlightedRow = index * 15 + 15;
            tft.setCursor(5, highlightedRow);         // Newly highlighted
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            if (index < 10) {
              tft.print("  "); // "Empty" tens digit is two space charss wide
            }
            tft.print(index);
            tft.print("  ");
            tft.print(contestExchanges[index]);
          } // End of going to the next row down
          break;

        case SELECTEDITMESSAGE: // 6
          return index;
          break;
      } // End of switch()
    } // End if if(pressed)
  } // End of Main loop
  // Can't get here since the main loop is while (1) but
  // sometimes compilers like to see return statements with values
  return -1;
} // End of DrawPickEditMessage()

/*****
  Purpose: Function to get the message number to be edited.

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    int           edit message index
*****/
void EnterReplacementMessage(int messageToReplaceIndex)
{
  tft.fillScreen(TFT_BLACK);

  DrawKeypad();
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  //  tft.setCursor(5, 15);
  //  tft.print(contestExchanges[0]);
  tft.setCursor(5, 15);
  tft.print(contestExchanges[messageToReplaceIndex]);

  // Probably would be better to #define something descriptive be used
  // as the first argument
  readKeyPad(0, messageToReplaceIndex); // 0 means we are editing, not creating

}

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
                    " ", "Z", "X", "V", "V", "B", "N", "M", ",", "."
                   };

/*****
  Purpose: To clear buttons to black.

  Parameter list:
    TFT_eSPI tft      display object

  Return value:
    void
*****/
void InitKeyboardButtons()
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
void InitKeypad(TFT_eSPI tft)
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
  done.initButton(&tft, 240, 220, 50, KEY_H - 1, TFT_WHITE, TFT_GREEN, TFT_BLACK,
                  "Done", KEY_TEXTSIZE);
  selectEditMessage.initButton(&tft, 110, 190, 190, 30, TFT_WHITE, TFT_YELLOW, TFT_BLACK,
                               "Edit highlighted",  KEY_TEXTSIZE);// x, y, w, h, outline, fill, text

}

/*****
  Purpose: Draw on-screen numeric keypad

  Paramter list:
    TFT_eSPI tft      display object

  Return value:
    void
*****/
void DrawKeypad(TFT_eSPI tft)
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
    int editFlag        -1 if not editing existing entry, but adding new. Non-zero if editing and this is index
                          into contestMessage[] array.
    int messageCount    the number of messages in the array

  Return value:
    void
*****/
void readKeyPad(TFT_eSPI tft, int editFlag, int messageCount)
{
  if (editFlag == 0) {
    tft.setCursor(5, 15);
    tft.print(contestExchanges[0]);
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 15);
    tft.print("Adding message ");
    tft.print(messageCount);
  }
  while (true) {
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      MyDelay(300L);
      touchX = DISPLAYWIDTH - touchX;           // Need to reverse X coordinate
      for (uint8_t b = 0; b < 40; b++) {
        if (key[b].contains(touchX, touchY)) {
#ifdef DEBUG          
          Serial.print("touchX = ");
          Serial.print(touchX);
          Serial.print("   touchY = ");
          Serial.print(touchY);
          Serial.print("  keyLabel[");
          Serial.print(b);
          Serial.print("] = ");
          Serial.println(keyLabel[b]);
#endif          
          key[b].press(true);  // tell the button it is pressed
        } else {
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
          displayKbdOut(tft);
          key[b].drawButton();
          key[b].press(false);  // tell the button it is no longer pressed; button has been processed.
        }
      }
      if (clearText.contains(touchX, touchY)) {                                       // Erase previous entry
        clearText.press(true);              // tell the button it is pressed
      } else {
        clearText.press(false);             // tell the button it is NOT pressed
      }
      if (clearText.justReleased())         // button release
        clearText.drawButton();

      if (clearText.justPressed()) {        // If they pressed it, clear entry
        clearText.drawButton(true);         // draw invert
        numberIndex = 0; // Reset index to 0
        memset(inputBuffer, ' ', MAXMESSAGELENGTH);
        inputBuffer[MAXMESSAGELENGTH - 1] = '\0';       // Place null in buffer
        displayKbdOut(tft);
      }

      if (spaceBar.contains(touchX, touchY)) {                                        // Process spacebar insert
        spaceBar.press(true);                  // tell the button it is pressed
      } else {
        spaceBar.press(false);                 // tell the button it is NOT pressed
      }
      if (spaceBar.justReleased())
        spaceBar.drawButton();
      if (spaceBar.justPressed()) {
        spaceBar.drawButton(true);            // draw invert
        inputBuffer[numberIndex] = ' ';       // Place null in buffer
        numberIndex++;
        inputBuffer[numberIndex] = '\0';      // zero terminate
        displayKbdOut(tft);
      }
      if (pressed && cancel.contains(touchX, touchY)) {
        cancel.press(true);
        return;
      }

      if (pressed && store.contains(touchX, touchY)) {                      // Write current entry to EEPROM
        store.press(true);  // tell the button it is pressed
      } else {
        store.press(false);  // tell the button it is NOT pressed
      }
      if (store.justReleased())                                             // The want to save it
        store.drawButton();
      if (store.justPressed()) {
        store.drawButton(true);
        displayPage = MENUDISPLAYPAGE;
        SaveEditMessage(inputBuffer, editFlag);
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
void touch_calibrate(TFT_eSPI tft)
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;
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
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }
  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
//    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();
    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }
//    tft.calibrateTouch(calData, TFT_PURPLE, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");
    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

/*****
  Purpose: Display Keypad selection in Keypad display

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    void
*****/
void displayKbdOut(TFT_eSPI tft)
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
int GetMainMenuChoice()
{
  if (ReviewMessages.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    return MENUDISPLAYPAGE;
  }
  if (NewMessage.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    return KEYPADINPUTPAGE;
  }
  if (AssignMessages.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    return ASSIGNMESSAGES;
  }
  if (EditMessages.contains(touchX, touchY)) {                      // Write current entry to EEPROM
    return EDITMESSAGE;
  }
  return -1;                                                      // Nothing found
}


/*****
  Purpose: If messages have been stored to EEPROM, this function scrolls through that list

  Paramter list:
    int messageCount    The number of messages that have been stored

  Return value:
    int               which was selected
*****/
void DisplayMessages(int messageCount)
{

}

/*****
  Purpose: Function to get the message number to be edited.

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    int           edit message index
*****/
int DrawPickEditMessage(TFT_eSPI tft)
{
  int flag, index, oldIndex, whichButton, row;
  static int highlightedRow, oldRow;

  row = switchIndexes[0] * 15 + 15;
  oldIndex = index = switchIndexes[0];
  highlightedRow = row;
  selectEditMessage.drawButton();
  arrowUp.drawButton();
  arrowDown.drawButton();

  while (true) {
    // / Check if any key coordinate boxes contain the touch coordinates
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      MyDelay(300L);
      touchX = DISPLAYWIDTH - touchX;
      whichButton = WhichMessageButtonPressed(touchX, touchY);
      switch (whichButton) {
        case UPARROWBUTTON:                         // 0
          tft.setCursor(5, highlightedRow);         // Redraw old message to unselected color
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          if (index < MAXMESSAGES) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);

          if (index == 1) {                          // If at the first one and an Up arrow, wrap around
            index = MAXMESSAGES;
          } else {
            index--;
          }
          highlightedRow = index * 15 + 15;
          tft.setCursor(5, highlightedRow);         // Draw new message
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          if (index < MAXMESSAGES) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);
          break;

        case DOWNARROWBUTTON:                       // 1
          tft.setCursor(5, highlightedRow);         // Redraw old message to unselected color
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          if (index < MAXMESSAGES) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);

          if (index == MAXMESSAGES) {           // At the last index, wrap to first index
            index = 1;
          } else {
            index++;
          }
          highlightedRow = index * 15 + 15;
          tft.setCursor(5, highlightedRow);         // Draw old message
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          if (index < 10) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);
          break;

        case SELECTEDITMESSAGE:                     // 5
          return index;
          break;
      }
    }
  }
}

/*****
  Purpose: Function to get the message number to be edited.

  Paramter list:
    TFT_eSPI tft      the display object

  Return value:
    int           edit message index
*****/
void EnterReplacementMessage(TFT_eSPI tft, int messageToReplaceIndex)
{
  tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen

  DrawKeypad(tft);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  //  tft.setCursor(5, 15);
  //  tft.print(contestExchanges[0]);
  tft.setCursor(5, 15);
  tft.print(contestExchanges[messageToReplaceIndex]);

  //  readKeyPad(tft,  messageToReplaceIndex, 0);
  readKeyPad(tft,  0, 0);

}

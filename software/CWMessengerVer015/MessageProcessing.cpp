#include "CWM.h"

extern char contestExchanges[][MAXMESSAGELENGTH + 1];

/*
  The CW messages are stored in the Messages.h header file. Currently, they are limited to 27 characters, with the
  first message being all blanks and used for erasing messages. You can make them longer if you wish, but will need
  to change MAXMESSAGELENGTH in the CWM.h header file for the new length. Obviously you should edit there for your
  own info.

      0                                                       27 bytes
      +-------------------------------------------------------+
  0    | All blank spaces used for erasing                     |
      +-------------------------------------------------------+
  1    | CQ CQ CQ DE W8TEE                                     |
      +-------------------------------------------------------+
  2    | 599 OH                                                |
      +-------------------------------------------------------+
  ...  | ...                                                   |
      +-------------------------------------------------------+
  13   | Woodland Mounds St Pk                                 |
      +-------------------------------------------------------+

*/

/*****
  Purpose: Returns the number of messages stored in the array, including the "blank" message

  Paramter list:
    void

  Return value:
    int           the number of elements in the contestExchanges[] array

  CAUTION:
*****/
int FetchMessageCount()
{
  int i, count = 0;

  for (i = 1; i < MAXMESSAGES; i++) {
    if ( !isprint((int) contestExchanges[i][0]))
      break;
    else
      count++;
  }
  return count + 1;       // Add 1 for first message
}




/*****
  Purpose: Display the message number associated with a switch immediately above that switch

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    void

  CAUTION:
*****/
void ShowSwitchMessageNumbers(TFT_eSPI tft)
{
  tft.fillRect(20, 180, DISPLAYWIDTH - 100, 22, TFT_BLACK);      // Erase screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(40, 200);
  tft.print(switchIndexes[0]);
  tft.setCursor(100, 200);
  tft.print(switchIndexes[1]);
  tft.setCursor(160, 200);
  tft.print(switchIndexes[2]);
}

/*****
  Purpose: Displays current message list and allows user to assign switches to messages

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    void

  CAUTION:
*****/
void ShowSwitchSelectionButtons(TFT_eSPI tft)
{
  int flag;
  tft.setFreeFont(FSB9);

  arrowUp.drawButton();
  arrowDown.drawButton();

  switch1.drawButton();
  switch2.drawButton();
  switch3.drawButton();

  done.drawButton();

  ShowSwitchMessageNumbers(tft);
}


/*****
  Purpose: Determines the index number for the last button pressed.

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    int                     An index that corresponds to the button pressed using symbolic constnats:
                                  #define UPARROWBUTTON           0
                                  #define DOWNARROWBUTTON         1
                                  #define SWITCH1BUTTON           2
                                  #define SWITCH2BUTTON           3
                                  #define SWITCH3BUTTON           4

  CAUTION:  Assume the display screen has been drawn.
*****/
int WhichButtonPressed(uint16_t touchX, uint16_t touchY)
{
  int whichOne;

  if (arrowUp.contains(touchX, touchY)) {                                 // Process arrowUp
    arrowUp.press(true);                  // tell the button it is pressed
    whichOne = UPARROWBUTTON;
  } else {
    arrowUp.press(false);                 // tell the button it is NOT pressed
  }
  if (arrowUp.justReleased())
    arrowUp.drawButton();
  if (arrowUp.justPressed()) {
    arrowUp.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (arrowDown.contains(touchX, touchY)) {                                 // Process arrowDown
    arrowDown.press(true);                  // tell the button it is pressed
    whichOne = DOWNARROWBUTTON;
  } else {
    arrowDown.press(false);                 // tell the button it is NOT pressed
  }
  if (arrowDown.justReleased())
    arrowDown.drawButton();
  if (arrowDown.justPressed()) {
    arrowDown.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (switch1.contains(touchX, touchY)) {                                 // Process switch1
    switch1.press(true);                  // tell the button it is pressed
    whichOne = SWITCH1BUTTON;
  } else {
    switch1.press(false);                 // tell the button it is NOT pressed
  }
  if (switch1.justReleased())
    switch1.drawButton();
  if (switch1.justPressed()) {
    switch1.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (switch2.contains(touchX, touchY)) {                                 // Process switch2
    switch2.press(true);                  // tell the button it is pressed
    whichOne = SWITCH2BUTTON;
  } else {
    switch2.press(false);                 // tell the button it is NOT pressed
  }
  if (switch2.justReleased())
    switch2.drawButton();
  if (switch2.justPressed()) {
    switch2.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (switch3.contains(touchX, touchY)) {                                 // Process switch3
    switch3.press(true);                  // tell the button it is pressed
    whichOne = SWITCH3BUTTON;
  } else {
    switch3.press(false);                 // tell the button it is NOT pressed
  }
  if (switch3.justReleased())
    switch3.drawButton();
  if (switch3.justPressed()) {
    switch3.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (done.contains(touchX, touchY)) {                                 // Process switch3
    done.press(true);                  // tell the button it is pressed
    whichOne = DONEBUTTON;
  } else {
    done.press(false);                 // tell the button it is NOT pressed
  }
  if (done.justReleased())
    done.drawButton();
  if (done.justPressed()) {
    done.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  if (selectEditMessage.contains(touchX, touchY)) {                                 // Process switch3
    selectEditMessage.press(true);                  // tell the button it is pressed
    whichOne = SELECTEDITMESSAGE;
  } else {
    selectEditMessage.press(false);                 // tell the button it is NOT pressed
  }
  if (selectEditMessage.justReleased())
    selectEditMessage.drawButton();
  if (selectEditMessage.justPressed()) {
    selectEditMessage.drawButton(true);            // draw invert
    displayKbdOut(tft);
  }

  return whichOne;
}


/*****
  Purpose: Displays current message list

  Paramter list:
    TFT_eSPI tft              display object
    int messageCoun           non-emptry messages

  Return value:
    void

  CAUTION:
*****/
void ShowMessages(TFT_eSPI tft, int messageCount)
{
  char temp[5];
  int i, count = 0, row = 30;

  tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen

  for (i = 1; i < messageCount; i++) {
    tft.setCursor(5, row);
    if (i == switchIndexes[0]) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    if (i < 10) {
      tft.print("  ");
    }
    tft.print(i);
    tft.print("  ");
    tft.print(contestExchanges[i]);
    row += 15;
  }

}


/*****
  Purpose: Determines which of the message buttons were pressed

  Paramter list:
    uint16_t touchX         X-Y for button object
    uint16_t touchY

  Return value:
    int                     An index that corresponds to the button pressed using symbolic constnats:
                                  #define UPARROWBUTTON           0
                                  #define DOWNARROWBUTTON         1
                                  #define SWITCH1BUTTON           2
                                  #define SWITCH2BUTTON           3
                                  #define SWITCH3BUTTON           4

  CAUTION:  Assume the display screen has been drawn.
*****/
int WhichMessageButtonPressed(uint16_t touchX, uint16_t touchY)
{

  if (arrowUp.contains(touchX, touchY)) {                                 // Process arrowUp
    arrowUp.press(true);                  // tell the button it is pressed
    arrowDown.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch2.press(false);
    return UPARROWBUTTON;
  }

  if (arrowDown.contains(touchX, touchY)) {                                 // Process arrowDown
    arrowDown.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch2.press(false);
    return DOWNARROWBUTTON;
  }

  if (switch1.contains(touchX, touchY)) {                                 // Process switch1
    switch1.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    arrowDown.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch2.press(false);
    return SWITCH1BUTTON;
  }
  if (switch2.contains(touchX, touchY)) {                                 // Process switch2
    switch2.press(true);                  // tell the button it is pressed
    switch1.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    arrowDown.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch2.press(false);
    return SWITCH2BUTTON;
  }

  if (switch3.contains(touchX, touchY)) {                                 // Process switch3
    switch3.press(true);                  // tell the button it is pressed
    switch1.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    arrowDown.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch2.press(false);
    return SWITCH3BUTTON;
  }

  if (done.contains(touchX, touchY)) {                                 // Process switch3
    done.press(true);                  // tell the button it is pressed
    switch1.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    arrowDown.press(false);
    switch1.press(false);
    switch2.press(false);
    switch3.press(false);
    done.press(false);
    return DONEBUTTON;
  }

  if (selectEditMessage.contains(touchX, touchY)) {                                 // Process switch3
    selectEditMessage.press(true);                  // tell the button it is pressed
    arrowUp.press(false);
    arrowDown.press(false);
    done.press(false);                  // tell the button it is pressed
    switch1.press(false);
    switch2.press(false);
    switch3.press(false);
    selectEditMessage.press(false);
    return SELECTEDITMESSAGE;
  }
}


/*****
  Purpose: Allows the user to hightlight and select a message for a switch

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    void

  CAUTION:  Assume the display screen has been drawn.
*****/
void MakeSwitchSelections(TFT_eSPI tft)
{
  int flag, index, oldIndex, whichButton, row;
  static int highlightedRow, oldRow;

  row = switchIndexes[0] * 15 + 15;
  oldIndex = index = switchIndexes[0];
  highlightedRow = row;

  while (true) {
    // / Check if any key coordinate boxes contain the touch coordinates
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      MyDelay(300L);
      touchX = DISPLAYWIDTH - touchX;
      whichButton = WhichMessageButtonPressed(touchX, touchY);

      switch (whichButton) {

        case UPARROWBUTTON:
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

        case DOWNARROWBUTTON:
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

        case SWITCH1BUTTON:
          switchIndexes[0] = (byte) index;
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers(tft);
          break;

        case SWITCH2BUTTON:
          switchIndexes[1] = (byte) index;
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers(tft);
          break;

        case SWITCH3BUTTON:
          switchIndexes[2] = (byte) index;
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers(tft);
          break;

        case DONEBUTTON :
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers(tft);
          return;                               // All done, get outta here...
          break;

        default:
          Serial.print("I shouldn't be here. whichButton = ");
          Serial.println(whichButton);
          break;
      }
    }
  }

}

/*****
  Purpose: Allows the user to hightlight and select a message for a switch

  Paramter list:
    char newMessage[]     The new message to save
    int where             The index where to write it in contestExchanges[]

  Return value:
    void

  CAUTION:  Assume the display screen has been drawn.
*****/
void SaveEditMessage(char newMessage[], int where)
{
  strcpy(contestExchanges[where], newMessage);
}

/*****
  Purpose: checks to see if a message switch has been pressed

  Paramter list:
    void

  Return value:
    int           the switch that was pressed or -1 if no switch
*****/
void ReadMessageSwitches()
{
  whichOne = -1;
  if (digitalRead(MSGSWITCH1) == LOW) {
    whichOne = 0;                               // Enen though switch 1, the array is zero-based
  } else if (digitalRead(MSGSWITCH2) == LOW) {
    whichOne = 1;
  } else if (digitalRead(MSGSWITCH3) == LOW) {
    whichOne = 2;
  }
}

/*****
  Purpose: The next 3 functions process the message interrupts.

  Paramter list:
    void

  Return value:
    int           the switch that was pressed or -1 if no switch
*****/
void ReadMessageSwitch1()
{
      SendMessage(tft, 0);

}

void ReadMessageSwitch2()
{
    SendMessage(tft, 1);
}

void ReadMessageSwitch3()
{
    SendMessage(tft, 2);
}

/*****
  Purpose: Use the selected switch to send the CW message

  Paramter list:
    TFT_eSPI tft         display
    int messageSwitch    the switch index (1-3)

  Return value:
    void
*****/
void SendMessage(TFT_eSPI tft, int messageSwitch)
{
  SendLetters(tft, &contestExchanges[switchIndexes[messageSwitch]][0]);
}

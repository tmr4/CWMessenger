#include "CWM.h"

// This is in CWM.h
//extern char contestExchanges[][MAXMESSAGELENGTH + 1];

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
    int  The number of user messages that have been
         stored.  Does not include the first (blank)
         line

*****/
int fetchUserMessageCount(void)
{
  int i, count = 0;

  for (i = 1; i <= MAXMESSAGES; i++) {
    if (!isprint((int)contestExchanges[i][0]))
      break;
    else
      count++;
  }
  return count;
}




/*****
  Purpose: Display the message number associated with a switch immediately above that switch

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    void

  CAUTION:
*****/
void ShowSwitchMessageNumbers(void)
{
#ifdef DEBUG
  Serial.print("ShowSwitchMessageNumbers:");Serial.print(switchIndexes[0]);
  Serial.print(", ");Serial.print(switchIndexes[1]);
  Serial.print(", ");Serial.println(switchIndexes[2]);
#endif
  tft.fillRect(20, 180, DISPLAYWIDTH - 100, 22, TFT_BLACK);      // Erase part of screen

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
void ShowSwitchSelectionButtons(void)
{
  int flag;
  tft.setFreeFont(FSB9);

  arrowUp.drawButton();
  arrowDown.drawButton();

  switch1.drawButton();
  switch2.drawButton();
  switch3.drawButton();

  done.drawButton();

  ShowSwitchMessageNumbers();
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
void ShowMessages(int userMessageCount)
{
  int i;
  int row = 30;

  #ifdef DEBUG
    Serial.print("ShowMessages:  messageCount = ");Serial.println(messageCount);
  #endif
  tft.fillScreen(TFT_BLACK);
  // Nlte: messageCount includes the blank line (!)
  // Actual messages are 1, 2,... userMesssageCount
  for (i = 1; i <= userMessageCount; i++) {
    tft.setCursor(5, row);
    if (i == switchIndexes[0]) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    if (i < 10) {
      // Two space chars equal one digit width
      tft.print("  ");
    }
    tft.print(i); // Message number
    tft.print("  "); // Space before text of message
    #ifdef DEBUG
      Serial.print("i = ");Serial.print(i);
      Serial.print(": ");Serial.println(contestExchanges[i]);
    #endif
    tft.print(contestExchanges[i]);
    //delay(100);
    row += 15;
  }

}
/* I moved the selectEditMessage button so that it overlaps
 *  other buttonns, so I require a separate function
 *  that only looks at this button and the arrow buttons
 */
 /*
  Purpose: Determines which of the message buttons were pressed

  Paramter list:
    uint16_t touchX         X-Y for button object
    uint16_t touchY

  Return value:
    int                     An index that corresponds to the button pressed using #defined
                            constants, or -1 if none of these buttons was pressed.
*/
int WhichEditScreenButtonPressed(uint16_t touchX, uint16_t touchY)
{

  if (arrowUp.contains(touchX, touchY)) {
    arrowUp.press(true);                  // tell the button it is pressed
    arrowDown.press(false);
    selectEditMessage.press(false);
    return UPARROWBUTTON;
  }

  if (arrowDown.contains(touchX, touchY)) {
    arrowDown.press(true);
    arrowUp.press(false);
    selectEditMessage.press(false);
    return DOWNARROWBUTTON;
  }
   if (selectEditMessage.contains(touchX, touchY)) {
    selectEditMessage.press(true);
    arrowDown.press(false);
    arrowUp.press(false);
    return SELECTEDITMESSAGE;
  }
  // Didn't find anything
  return -1;
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

#if 0
  // Now is in a separate function
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
  #endif
  // Didn't find anything
  return -1;
}


/*****
  Purpose: Allows the user to hightlight and select a message for a switch

  Paramter list:
    TFT_eSPI tft             display object

  Return value:
    void

  CAUTION:  Assume the display screen has been drawn.
*****/
void MakeSwitchSelections(void)
{
  uint8_t pressed;
  uint16_t touchX, touchY;
  int flag, index, oldIndex, whichButton, row;
  static int highlightedRow, oldRow;

  row = switchIndexes[0] * 15 + 15;
  oldIndex = index = switchIndexes[0];
  highlightedRow = row;

  while (true) {
    // / Check if any key coordinate boxes contain the touch coordinates
    pressed = tft.getTouch(&touchX, &touchY);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed) {
      delay(300);
      whichButton = WhichMessageButtonPressed(touchX, touchY);

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

          if (index > 1) {                          // If at the first one and an Up arrow, wrap around
            index--;
          }
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
          tft.setCursor(5, highlightedRow);         // Redraw old message to unselected color
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          if (index < 10) {
            tft.print("  ");
          }
          tft.print(index);
          tft.print("  ");
          tft.print(contestExchanges[index]);

          if (index < MAXMESSAGES) {           // At the last index, wrap to first index
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
          ShowSwitchMessageNumbers();
          break;

        case SWITCH2BUTTON:
          switchIndexes[1] = (byte) index;
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers();
          break;

        case SWITCH3BUTTON:
          switchIndexes[2] = (byte) index;
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers();
          break;

        case DONEBUTTON :
          UpdateSwitchesArray();
          ShowSwitchMessageNumbers();
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
           

  Parameter list:
    void

  Return value:
    int           the switch that was pressed or -1 if no switch
*****/
int ReadMessageSwitches(void)
{
  int swno = -1;

  if (digitalRead(MSGSWITCH1) == LOW) {
    swno = 0;
  } else if (digitalRead(MSGSWITCH2) == LOW) {
    swno = 1;
  } else if (digitalRead(MSGSWITCH3) == LOW) {
    swno = 2;
  }
  return swno;
}

/*****
  Purpose: The next 3 functions process the message interrupts.

  Paramter list:
    void

  Return value:
    int           the switch that was pressed or -1 if no switch
*****/
void ReadMessageSwitch1(void)
{
      SendMessage(0);

}

void ReadMessageSwitch2(void)
{
    SendMessage(1);
}

void ReadMessageSwitch3(void)
{
    SendMessage(2);
}

/*****
  Purpose: Use the selected switch to send the CW message

  Paramter list:
    TFT_eSPI tft         display
    int messageSwitch    the switch index (1-3)

  Return value:
    void
*****/
void SendMessage(int messageSwitch)
{
#ifdef DEBUG
  Serial.print("SendMessage: messageSwitch = ");Serial.print(messageSwitch);
  Serial.print(", switchIndexes[messageSwitch] = ");Serial.println(switchIndexes[messageSwitch]);
#endif
  SendLetters(&contestExchanges[switchIndexes[messageSwitch]][0]);
}

#include "CWM.h"

/*
  The EEPROM space sets aside MAXMESSAGELENGTH * MAXMESSAGES (i.e., 50 * 40) bytes of memory to store the
  messages that can be sent via this device. The first message at EEPROM address 0 is not a message, but rather a number
  that tells how many messages are available. There the organization of this block of memory is:

      0                                                       49 bytes
      +-------------------------------------------------------+
  0    | 20                                                    |   // there are 20 messages that have been stored
      +-------------------------------------------------------+
  1    | CQ CQ CQ DE W8TEE CQ CQ CQ DE W8TEE KN                |
      +-------------------------------------------------------+
  2    | NAME IS JACK JACK                                     |
      +-------------------------------------------------------+
  ...  | ...                                                   |
      +-------------------------------------------------------+
  20   | UR NO                                                 |
      +-------------------------------------------------------+

  Each new message is added at the end of the message list. Therefore, the next message is always added at
      (messagesInList + 1) *  MAXMESSAGELENGTH
  so we skip over the first "message". Example: If the list is empty:
     (messagesInList + 1) *  MAXMESSAGELENGTH
             (0 + 1) * 50
             50
  so the first message would be written at byte offset 50, as shown above.

*/

/*****
  Purpose: This function display the entire EEPROM memory block. 

  Paramter list:
    void

  Return value:
    void

  CAUTION: PC output only
*****/
void EEPROMDump()
{
  byte c;
  int i;

  Serial.println("Into EEPROMDump()");
  for (i = 0; i < MAXMESSAGELENGTH * MAXMESSAGES; i++) {
    c = EEPROM.readByte(i);
    Serial.print(c, HEX);
    if (i % MAXMESSAGELENGTH == 0)
      Serial.println(" ");
  }
}
/*****
  Purpose: This function takes the new message and writes it to EEPROM. This function updates the messageCounter
           stored at address 0

  Paramter list:
    char *inputBuffer     pointer to the new message, which is null-terminated
    int messageCount      its position in the message data block

  Return value:
    void

  CAUTION:
*****/
void SaveNewMessage(char *inputBuffer, int messageCount)
{
  EEPROM.writeBytes(messageCount * MAXMESSAGELENGTH, (void *) inputBuffer, strlen(inputBuffer) );
  EEPROM.commit();
  EEPROM.writeByte(0, (byte) messageCount);                                 // Update the counter
  EEPROM.commit();
}




/*****
  Purpose: Writes the current values of the switches array which holds the currently-active messages.

  Paramter list:
    void

  Return value:
    void

  CAUTION:
*****/
void UpdateSwitchesArray()
{
  int i;

  for (i = 0; i < 3; i++) {
    EEPROM.writeByte(i, switchIndexes[i]);
  }
  EEPROM.commit();
}

/*****
  Purpose: Read the current switch assignments

  Paramter list:
    void

  Return value:
    void

  CAUTION:
*****/
void ReadSwitchesArray()
{
  switchIndexes[0] = EEPROM.readByte(0);
  switchIndexes[1] = EEPROM.readByte(1);
  switchIndexes[2] = EEPROM.readByte(2);
}

/*****
  Purpose: Read words per minute from EEPROM

  Paramter list:
    void

  Return value:
    void

  CAUTION:
*****/

int ReadWordsPerMinute()
{
  wordsPerMinute = (int) EEPROM.readByte(3);
  if (wordsPerMinute > MINWPM && wordsPerMinute < MAXWPM) {
    return wordsPerMinute;
  } else {
    wordsPerMinute = MESSAGEWPM;                   // If no value written yet...
    EEPROM.writeByte(3, (byte) wordsPerMinute);
  }
  return wordsPerMinute;
}

/*****
  Purpose: Write the words per minute to EEPROM

  Paramter list:
    void

  Return value:
    void

  CAUTION:
*****/

void WriteWordsPerMinute()
{
  EEPROM.writeByte(3, wordsPerMinute);
  EEPROM.commit();
}

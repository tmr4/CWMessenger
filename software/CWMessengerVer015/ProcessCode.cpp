#include "CWM.h"

#define TOPDEADSPACE  10
#define LETTERSPERROW 20
#define ROWCOUNT      15

/*****
  Purpose: to send a Morse code dit

  Paramter list:
  void

  Return value:
  void

  CAUTION: Assumes that a global named ditlen holds the value for dit spacing
*****/
void  Dit()
{
  digitalWrite(OPTOINPUT, HIGH);    // Start dit
  delay(ditlen);
  digitalWrite(OPTOINPUT, LOW);     // End dit
  delay(ditlen);

}

/*****
  Purpose: to send a Morse code dah

  Paramter list:
  void

  Return value:
  void

  CAUTION: Assumes that a global named ditlen holds the value for dit spacing
*****/
void IRAM_ATTR Dah()
{
  digitalWrite(OPTOINPUT, HIGH);    // Start dah

  delay(ditlen * 3UL);
  digitalWrite(OPTOINPUT, LOW);    // End dah
  delay(ditlen);
}

/*****
  Purpose: to provide spacing between letters

  Paramter list:
  void

  Return value:
  void

  CAUTION: Assumes that a global named ditlen holds the value for dit spacing
*****/
void LetterSpace()
{
  MyDelay(3UL * ditlen);
}
/*****
  Purpose: to provide spacing between words

  Paramter list:
  void

  Return value:
  void

  CAUTION: Assumes that a global named ditlen holds the value for dit spacing
*****/
void WordSpace()
{
  MyDelay(7UL * ditlen);
}

/*****
  Purpose: to send a Morse code character

  Paramter list:
  char code       the code for the letter to send

  Return value:
  void
*****/
void SendCode(char code)
{
  int i;


  for (i = 7; i >= 0; i--)
    if (code & (1 << i))
      break;

  for (i--; i >= 0; i--) {
    if (code & (1 << i))
      Dah();
    else
      Dit();
  }
  LetterSpace();
}


/*****
  Purpose: to send a Morse code character


  Paramter list:
  char myChar       The character to be sent

  Return value:
  void
*****/
void Send(char myChar)
{
  int index;

  if (isalpha(myChar)) {
    if (islower(myChar)) {
      myChar = toupper(myChar);
    }
    SendCode(letterTable[myChar - 'A']);   // Make into a zero-based array index
    return;
  } else if (isdigit(myChar)) {
    SendCode(numberTable[myChar - '0']);   // Same deal here...
    return;
  }
  index = (int) myChar;
  switch (myChar) {                 // Non-alpha and non-digit characters
    case '\r':
    case '\n':
    case '!':
      SendCode(0b01101011);         // exclamation mark 33
      break;
    case '"':
      SendCode(0b01010010);         // double quote 34
      break;
    case '$':
      SendCode(0b10001001);         // dollar sign 36
      break;
    case '@':
      SendCode(0b00101000);         // ampersand 38
      break;
    case '\'':
      SendCode(0b01011110);         // apostrophe 39
      break;

    case '(':
    case ')':
      SendCode(0b01011110);         // parentheses (L) 40, 41
      break;

    case ',':
      SendCode(0b01110011);         // comma 44
      break;

    case '.':
      SendCode(0b01010101);         // period  46
      break;
    case '-':
      SendCode(0b00100001);         // hyphen 45
      break;
    case ':':
      SendCode(0b01111000);         // colon 58
      break;
    case ';':
      SendCode(0b01101010);         // semi-colon 59
      break;
    case '?':
      SendCode(0b01001100);         // question mark 63
      break;
    case '_':
      SendCode(0b01001101);         // underline 95
      break;

    case '⌠':
      SendCode(0b01101000);         // paragraph
      break;

    case ' ':                       // Space
      WordSpace();
      break;

    default:
      WordSpace();
      break;
  }
}



/*****
  Purpose: establish the dit length for code transmission. Crucial since
    all spacing is done using dit length

  Parameter list:
    int wpm

  Return value:
    void
*****/
void SetDitLength(int wpm)
{
  ditlen = 1200 / wpm;
}

/*****
  Purpose: display a letters on the LCD display and send them in code

  Parameter list:
    TFT_eSPI tft        the display device
    char *msg         the message to be send

  Return value:
    void

  CAUTION: assumes call[] is large enough for up to 7 chars
*****/
void SendLetters(TFT_eSPI tft, char *msg)
{
  int letterCount = 0;
  int rowCount = 0;

  tft.fillRect(0, 0, DISPLAYWIDTH, DISPLAYWIDTH, TFT_BLACK);      // Erase screen
  tft.setCursor(0, TOPDEADSPACE + 10);
  while (/*digitalRead(ENCODERSWITCH1) != LOW &&*/ *msg != '\0') {
    Send(*msg);
    tft.print(*msg++);
    letterCount++;

    if (letterCount > LETTERSPERROW) {
      rowCount++;
      letterCount = 0;
      if (rowCount > ROWCOUNT) {
        tft.setCursor(0, rowCount * 10);
        rowCount = 0;
      }
    }
  }
}

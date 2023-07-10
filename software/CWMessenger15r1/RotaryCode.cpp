#include "CWM.h"


// Moved UpdateWPM() to RotaryCode.cpp so that we don't need a global variable encoderDirection
/*****
  Purpose: to update the words per minute data on the display when the encoder is rotated

  Parameter list:
    void

  Return value:
    void
*****/
static volatile int encoderDirection;

void UpdateWPM(void)
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

// This the ISR for Encoder signal interrupts.
// I like to give "special" functions like this
// distinctive names
void encoderISR(void)
{
  unsigned char result = rotary.process();

  encoderDirection = 0;
  switch (result) {
    case 0:           // No interrupt
      return;
      break;

    case DIR_CW:       // clockwise
      encoderDirection = 1;
      break;

    case DIR_CCW:       // clockwise
      encoderDirection = -1;
      break;

    default:
      encoderDirection = 0;
      break;
  }
  UpdateWPM();
}

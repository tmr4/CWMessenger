# Software Versions

I've uploaded two versions of the CW Messenger software:

* CWMessengerVer015 is the latest official version of the author's software that I could find on the project's [website](https://groups.io/g/SoftwareControlledHamRadio/files/ZippedSourceCode.7z). 
* CWMessenger15r1 is the latest unofficial version of the software.  This version was posted by Dave Evans in [message #11747](https://groups.io/g/SoftwareControlledHamRadio/message/11747).

Based on [messages](https://groups.io/g/SoftwareControlledHamRadio/search?p=recentpostdate%2Fsticky%2C%2C%2C20%2C2%2C0%2C0&q=CW+Messenger) regarding the CW Messenger project, CWMessengerVer015 appears to be the latest official version.  I could not get this version running with version 1.8.13 or 1.8.19 of the Arduino IDE and the latest versions of the libraries I had to download for the project (with the version 1.8.13 I did not update libraries already present on my computer for the test).  Given that other builders had success with version CWMessenger15r1, I moved on to that without further testing this version.

I could not get CWMessenger15r1 running on version 1.8.19 of the IDE.  Dave described his setup in this [message](https://groups.io/g/SoftwareControlledHamRadio/message/11729):
  * Arduino IDE version 1.8.13
  * ESP32 version 1.06
  * Rotary library from https://github.com/buxtronix/arduino/blob/master/libraries/Rotary
  * TFT_eSPI version 1.06

I duplicated this setup on an old laptop.  I couldn't locate TFT_eSPI version 1.06 (I think this is a typo) so I installed the latest version, 2.5.0 (I thought I had installed the version active at the time Dave was doing his testing, version 2.3.61, but I guess I did something wrong, or the Arduino IDE automatically updated it to the latest version at some point).  The only change I had to make was uncommenting the ESP32 development board SPI and touch pin definitions in the TFT_eSPI User_Setup.h file (lines 212-217 and line 230).  Note that the development board defines can be made in the CWM.h header file, but I could not get the program to compile without TOUCH_CS being defined in the User_Setup.h file.

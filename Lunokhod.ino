// UTFT_Demo_400x240 (C)2014 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 400x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <UTouch.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Arduino Uno / 2009:
// -------------------
// Standard Arduino Uno/2009 shield            : <display model>,A5,A4,A3,A2
// DisplayModule Arduino Uno TFT shield        : <display model>,A5,A4,A3,A2
//
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ITDB32WC,38,39,40,41);
UTouch myTouch(6,5,4,3,2);
void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  myTouch.InitTouch();
   myGLCD.clrScr();
  myGLCD.fillScr(255,0,0);
}

void loop()
{
//  myGLCD.clrScr();
// Clear the screen and draw the frame



  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 399, 13);
  myGLCD.setColor(64, 64, 64);
//  myGLCD.fillRect(0, 226, 399, 239);
  myGLCD.setColor(255, 255, 255);
//  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("*** Universal Color TFT Display Library ***", LEFT,1 );
  myGLCD.print("test", LEFT, 12);
if (myTouch.dataAvailable()) 
{
   myGLCD.print("Mytouch dataVailable", LEFT, 24);
  myGLCD.fillScr(0,255,0);
}
  else 
{
     myGLCD.print("Mytouch no data", LEFT, 24);
    myGLCD.fillScr(255,0,0);
} 
  
//  delay (10000);
}


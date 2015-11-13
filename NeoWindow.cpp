// NeoWindow
#include "NeoStrip.h"
#include "NeoWindow.h"

//////
// used to invoke the member function pointer...
//#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

// Class Variable definitions:

unsigned long NeoWindow::currTime = 0; // current Millis
int NeoWindow::s_nIDGenerator = 1;

/////////////////
// Constructor
NeoWindow::NeoWindow(NeoStrip *strip, int startPixel, int len)
{
  myId = s_nIDGenerator++;
  myStrip = strip;
  myStartPixel = startPixel;
  myPixelCount = len;
  myEndPixel = myStartPixel + myPixelCount-1;
  efxDone = false;

  lastTime = 0;
//  diffTime = 0;

//  currentEffect = -1; // id of current effect
  effectDelay = 0; // delay between updates of current effect
  curUpdateFunc = NULL;
}

void NeoWindow::printId(void)
{
  Serial.print("NeoWindow ");Serial.println(myId);
}

void NeoWindow::printData(void)
{
  printId();
  Serial.print("   myStartPixel: ");Serial.println(myStartPixel);
  Serial.print("   myPixelCount: ");Serial.println(myPixelCount);
  Serial.print("   myEndPixel: ");Serial.println(myEndPixel);
  Serial.print("   myStartPixel: ");Serial.println(myStartPixel);
}

void NeoWindow::updateWindow(void)
{
  if (!curUpdateFunc) {
    printId(); Serial.println("    No updateFunc");
    return; // no effect defined. quick return
  }
  
   // determine if the current Effect time has passed

   if (NeoWindow::currTime - lastTime < effectDelay) {
//    printId(); Serial.println("    Not yet time");
    return;
   }
   lastTime = NeoWindow::currTime;
   
   // invoke current effect function update
   (this->*curUpdateFunc)();

   myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::fillColor(uint32_t color)
{

  for (int i=myStartPixel; i <= myEndPixel; i++)
       myStrip->setPixelColor(i, color); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed

}

void NeoWindow::fillBlack()
{
    fillColor(0);
}


void NeoWindow::setCircleEfx(uint32_t color, uint32_t delayTime)
{
  printId(); Serial.println("    set to use circle effect");
  efxDone = false;
  effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::circleUpdateEfx;
  
  // starting a Circle Effect using color and time
  circle_color = color;
  circle_cursor = myStartPixel;

  // setup the initial frame
  myStrip->setPixelColor(circle_cursor, circle_color);
  for (int i=circle_cursor+1; i< myEndPixel; i++)
    myStrip->setPixelColor(i, 0); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::circleUpdateEfx(void)
{
  // we assume the update function has determined if it is time to call me
  
//  printData();
//  Serial.println("Updating Circle Effect");
//  Serial.print("   circle_cursor: ");Serial.println(circle_cursor);
   
  // circle moves a single pixel of circle_color around the virtual circle of the window
  // clear the currentPixel
  myStrip->setPixelColor(circle_cursor, 0);
  circle_cursor++;
  if (circle_cursor > myEndPixel) { 
     circle_cursor = myStartPixel;
     efxDone = true; // if we dont check, it just continues
//     Serial.println(" wrapped circle");
  }
  myStrip->setPixelColor(circle_cursor, circle_color);
}

void NeoWindow::setWipeEfx(uint32_t color, uint32_t delayTime) // Wipe color once around window
{
//  printId(); Serial.println("    set to use wipe effect");
  efxDone = false;
  effectDelay = delayTime;  
  curUpdateFunc = &NeoWindow::wipeUpdateEfx;
  
  // starting a Circle Effect using color and time
  wipe_color = color;
  wipe_cursor = myStartPixel;

  myStrip->setPixelColor(wipe_cursor, wipe_color);
  // dont change rest of colors
//  for (int i=circle_cursor+1; i< myEndPixel; i++)
//    myStrip->setPixelColor(i, 0); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::wipeUpdateEfx(void)
{
   // wipe fills the window one pixel each update, then sets Done
  
  myStrip->setPixelColor(wipe_cursor, wipe_color);
  wipe_cursor++;
  
  if (wipe_cursor > myEndPixel) { 
     efxDone = true;
  }
}

void NeoWindow::setBlinkEfx(uint32_t color, uint32_t delayTime, int count)
{
  // set the instance stuff
  efxDone = false;
  effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::blinkUpdateEfx;

  // effect specific stuff
  blink_color = color;
  blink_state = false;
  blink_maxCount = count;
  blink_step = 0;
  
  blinkUpdateEfx();

}

void NeoWindow::blinkUpdateEfx()
{
  if (blink_state)
  {
      fillColor(blink_color);
      blink_state = false;
  } else {
      fillBlack();
      blink_state = true;
  }
  
  blink_step++;
  if (blink_maxCount > 0 && blink_step > blink_maxCount)
    efxDone = true;
}


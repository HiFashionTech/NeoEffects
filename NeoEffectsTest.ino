// test the NeoStrip and NeoWindow classes
//

#include <Adafruit_NeoPixel.h>
#include "NeoStrip.h"
#include "NeoWindow.h"

#define SMALL_NEORING_SIZE 12

// Pins for strips connected via OctoWS2811
// these are assigned to odd ordering of pins for reasons internal to Octo library
// although we are not using the library, we may use the Octo interface board 
// which provides access to these pins via 2 RJ45 connectors.
#define STRIP_1_PIN 2
//#define STRIP_2_PIN 14
//#define STRIP_3_PIN 7
//#define STRIP_4_PIN 8
//#define STRIP_5_PIN 6
//#define STRIP_6_PIN 20
//#define STRIP_7_PIN 21
//#define STRIP_8_PIN 5

//NeoStrip strip2 = NeoStrip(60, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

const int RING_1_START = 0;
const int RING_2_START = (RING_1_START + SMALL_NEORING_SIZE);
const int RING_3_START = (RING_2_START + SMALL_NEORING_SIZE);
const int RING_4_START = (RING_3_START + SMALL_NEORING_SIZE);
const int RING_5_START = (RING_4_START + SMALL_NEORING_SIZE);
const int RING_6_START = (RING_5_START + SMALL_NEORING_SIZE);
const int RING_7_START = (RING_6_START + SMALL_NEORING_SIZE);
const int RING_8_START = (RING_7_START + SMALL_NEORING_SIZE);
const int numRings = 8;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
NeoStrip strip1 = NeoStrip(SMALL_NEORING_SIZE * numRings, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

NeoWindow ring1 = NeoWindow(&strip1, RING_1_START, SMALL_NEORING_SIZE);
NeoWindow ring2 = NeoWindow(&strip1, RING_2_START, SMALL_NEORING_SIZE);
NeoWindow ring3 = NeoWindow(&strip1, RING_3_START, SMALL_NEORING_SIZE);
NeoWindow ring4 = NeoWindow(&strip1, RING_4_START, SMALL_NEORING_SIZE);
NeoWindow ring5 = NeoWindow(&strip1, RING_5_START, SMALL_NEORING_SIZE);
NeoWindow ring6 = NeoWindow(&strip1, RING_6_START, SMALL_NEORING_SIZE);
NeoWindow ring7 = NeoWindow(&strip1, RING_7_START, SMALL_NEORING_SIZE);
NeoWindow ring8 = NeoWindow(&strip1, RING_8_START, SMALL_NEORING_SIZE);

NeoWindow *rings[] = {&ring1, &ring2, &ring3, &ring4, &ring5, &ring6, &ring7, &ring8};

uint32_t anoukPurple = strip1.Color(128, 0, 50);

void setup() {
  delay(5000); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.begin(115200);
  Serial.println("Starting NeoEffects Test");
  
  strip1.begin();

 // if dont setBrightness down, we get odd effects
 strip1.setBrightness(100);

 strip1.clearStrip();
 strip1.show();
delay(1000);

//  strip1.fillStrip(Adafruit_NeoPixel::Color(255,255,255));
  strip1.fillStrip(anoukPurple);  //Adafruit_NeoPixel::Color(128,128,128));
  strip1.show();
 delay(1000);
 strip1.clearStrip();
  strip1.show();
delay(1000);
  
  // put your setup code here, to run once:
  ring1.setWipeEfx(strip1.randomColor(),100 );
  ring2.setWipeEfx(anoukPurple,100 );
  ring3.setBlinkEfx(anoukPurple, 250, 5);
  
  ring4.setCircleEfx(strip1.randomColor(), 200);
  ring5.setCircleEfx(Adafruit_NeoPixel::Color(128, 0, 128), 10);
  ring6.setCircleEfx(Adafruit_NeoPixel::Color(128, 128, 128), 500);
  ring7.setCircleEfx(Adafruit_NeoPixel::Color(0, 128, 128), 500);
  ring8.setCircleEfx(Adafruit_NeoPixel::Color(128, 0,0), 50);

  // now for each Window
//  for (int i=0; i< numRings;i++)
//  {
//      rings[i]->printData();
//  }
//  delay(10000);
Serial.println("Setup Done");
}

int ring2State = false;

void loop() {
    
  // grab the current time in class method
  NeoWindow::updateTime();

  // check all inputs and update Effects per Window, etc
  if (ring1.effectDone())
      ring1.setWipeEfx(strip1.randomColor(),100 );
  if (ring2.effectDone()){
    if (ring2State)
    {
      ring2.setWipeEfx(0,100 );
      ring2State = false;
    } else {
      ring2.setWipeEfx(anoukPurple,100 );
      ring2State = true;
    }
  }
  if (ring3.effectDone())
     ring3.setBlinkEfx(anoukPurple, 250, 10);
  if (ring4.effectDone())
      ring4.setCircleEfx(strip1.randomColor(), 200);

  // now update each Window
  for (int i=0; i< numRings;i++)
  {
      rings[i]->updateWindow();
  }
  
//  ring1.updateWindow();
//  ring2.updateWindow();
//  ring3.updateWindow();
//  ring4.updateWindow();
//  ring5.updateWindow();
//  ring6.updateWindow();
//  ring7.updateWindow();
//  ring8.updateWindow();

  strip1.show();
//Serial.print("BottomLoop "); Serial.println(millis());
//  delay(1000); 
}

Provides an Arduino library called **NeoEffects** based on AdaFruit_NeoPixel library for LED Strips (rings, matrices, etc) that avoids the blocking delay() function.

## Motivation

Most of the example code for the AdaFruit_NeoPixel library uses the arduino delay().  We had a several projects that use upwards of 8 of the Adafruit NeoPixel 12-LED rings for a Fashion Tech workshop with [Anouk Wipprecht] (http://www.anoukwipprecht.nl). The blocking `delay()` made these difficult, requiring multiple Teensy controllers to get multiple simultaneous effects.

For Example the Wipe Effect fills a strip with a color one pixel at a time:

```
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
```

This won’t work well with daisy chained rings (effectively making a longer strip), that want different effects on different rings.

The classic way to avoid a delay is to use the millis() function to check if some time period has passed.  For a full discussion, see [http://playground.arduino.cc/Code/AvoidDelay](http://playground.arduino.cc/Code/AvoidDelay).

A second issue is the need to support effects that happen on one individual ring when multiple rings are daisy chained together.  An effect needs to know the absolute index of the first pixel and the number of pixels in the ring.

## Installation

Download this code into a directory called "NeoEffects", which must be a subdirectory within the [Arduino Library](https://www.arduino.cc/en/Hacking/Libraries) directory. If you are cloning this repository, just clone it into your Arduino Library directory.

## Approach

The NeoEffects library uses two classes: NeoStrip and NeoWindow

NeoStrip is derived (subclassed) from Adafruit_NeoPixel, providing a wrapper on that functionality and extending it.  The primary enhancement provided is the ability for the show() method to know if the strip has changed or not. (explained more below.) It also provides some useful helper functions such as clearStrip() and fillStrip(color);

The NeoWindow class defines a conceptual window as a subset of a strip, on which independent effects can be run. The effects in different windows on a strip can be updated independently, each using its own timer.

Note: The NeoWindow class currently defines the different effects as member functions. There may be a better design, but that is for later versions.

The basic layout is:

```
// define a NeoStrip of N pixels on a pin; 
//   strip consists of a daisy chain of numRings of ringSize
NeoStrip strip1 = NeoStrip(ringSize * numRings, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

// each of the rings is defined as a NeoWidow on the strip:
NeoWindow ring1 = NeoWindow(&strip1, RING_1_START, SMALL_NEORING_SIZE);
NeoWindow ring2 = NeoWindow(&strip1, RING_2_START, SMALL_NEORING_SIZE);
```

The setup() function needs to call begin on each NeoStrip, perhaps clearing or setting to a color:

```
strip1.begin();
strip1.clearStrip();
strip2.begin();
strip2.fillStrip(strip2.White);
```

And perhaps define an initial effect to occur in each window:

```
ring1.setBlinkEfx( color, blinkMillis, repeatCount);
```

Then in the loop() function:

  1. invoke the class method updateTime() to snag the current time to internal variable
  
          NeoWindow::updateTime();

  2. handle any effect changes due to user inputs or effects finishing defined run time.

  3. For each ring, invoke its updateWindow() to advance the effect (if its timer is ready) 
	
	        ring->UpdateWindow();
	        
  4. Show the strip. Note that this show() will check to see if the strip changed, and only send commands out if it has changed:
	
	       strip1.show();


## Implementation notes

The primary magic happens in the NeoWindow class. Rhe setXXXEffect methods set up a variety of instance variables, and initialize the effect.  Of particular importance are:

  * Clearing the efxDone flag
  * Setting the instance timer value
  * Setting the updateFunction to the current function

The update function is a bit of C magic using a Pointer To A Function. This allows a common UpdateWindow() method to be called by the application, and it knows internally if the current effect timer needs to run the update, and invoke that update function.  You could use some sort of effect ID# and a big switch statement, but function pointers are so much more fun.  A bit of care is needed to get the syntax right but the cleanliness of code is worth it.




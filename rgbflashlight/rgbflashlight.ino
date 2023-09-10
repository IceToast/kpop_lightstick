/*
RGB Flashlight v1 - Ben Miller @vmFoo 5-17-2014
Some minor #define snippets borrowed from neopixel examples

The switch on STATEPIN controls state for the inputs
State 1 (LOW) : Color modification:  The potentiometers change red, green, blue values only
State 2 (HIGH): Effect mode: The red pot now changes brightness and the green pot changes
                             the flash delay.  It goes on solid below a flash freq of 5ms
---------------------------- DEPRECATED ---------------------------------------------

This comment is deprecated.  It was written for the original RGB Flashlight v1.  The new
will feature a button to switch between modes.
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define POTI1 1 // silkscreen 2
#define POTI2 2 // silkscreen 4
#define POTI3 3 // silkscreen 3
#define STATEPIN 0
#define LEDPIN 1
#define PIXELS 16

/* POTI States on mode switch */
int poti1ModeChange = 128;
int poti2ModeChange = 128;
int poti3ModeChange = 128;

/* Initiate variables and set defaults */
int red = 128;
int green = 128;
int blue = 128;
int bright = 86;  // 1/3 brightness
int cycle = 1000; // 1 second
int blinkstate = 1;
int mode = 0;
int modeCycle = 500;
uint32_t modeCanChange;
int spinCurrentHead = PIXELS;
int spinCurrentTail = PIXELS / 3;
int spinCanChange = 1;

int i = 0;           // predeclare a counter integer
uint32_t stateStart; // for managing the blink

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup()
{
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

  pinMode(STATEPIN, INPUT);
  pixels.begin();
  pixels.setBrightness(bright);
  pixels.show();
  stateStart = millis();
  modeCanChange = millis();
}

void loop()
{
  if (digitalRead(STATEPIN) == HIGH)
  {
    setMode();
  }

  switch (mode)
  {
  case 0:
  { // Color modification mode
    if ((analogRead(POTI1) == poti1ModeChange) &&
        (analogRead(POTI2) == poti2ModeChange) &&
        (analogRead(POTI3) == poti3ModeChange))
    {
      break;
    }

    red = map(analogRead(POTI1), 0, 1024, 0, 254);
    green = map(analogRead(POTI3), 0, 1024, 0, 254);
    blue = map(analogRead(POTI2), 0, 1024, 0, 254);
    for (i = 0; i < PIXELS; i++)
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.setBrightness(bright);
    pixels.show();
    break;
  }
  case 1:
  { // moving ring mode
    // the leds are arranged in a ring, so we can use the i counter to
    // make the ring spin by lighting up the next led in the ring and turning off the last one

    if (analogRead(POTI3) != poti3ModeChange || analogRead(POTI1) != poti1ModeChange)
    {
      bright = map(analogRead(POTI1), 0, 1024, 5, 254);
      cycle = map(analogRead(POTI3), 0, 1024, 100, 500);
    }

    pixels.setBrightness(bright);

    if (stateStart < (millis() - cycle / 10))
    {
      spinCurrentHead = (spinCurrentHead + 1) % PIXELS;
      spinCurrentTail = (spinCurrentTail + 1) % PIXELS;
      pixels.setPixelColor(spinCurrentHead, pixels.Color(red, green, blue));
      pixels.setPixelColor(spinCurrentTail, pixels.Color(0, 0, 0));
      pixels.show();

      stateStart = millis();
    }

    break;
  }
  case 2:
  { // Strobe mode
    if (analogRead(POTI3) != poti3ModeChange || analogRead(POTI1) != poti1ModeChange)
    {
      bright = map(analogRead(POTI1), 0, 1024, 5, 254);
      cycle = map(analogRead(POTI3), 0, 1024, 20, 300);
    }

    if (cycle > 5)
    { // solid if "faster" than 5 ms
      if (stateStart < (millis() - cycle))
      { // time to change state
        if (blinkstate == 1)
        { // it's on, turn it off
          pixels.setBrightness(0);
          for (i = 0; i < PIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(red, green, blue));
          pixels.show();
          stateStart = millis();
          blinkstate = 0;
        }
        else
        { // it's off turn it on
          pixels.setBrightness(bright);
          for (i = 0; i < PIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(red, green, blue));
          pixels.show();
          stateStart = millis();
          blinkstate = 1;
        }
      }
    }
    else
    { // if the blink cycle < 5 ms
      pixels.setBrightness(bright);
      for (i = 0; i < PIXELS; i++)
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
      pixels.show();
    }
    break;
  }
  }
}

void setMode()
{
  if (mode == 2 && millis() > modeCanChange)
  { // moving ring mode
    setPotiState();
    mode = 0;
    modeCanChange = millis() + modeCycle;
  }
  else if (mode == 1 && millis() > modeCanChange)
  { // Strobe mode
    setPotiState();
    mode = 2;
    modeCanChange = millis() + modeCycle;
  }
  else if (mode == 0 && millis() > modeCanChange)
  { // Color changer
    setPotiState();
    mode = 1;
    modeCanChange = millis() + modeCycle;
  }
}

void setPotiState()
{
  poti1ModeChange = analogRead(POTI1);
  poti2ModeChange = analogRead(POTI2);
  poti3ModeChange = analogRead(POTI3);
}

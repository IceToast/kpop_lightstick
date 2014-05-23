/* 
RGB Flashlight v1 - Ben Miller @vmFoo 5-17-2014
Some minor #define snippets borrowed from neopixel examples

The switch on STATEPIN controls state for the inputs
State 1 (LOW) : Color modification:  The potentiometers change red, green, blue values only
State 2 (HIGH): Effect mode: The red pot now changes brightness and the green pot changes
                             the flash delay.  It goes on solid below a flash freq of 5ms

*/


#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  #include <avr/power.h>
#endif

#define REDPIN 1  //silkscreen 2
#define GREENPIN 3 //silkscreen 3
#define BLUEPIN 2 //silkscreen 4
#define STATEPIN 0
#define LEDPIN 1
#define PIXELS 16

/* Initiate variables and set defaults */
int red=128;
int green=128;
int blue=128;
int bright=86; // 1/3 brightness
int cycle = 1000; //1 second
int blinkstate = 1;
int i=0;  //predeclare a counter integer
uint32_t stateStart; //for managing the blink

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  #ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pinMode(STATEPIN, INPUT); 
  pixels.begin();
  pixels.setBrightness(bright); 
  pixels.show();
  stateStart=millis();
}

void loop(){

 if (digitalRead(STATEPIN) == LOW) {  //Color modification mode
   red = map(analogRead(REDPIN), 0,1024, 0, 254);  
   green = map(analogRead(GREENPIN), 0,1024, 0, 254);  
   blue = map(analogRead(BLUEPIN), 0,1024, 0, 254); 
   for( i =0 ; i< PIXELS ; i++)
     pixels.setPixelColor(i, pixels.Color(red, green, blue));
   pixels.setBrightness(bright);
   pixels.show();

 } else {  //Effect control mode
   bright = map(analogRead(REDPIN), 0,1024, 0, 254); 
   cycle = map(analogRead(GREENPIN), 0,1024, 0, 1000);  
   
   if (cycle > 5) { //solid if "faster" than 5 ms
     if (stateStart < (millis()-cycle) ) {  //time to change state
       if (blinkstate==1) {  //it's on, turn it off
         pixels.setBrightness(0);      
         for( i =0 ; i< PIXELS ; i++)
           pixels.setPixelColor(i, pixels.Color(red, green, blue));    
         pixels.show();
         stateStart=millis();
         blinkstate=0;
       } else {  //it's off turn it on
         pixels.setBrightness(bright);
         for( i=0; i< PIXELS ; i++)
           pixels.setPixelColor(i, pixels.Color(red, green, blue));
         pixels.show();
         stateStart=millis();
         blinkstate=1;
       }
     }     
   } else {  //if the blink cycle < 5 ms
      pixels.setBrightness(bright);
      for( i=0; i< PIXELS ; i++)
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
      pixels.show();
   }     
 }
}

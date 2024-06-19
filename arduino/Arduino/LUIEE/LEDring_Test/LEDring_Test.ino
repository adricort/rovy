#include <FastLED.h>
#define NUM_LEDS 24
#define LEDring_DELAY 20

unsigned long timer_prev_LEDring = 0;
unsigned long timer_LEDring = 0;
unsigned long timer_difference_LEDring = 0;
short i = 0;
boolean LEDring_flag = false;
CRGBArray<NUM_LEDS> leds;

void setup() 
{ 
  FastLED.addLeds<NEOPIXEL,6>(leds, NUM_LEDS);
  static uint8_t hue;
  timer_prev_LEDring = millis();
}

void loop()
{ 
  timer_LEDring = millis();
  timer_difference_LEDring = timer_LEDring - timer_prev_LEDring;

  leds.fadeToBlackBy(1);
  leds[i] = CHSV(50,150,160);   // (50,255,150) YELLOW
                               // (100,255,150) GREEN
                               // (150,255,150) BLUE
                               // (200,255,150) PURPLE
                               // (255,255,150) RED
  //leds[i] = CRGB::Black;                   
  leds(NUM_LEDS/2,NUM_LEDS-1) = leds(0,NUM_LEDS/2 - 1 );
  //FastLED.delay(1);
  FastLED.show();
  
  if(i < NUM_LEDS/2)
  {
    if(timer_difference_LEDring > LEDring_DELAY)
    {
      i++;
      timer_prev_LEDring = millis();
    }
  }
  else
  {
    i = 0;
    timer_prev_LEDring = millis();
    LEDring_flag = !LEDring_flag;
  }
}

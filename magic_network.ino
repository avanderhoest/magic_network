#include <Adafruit_NeoPixel.h>

#define LEDPIN 2 
#define LEDS 26

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, LEDPIN, NEO_GRB + NEO_KHZ800);

const int TouchOne=3;
const int TouchTwo=5;
const int TouchThree=6;
const int TouchFour=4;
int led_pos = 0;
int offset = 0;
int led_pos_off =  strip.numPixels();
bool turn_off = false;

void setup() {
    Serial.begin(9600);
    pinMode(TouchOne, INPUT);
    pinMode(TouchTwo, INPUT);
    pinMode(TouchThree, INPUT);
    pinMode(TouchFour, INPUT);

    strip.begin();
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); 
    }
    strip.show();

    randomSeed(analogRead(0));
}

void loop() {
    int sensorOne = digitalRead(TouchOne);
    int sensorTwo = digitalRead(TouchTwo);
    int sensorThree = digitalRead(TouchThree);
    int sensorFour = digitalRead(TouchFour);

    if (sensorOne==1 || sensorTwo==1 || sensorThree==1 || sensorFour==1) {
      turn_off = false;
      
      if (sensorOne == 1) {
        offset = 0;
      }
      
      if (sensorTwo == 1) {
        offset = strip.numPixels()/4;
      }

      if (sensorThree == 1) {
        offset = strip.numPixels()/2;
      }
      
      if (sensorFour == 1) {
        offset = (strip.numPixels()/4 ) * 3;
      }
      
      if (led_pos + offset > strip.numPixels()) {
        led_pos = led_pos - strip.numPixels();
      }
      
      strip.setPixelColor(led_pos + offset, Wheel( (256 / strip.numPixels()) * led_pos ));
      led_pos++;
      led_pos_off = strip.numPixels();
    } else {
      led_pos = 0;
      turn_off = true;
      strip.setPixelColor(led_pos_off, 0); 
      led_pos_off--;
    }

    strip.show();
    
    if (turn_off) {
       delay(60);
    } else {
      delay (random(1,5) * 100);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

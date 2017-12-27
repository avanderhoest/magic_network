#include <Adafruit_NeoPixel.h>

#define LEDPIN 2 
#define LEDS 92

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, LEDPIN, NEO_GRB + NEO_KHZ800);

//ANNICK
int ledstrip[] = {16, 0,18, 17, 13, 5, 11, 8, 10, 23, 25, 41, 39, 38, 36, 73, 76, 78, 79, 46, 52, 56, 58, 87, 10, 85, 82, 84, 62, 65, 67}; 
int touchIndicatorLeds1[] = {16, 0, 17, 18};//NOG JUISTE LEDS INZETTEN
int touchIndicatorLeds2[] = {65, 67, 84};//NOG JUISTE LEDS INZETTEN

//LILLY:
//int ledstrip[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19, 20, 21, 22, 23}; //NOG JUISTE LEDS INZETTEN
//int touchIndicatorLeds1[] = {0,1,2,3};//NOG JUISTE LEDS INZETTEN
//int touchIndicatorLeds2[] = {21, 22, 23};//NOG JUISTE LEDS INZETTEN
 
int nrLeds = 0;

int SCREENSAVER_TIMEOUT = 3000;

int mode = 0;
int prevMode = -1;
const int MODE_TURN_OFF = 0;
const int MODE_RIGHT_RAINBOW = 1;
const int MODE_LEFT_LINE = 2;
const int MODE_COMBINED_ENERGY = 3;

//ANNICK:
const int TouchOne = 4;
const int TouchTwo = 3;

//LILLY:
//const int TouchOne = 3;
//const int TouchTwo = 4;

//const int TouchThree=6;
//const int TouchFour=4;

unsigned long timestampSinceLastActivity = 0;
unsigned long timestampSinceScreenSaverActivation = 0;

int led_pos = 0;
int led_pos_left = 0;

int offset = 0;
int led_pos_off = sizeof(ledstrip);
bool turn_off = false;

void setup() {
    Serial.begin(9600);
    pinMode(TouchOne, INPUT);
    pinMode(TouchTwo, INPUT);
    //pinMode(TouchThree, INPUT);
    //pinMode(TouchFour, INPUT);

    strip.begin();
    for(uint16_t i=0; i < led_pos_off; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); 
    }
    strip.show();

    //for(uint16_t i=0; i < sizeof(ledstrip); i++) {
    //  strip.setPixelColor(ledstrip[i], strip.Color(100 + i * 20, i * 20, 50 + i * 10)); 
   // }
    strip.show();

    randomSeed(analogRead(0));
    nrLeds = sizeof(ledstrip)/sizeof(int);
}

void loop() {
    int sensorOne = digitalRead(TouchOne);
    int sensorTwo = digitalRead(TouchTwo);

      //nothing pressed
      prevMode = mode;
      mode = MODE_TURN_OFF;  

      //sensor 1 pressed means wheel behaviour
      if (sensorOne == 1) {
        offset = 0;
        mode = MODE_RIGHT_RAINBOW;
      }

      //sensor 2 pressed means energy charge
      if (sensorTwo == 1) {
        offset = 0;
        mode = MODE_LEFT_LINE;
      }

     //both charged means COMBINED ENERGY !!!!
      if (sensorOne == 1 && sensorTwo == 1) {
        mode = MODE_COMBINED_ENERGY;  
      }
     
     //Serial.println("Time: ");
     //Serial.print(millis());
     UpdateLeds(mode, prevMode);
      
}

int colorCyclePos = 0;
int screensaverPos = 0;
bool screenSaverInit = false;
bool turnOffFromLeft = false;
int indexWheel1;
int indexWheel2;

unsigned long timestampSinceActivationOfMode = 0;
unsigned long timeInModeInSeconds = 0;
unsigned long delayDecrease = 0;

int easterEggCounter = 0;
int easterEggEvent = 0;
int easterLooper = 0;

void UpdateLeds(int aMode, int aPrevMode) {
  
  int delayMS = 0;

  if (aMode != MODE_TURN_OFF) {
    timestampSinceLastActivity = millis();
    timestampSinceScreenSaverActivation = millis();
  }

   if (aMode != aPrevMode) {
      timestampSinceActivationOfMode = millis();
      timestampSinceLastActivity = millis();
      timeInModeInSeconds = 0;
    } else {
     timeInModeInSeconds = (millis() - timestampSinceActivationOfMode) / 1000;    
    }
   
  switch(aMode) {
    case MODE_TURN_OFF: //NO SIDE PRESSED : 
      if (aMode != aPrevMode) {
        SCREENSAVER_TIMEOUT = 3000;
        timestampSinceScreenSaverActivation = millis();
      }

     if (millis() - timestampSinceLastActivity < SCREENSAVER_TIMEOUT) {
      led_pos = 0;
      strip.setPixelColor(ledstrip[led_pos_off], 0); 

      if (!turnOffFromLeft)
        led_pos_off--;
      else
        led_pos_off++;
      
      delayMS = 60; 
      screenSaverInit = false;
     } else { 
      if (!screenSaverInit) {
        screenSaverInit = true;
        timestampSinceScreenSaverActivation = millis();
        easterEggCounter++;
      }

    
     if (millis() - timestampSinceScreenSaverActivation > 5000) {
        for(int j = 0; j < sizeof(touchIndicatorLeds1); j++) {
           strip.setPixelColor(touchIndicatorLeds1[j], 0); 
        }

        for(int k = 0; k < sizeof(touchIndicatorLeds2); k++) {
           strip.setPixelColor(touchIndicatorLeds2[k], 0); 
        }

        timestampSinceScreenSaverActivation = millis();
        timestampSinceLastActivity = millis(); //will make it switch back to clearing
        SCREENSAVER_TIMEOUT = 60000;
        led_pos_off = nrLeds;

        if (easterEggCounter > easterEggEvent) {
          easterEggCounter = 0;
          easterEggEvent = random(5, 15); 
        }
      }
            
        screensaverPos++;

        if (screensaverPos > 255)
          screensaverPos = 0;

          //NORMAL SCREENSAVER      
        if (easterEggCounter < easterEggEvent) {
          delayMS = 40; 
          for(int j = 0; j < sizeof(touchIndicatorLeds1); j++) {
             strip.setPixelColor(touchIndicatorLeds1[j], Wheel(screensaverPos)); 
          }
  
          for(int k = 0; k < sizeof(touchIndicatorLeds2); k++) {
             strip.setPixelColor(touchIndicatorLeds2[k], Wheel(screensaverPos)); 
          }
        } else { //EASTER EGG SCREENSAVER
            delayMS = 5;
            easterLooper++;
            int easterIndex = (screensaverPos + easterLooper) % 256;
            for(int m = 0; m < nrLeds; m++) {
                strip.setPixelColor(ledstrip[m], Wheel(easterIndex)); 
            }
        }
        
        led_pos_off = nrLeds;
        turnOffFromLeft = false;
      
      } 
    break;
    case MODE_RIGHT_RAINBOW: //ONLY RIGHT SIDE PRESSED
 
      if (mode != aPrevMode){
        led_pos = 0;
      }
        
      if (led_pos + offset > nrLeds) {
        led_pos = led_pos - nrLeds;
      }

      colorCyclePos++;
      colorCyclePos = colorCyclePos % 256;
     
      indexWheel1 = ((256 / nrLeds) * (led_pos + 1)) + colorCyclePos;
      indexWheel1 = indexWheel1 % 256;

      strip.setPixelColor(ledstrip[led_pos + offset], Wheel(indexWheel1));
      led_pos++;
      led_pos_off = nrLeds;

      delayDecrease = timeInModeInSeconds * 5;
      if (delayDecrease > 90)
        delayDecrease = 90;
      
      delayMS = random(1,5) * (100 - delayDecrease);
      turnOffFromLeft = false;
   
    break; 
    case MODE_LEFT_LINE: //ONLY LEFT SIDE PRESSED
      //Serial.print("LEFT LINE");
      if (mode != aPrevMode)
        led_pos_left = nrLeds;
      
      if (led_pos_left < 0) {
        led_pos_left = nrLeds + led_pos_left;
      }

      colorCyclePos--;
      indexWheel2 = ((256 / nrLeds) * (led_pos + 1)) + colorCyclePos;
      indexWheel2 = indexWheel2 % 256;
      strip.setPixelColor(ledstrip[led_pos_left + offset], Wheel(indexWheel2));
      
      led_pos_left--;
      led_pos_off = 0;
      turnOffFromLeft = true;
      
      delayDecrease = timeInModeInSeconds * 5;
      if (delayDecrease > 90)
        delayDecrease = 90;
       
      delayMS = random(1,5) * (100 - delayDecrease);
    break;
    case MODE_COMBINED_ENERGY: //BOTH SIDES PRESSED
     //Serial.print("COMBINED ENERGY");
     //
    led_pos_off = nrLeds;
    turnOffFromLeft = false;
     
     if (mode != aPrevMode) {
      led_pos = 0;
      led_pos_left = nrLeds;
     }

      delayDecrease = timeInModeInSeconds * 10;
      if (delayDecrease > 110)
        delayDecrease = 110;

      delayMS = 120 - delayDecrease;
      
     led_pos++;
     led_pos_left--;

     if (led_pos + offset > nrLeds) {
        led_pos = led_pos - nrLeds;
      }

      if (led_pos_left < 0) {
        led_pos_left = nrLeds + led_pos_left;
      }

     colorCyclePos--;
     colorCyclePos = colorCyclePos % 256;

     uint32_t color;
     uint32_t RED = strip.Color(100, 230, 50);
     uint32_t BLUE = strip.Color(50, 100, 230);
     uint32_t PURPLE = Wheel(colorCyclePos); 
    
     color = RED;
    
     if (led_pos > led_pos_left)
       color = PURPLE;

     strip.setPixelColor(ledstrip[led_pos], color);

     if (color != PURPLE)
      color = BLUE;

     strip.setPixelColor(ledstrip[led_pos_left + offset], color);
    
    break;
  }   
  
   strip.show();
      delay(delayMS); 
 
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

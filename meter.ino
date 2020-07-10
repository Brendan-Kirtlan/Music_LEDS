#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 300

CRGB leds[NUM_LEDS];
const int soundpinA = A4;
const int baseLine = 522;

int r,g,b;
int turn = 0;
int lastVol = 0;
int counter = 0;
CRGB white = CRGB(50,50,50);
CRGB rep;
CRGB off = CRGB(0,0,0);

void setup() {
  //Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
}

void loop() {
  
  int top = 0;

  //finds max sound for 10 polls of the sensor
  for(int i = 0; i < 10; i++){
    int sound = analogRead(soundpinA);
    int diff = abs(baseLine - sound);
    if(diff > top){
      top = diff;
    }
  }

  //Every 5 loops, changes the base color of the meter
  if(counter == 4){
    nextColor();
    counter = 0;
  }

  //Sets the replace color with the changed r g b values
  rep = CRGB(r,g,b);
  volume(top * 1.5);
  FastLED.show();
  counter++;
}

void slide(){
  CRGB temp = leds[0];
  for(int i = 0; i < 299; i++){
    leds[i] = leds[i+1];
  }
  leds[299] = temp;
}

void volume(int vol){
  //Makes sure to not get an IOB error
  if(vol > 149){
    vol = 149;
  }
  //Stops from flickering when low volume is passed
  else if(vol < 5){
    vol = 5;
  }
  //If sound measured is less than the last sound, it makes it only 2 less to create a smooth sliding effect, without it it's a seizure nightmare.
  if(vol < lastVol){
    vol = lastVol - 2;
  }
  //Sets both sides that are no longer lit up by the meter with white
  if(vol < lastVol){
    for(int i = 149 - lastVol; i < (149 - vol); i++){
      leds[i] = white;
    }
    for(int i = 149 + vol; i < (149 + lastVol); i++){
      leds[i] = white;
    }

    //for rainbow background, doesn't look too good, also slow, need tweaking to look good
    
    //for(int i = 0; i < (149 - vol); i++){
    //  leds[i] = colorFromIndex(i);
    //}
    //for(int i = 149 + vol; i < 300; i++){
    //  leds[i] = colorFromIndex(i);
    //}
  }
  else{
    //Sets the meter with the replace color
    for(int i = 149 - vol; i < (149 + vol); i++){
      leds[i] = rep;
    }
  }
  lastVol = vol;
}

//Not used, but could be cool to convert volume to color, should be tweaked to look good
CRGB volumeToColor(int volume){
  int red, green, blue;
  volume *= 2;
  if(volume < 101){
    red = 255 - (int)(2.55 * volume);
    green = (int)(2.55 * volume);
    blue = 0;
  }
  else if(volume < 201){
    red = 0;
    green = 255 - (int)(2.55 * (volume - 100));
    blue = (int)(2.55 * (volume - 100));
  }
  else{
    red = (int)(2.55 * (volume - 200));
    green = 0;
    blue = 255 - (int)(2.55 * (volume - 200));
  }

  return CRGB(red,green,blue);
}

//Would make a rainbow if given all the whole array, not used either
CRGB colorFromIndex(int index){
  int red,green,blue;
  if(index < 101){
    red = 255 - (int)(2.55 * index);
    green = (int)(2.55 * index);
    blue = 0;
  }
  else if(index < 201){
    red = 0;
    green = 255 - (int)(2.55 * (index - 100));
    blue = (int)(2.55 * (index - 100));
  }
  else{
    red = (int)(2.55 * (index - 200));
    green = 0;
    blue = 255 - (int)(2.55 * (index - 200));
  }
  CRGB returnColor = CRGB(red,green,blue);

  return returnColor;
}

//Cycles through the spectrum by 5, turn variable tracks which color it's on
void nextColor(){
  if(turn == 0){
    r -=5;
    g += 5;
  }
  else if(turn == 1){
    g -=5;
    b += 5;
  }
  else{
    b -=5;
    r += 5;
  }
  if(r == 255){
    turn = 0;
  }
  if(g == 255){
    turn = 1;
  }
  if(b == 255){
    turn = 2;
  }
  if(r < 0){
    r = 0;
  }
  if(g < 0){
    g = 0;
  }
  if(b < 0){
    b = 0;
  }
}

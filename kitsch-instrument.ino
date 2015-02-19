// The Kitsch-Instrument code
//
// Jiffer Harriman
// modular-muse.com


#include "buttonStruct.h"
// this version also can blink some lights to the 
// beat using the AdaFruit neopixel
#include <Adafruit_NeoPixel.h> 

buttonStruct playButton = {
  2, LOW, LOW, 0, 0};

buttonStruct solButton1 = {
  4, LOW, LOW, 0, 0};
buttonStruct solButton2 = {
  7, LOW, LOW, 0, 0};
buttonStruct solButton3 = {
  8, LOW, LOW, 0, 0};
buttonStruct solButton4 = {
  12, LOW, LOW, 0, 0};

int debounceTime = 30;
int count = 0;

// running or not?
boolean play = true;
long lastPlayChange = 0;

// which pins are solenoids attached to?
const int sol1 = 3;
const int sol2 = 5;
const int sol3 = 6;
const int sol4 = 9;

boolean solOn1 = false;
boolean solOn2 = false;
boolean solOn3 = false;
boolean solOn4 = false;

unsigned long lastHit = 0;
unsigned long lastHit1 = 0;
unsigned long lastHit2 = 0;
unsigned long lastHit3 = 0;
unsigned long lastHit4 = 0;


int tempo = 200;
const int onTime = 30; // how long to keep the solenoid on? I like 30ms
boolean swingBeat = false; // feeling jazzy?
boolean delayBeat = false; // used for swing mode

const int neoPin = 10;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, neoPin, NEO_GRB + NEO_KHZ800);

void setup(){
  // Assign pins
  //  playButton.pin = 2;
  //  playButton.lastState = LOW;
  //  playButton.lastDebounceTime = 0;
  // pinMode(playButton, INPUT);
  pinMode(sol1, OUTPUT);
  pinMode(sol2, OUTPUT);
  pinMode(sol3, OUTPUT);
  pinMode(sol4, OUTPUT);

  Serial.begin(9600);

  strip.begin();
  strip.show();

}

// main loop!
void loop(){
  // Check play button
  playButton = readDebounce(playButton);
  if(playButton.bang){
    play = !play;
  }


  if(play){
    chanceDance();
  }
  else // manual play mode
  {
    manualPlay();
  }

// blink the lights!
  int r = 0;
  int g = 0;
  int b = 0;
  if(solOn1 == true){
    r+=50;
    g +=100;
  }
  if(solOn2 == true){
    g+=50;
    b +=100;
  }
  if(solOn3 == true){
    b+=50;
    r +=100; 
  }
  if(solOn4 == true){
    r +=50;
    g +=50;
    b +=50;
  }

  boolean showCount = false;
  if(r == 0 && g == 0 && b == 0 && play){
    showCount = true;
  }
  for(int i = 0; i < 8; i++){
    if (showCount && i == count){
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    else{
      strip.setPixelColor(i, strip.Color(r, g, b));
    }
  }
  strip.show();

}
// end main loop!

// deal with button inputs
buttonStruct readDebounce(buttonStruct button){

  int reading = digitalRead(button.pin);
  button.bang = false;

  if(reading != button.lastState){
    // reset timer
    button.lastDebounceTime = millis();
  }

  if((millis() - button.lastDebounceTime) > debounceTime){
    {
      if(reading!= button.state){
        button.state = reading;
        if( button.state == LOW){
          button.bang = true;
        }
      }
    }
  }
  button.lastState = reading;
  return button;
}


// algorithmic pattern based on values at analog pins A0-A3, 
// pin A4 controls tempo
void chanceDance(){
  tempo = map(analogRead(A4), 0, 1023, 1000, 50);

  if(swingBeat){
    if(delayBeat){
      tempo = tempo + .5*tempo;
    }
  }

  if ((millis() - lastHit) > tempo){
    count++;
    count %= 8;
    lastHit = millis();
    delayBeat = !delayBeat;

    if(random(100) < analogRead(A0)/10.240){
      solOn1 = true;
      digitalWrite(sol1, HIGH);
    }

    if(random(100) < analogRead(A1)/10.240){
      solOn2 = true;
      digitalWrite(sol2, HIGH);
    }

    if(random(100) < analogRead(A2)/10.240){
      solOn3 = true;
      digitalWrite(sol3, HIGH);
    }

    if(random(100) < analogRead(A3)/10.240){
      solOn4 = true;
      digitalWrite(sol4, HIGH);
    }
  }

  if(solOn1 && (millis() - lastHit > onTime)){
    solOn1 = false;
    digitalWrite(sol1, LOW);
  }
  if(solOn2 && (millis() - lastHit > onTime)){
    solOn2 = false;
    digitalWrite(sol2, LOW);
  }
  if(solOn3 && (millis() - lastHit > onTime)){
    solOn3 = false;
    digitalWrite(sol3, LOW);
  }
  if(solOn4 && (millis() - lastHit > onTime)){
    solOn4 = false;
    digitalWrite(sol4, LOW);
  }
}


void manualPlay(){

  // debounce buttons and assign to a short pulse
  solButton1 = readDebounce(solButton1);
  solButton2 = readDebounce(solButton2);
  solButton3 = readDebounce(solButton3);
  solButton4 = readDebounce(solButton4);

  // if a new bang turn the solenoid on
  if(solButton1.bang){
    solOn1 = true;
    lastHit1 = millis();
    digitalWrite(sol1, HIGH);
  }
  if(solButton2.bang){
    solOn2 = true;
    lastHit2 = millis();
    digitalWrite(sol2, HIGH);
  }

  if(solButton3.bang){
    solOn3 = true;
    lastHit3 = millis();
    digitalWrite(sol3, HIGH);
  }
  if(solButton4.bang){
    solOn4 = true;
    lastHit4 = millis();
    digitalWrite(sol4, HIGH);
  }

  // if its been "onTime" milliseconds since a solenoid was fired, turn it off
  if(solOn1 == true && millis() - lastHit1 > onTime)
  {
    solOn1 = false;
    digitalWrite(sol1, LOW);
  }
  if(solOn2 == true && millis() - lastHit2 > onTime)
  {
    solOn2 = false;
    digitalWrite(sol2, LOW);
  }
  if(solOn3 == true && millis() - lastHit3 > onTime)
  {
    solOn3 = false;
    digitalWrite(sol3, LOW);
  }
  if(solOn4 == true && millis() - lastHit4 > onTime)
  {
    solOn4 = false;
    digitalWrite(sol4, LOW);
  }
}








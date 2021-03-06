#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define PIN2 6
#define NUMPIXEL 32
#define NUMPIXEL2 26


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXEL, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXEL2, PIN2, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.



int lightOn;
int lightOn2;
int lightOnMax;
int moveLight;

int avarageB;
int avarageA;
int sampleSize = 5;

int breathSensor;
int breathSensorMax;
int breathSensorMin;

int armSensor;
int armSensorMax;
int armSensorMin;

int breathPin = A3;
int armBendPin = A2;
int calibButtonPin = 10;

boolean buttonState;
boolean lastButtonState;

int R = 255;
int G = 114;
int B = 0;

boolean armSensorOn = true;
boolean breathSensorOn = true;

int animLight = 1;

int sensorOffcntr;

int fakeCounter;
int fakeMax;
int inc=1;

int fakeArmCnt;
int fakeArmMax;
int arminc=1;

boolean fakeArmOn=false;

void setup() {

  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(255);

  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip2.setBrightness(255);

  pinMode(calibButtonPin, INPUT_PULLUP);
  //pinMode(breathPin,INPUT_PULLUP);
  //pinMode(armBendPin,INPUT_PULLUP);
 

  
 delay(500);

  //avarageB = analogRead(breathPin);
 // avarageA = analogRead(armBendPin);


//
//  if (avarageB < 1023){
//    breathSensorOn = true;
//  }
//  else {
//    breathSensorOn = false;
//    Serial.println("no breathing sensor calibration");
//  }
//  if (avarageA < 1023) {
//    armSensorOn = true;
//  }
//  else{
//    armSensorOn = false;
//    Serial.println("no arm sensor calibration");
//  }

/*
  if (breathSensorOn){
    calibrationBreath();
  }
  else {
    breathSensorMax=1015;
    breathSensorMin=800;
  }
  if (armSensorOn){
    calibrationArm();
  }
  else {
    armSensorMax=600;
    armSensorMin=200;
  }
*/
 breathSensorMin=0 ;
 breathSensorMax=516;

armSensorMax=255;
armSensorMin=0;

}


void loop() {

/*
  // read breathing sensor
  breathSensor = analogRead(breathPin);

  // read arm bend sensor
  armSensor = analogRead(armBendPin);
 

// check if the sensor is still there

  if (breathSensor < 1019) {
    breathSensorOn = true;
    sensorOffcntr=0;
  }
  else {
    sensorOffcntr++;
  }
  if (armSensor< 1023) {
    armSensorOn = true;
  }
  else {
   // armSensorOn = false;
  }

// if the breathsensor stays 1019 or bigger for loing time, then go to full mode
if (sensorOffcntr>2000){
  breathSensorOn = false;
}
  //-----------------------------------

  // smooth out with avarage
  breathSensor = ( avarageB * (sampleSize - 1) + breathSensor) / sampleSize;
  avarageB = breathSensor;

  // smooth out with avarage
  armSensor = ( avarageA * (sampleSize - 1) + armSensor) / sampleSize;
  avarageA = armSensor;


  // read calib button and call the callibration if necessary
  buttonState = digitalRead(calibButtonPin);
  if (buttonState != lastButtonState && buttonState == LOW) {
    if (breathSensorOn)calibrationBreath();
    if (armSensorOn)calibrationArm();
  }
  lastButtonState = buttonState;
  //------------------------------------

  Serial.print("breathSensor :");
  Serial.print(breathSensor);

  Serial.print(" \t ");
  Serial.print("armSensor :");
  Serial.print(armSensor);
*/

// fake breath
breathSensor=fakeBreath();





  int totalPixelNum=NUMPIXEL + NUMPIXEL2;

  if (breathSensorOn) {  
    // map the readhing of breath sensor to which light should be on
    breathSensor = constrain(breathSensor, breathSensorMin, breathSensorMax);
    lightOn = map(breathSensor, breathSensorMin, breathSensorMax,-1, totalPixelNum);
    Serial.println(lightOn);
  }
  else {
      
//    if (lightOn <= 0 || lightOn >= totalPixelNum) {
//      animLight = -animLight;
//    }
//    lightOn = lightOn + animLight;

  // all of them are on
  lightOn=totalPixelNum;
     delay(10);
  }
  
  lightOn=constrain(lightOn,0, totalPixelNum);
  
//  Serial.print(" \t ");
//  Serial.print("lightOn :");
//  Serial.print(lightOn);
//
//  Serial.print(" \t ");
//  Serial.print("animationMode :");
//  Serial.print(breathSensorOn);
//
//  Serial.print(" \t ");
//  Serial.print("cntr :");
//  Serial.print(sensorOffcntr);
//
//  
//  Serial.println();

//fake arm
long timing = millis();
if (timing%1078 == random(300)){
  fakeArmOn=true;
      if (fakeArmCnt<=0){
    fakeArmMax = random(5,22);
    fakeArmMax = fakeArmMax*12;
    fakeArmMax = constrain (fakeArmMax, 0,armSensorMax);
    arminc = random (1,4);

  }
}
if (fakeArmOn){
armSensor=fakeArm();
if (armSensor <=0){
  fakeArmOn=false;
}
}


  if (armSensorOn) {
    // decide the color of pixel with arm sensor
    armSensor = constrain(armSensor, armSensorMin, armSensorMax);
    R = map(armSensor, armSensorMin, armSensorMax, 255,114);
    G = map(armSensor, armSensorMin, armSensorMax, 114,255);
    B = map(armSensor, armSensorMin, armSensorMax, 0, 0);
  }
  else {
    R = 255;
    G = 114;
    B = 0;
  }

  for (int i = 1; i < NUMPIXEL+1; i++) {
    
    
    if (i < lightOn) {
      strip.setPixelColor(NUMPIXEL- i, strip.Color(R, G, B) );
    }
    else  strip.setPixelColor(NUMPIXEL- i, strip.Color(0, 0, 0) );
  }

  for (int i = 0; i < NUMPIXEL2; i++) {
    if (i < lightOn - NUMPIXEL) {
      strip2.setPixelColor(i, strip2.Color(R, G, B) );
    }
    else  strip2.setPixelColor(i, strip2.Color(0, 0, 0) );
  }



  strip.show();
  strip2.show();


  delay(10);
}


void calibrationBreath() {
  breathSensorMax = 0;
  breathSensorMin = 1023;

  Serial.println("calibration Breath sensor starts");

  for (int i = 0; i < 1000; i++) {
    breathSensor = analogRead(breathPin);
    Serial.print("breathSensor : ");
    Serial.println(breathSensor);
    if (breathSensor > breathSensorMax) {
      breathSensorMax = breathSensor;
    }
    if (breathSensor < breathSensorMin) {
      breathSensorMin = breathSensor;
    }
    // map the readhing of breath sensor to which light should be on
    breathSensor = constrain(breathSensor, breathSensorMin, breathSensorMax);
    lightOn = map(breathSensor, breathSensorMin, breathSensorMax, -1,NUMPIXEL);


    for (int i = 0; i < NUMPIXEL; i++) {
      if (i < lightOn) {
        strip.setPixelColor(NUMPIXEL - i, strip.Color(0, 255, 0) );
      }
      else  strip.setPixelColor(NUMPIXEL - i, strip.Color(0, 0, 0) );
    }
    strip.show();
    delay(10);
  }

  for (int i = 0; i < NUMPIXEL; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0) );
  }
  strip.show();

    // in case there was no sensor input during the calibration, use the default min/max value
if (breathSensorMax-breathSensorMin <10){
  breathSensorMax=1015;
    breathSensorMin=800;
}
  Serial.print("breathSensorMin :");
  Serial.print(breathSensorMin);
  Serial.print("\t");
  Serial.print("breathSensorMax :");
  Serial.print(breathSensorMax);
  Serial.println();
  Serial.println("calibration Breath sensor ends");
}

void calibrationArm() {
  armSensorMax = 0;
  armSensorMin = 1023;
  Serial.println("calibration Arm sensor starts");

  for (int i = 0; i < 1000; i++) {

    armSensor = analogRead(armBendPin);
    Serial.print("armSensor : ");
    Serial.println(armSensor);
    if (armSensor > armSensorMax) {
      armSensorMax = armSensor;
    }
    if (armSensor < armSensorMin) {
      armSensorMin = armSensor;
    }

    armSensor = constrain(armSensor, armSensorMin, armSensorMax);
    lightOn2 = map(armSensor, armSensorMin, armSensorMax, NUMPIXEL2,-1);


    for (int i = 0; i < NUMPIXEL2; i++) {
      if (i < lightOn2) {
        strip2.setPixelColor(NUMPIXEL2 - i, strip2.Color(0, 255, 0) );
      }
      else  strip2.setPixelColor(NUMPIXEL2 - i, strip2.Color(0, 0, 0) );
    }
    strip2.show();

    delay(10);
  }

  for (int i = 0; i < NUMPIXEL2; i++) {
    strip2.setPixelColor(i, strip2.Color(0, 0, 0) );
  }
  strip2.show();

    if (armSensorMax-armSensorMin <50){
    armSensorMax=600;
    armSensorMin=200;
}

  Serial.print("armSensorMin :");
  Serial.print(armSensorMin);
  Serial.print("\t");
  Serial.print("armSensorMax :");
  Serial.print(armSensorMax);
  Serial.println();
  Serial.println("calibration arm sensor ends");
}

int fakeBreath(){

  if (fakeCounter<=0){
    fakeMax = random(12,48);
    //fakeMax = fakeMax*12;
    fakeMax =breathSensorMax;
    fakeMax = constrain (fakeMax, 0,breathSensorMax);
    inc = random (2,5);
  }

if (fakeCounter >=fakeMax){
  inc = -inc;
  int pause = random(100, 700);
  delay (pause);
}
fakeCounter= fakeCounter+inc;

fakeCounter= constrain(fakeCounter,0,breathSensorMax);
return fakeCounter;
  
}

int fakeArm(){
  


if (fakeArmCnt >=fakeArmMax){
  arminc = -arminc;
}
fakeArmCnt= fakeArmCnt+arminc;

fakeArmCnt= constrain(fakeArmCnt,0,armSensorMax);
return fakeArmCnt;
  
}



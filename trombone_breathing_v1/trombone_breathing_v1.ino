#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define PIN2 5
#define NUMPIXEL 34
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

void setup() {

  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(128);

  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip2.setBrightness(128);

  pinMode(calibButtonPin, INPUT_PULLUP);
  //pinMode(breathPin,INPUT_PULLUP);
  //pinMode(armBendPin,INPUT_PULLUP);

  
 delay(500);
  avarageB = analogRead(breathPin);
  avarageA = analogRead(armBendPin);



  if (avarageB < 1000){
    breathSensorOn = true;
  }
  else {
    breathSensorOn = false;
    Serial.println("no breathing sensor calibration");
  }
  if (avarageA < 1023) {
    armSensorOn = true;
  }
  else{
   // armSensorOn = false;
  //  Serial.println("no arm sensor calibration");
  }

delay(1000);
  if (breathSensorOn)calibrationBreath();
  if (armSensorOn)calibrationArm();
}

void loop() {


  // read breathing sensor
  breathSensor = analogRead(breathPin);

  // read arm bend sensor
  armSensor = analogRead(armBendPin);

// check if the sensor is still there

  if (breathSensor < 1000) {
    breathSensorOn = true;
  }
  else {
    breathSensorOn = false;
  }
  if (armSensor< 1023) {
    armSensorOn = true;
  }
  else {
   // armSensorOn = false;
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
  

  if (breathSensorOn) {  
    // map the readhing of breath sensor to which light should be on
    breathSensor = constrain(breathSensor, breathSensorMin, breathSensorMax);
    lightOn = map(breathSensor, breathSensorMin+50, breathSensorMax-50,-1, NUMPIXEL + NUMPIXEL2);
  }
  else {
      lightOn = lightOn + animLight;
    if (lightOn <= 0 || lightOn >= NUMPIXEL + NUMPIXEL2) {
      animLight = -animLight;
    }
     delay(10);
  }
  
  Serial.print(" \t ");
  Serial.print("lightOn :");
  Serial.print(lightOn);

  Serial.print(" \t ");
  Serial.print("animationMode :");
  Serial.print(breathSensorOn);

  
  Serial.println();

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

  for (int i = 0; i < NUMPIXEL; i++) {
    if (i < lightOn) {
      strip.setPixelColor(NUMPIXEL - i, strip.Color(R, G, B) );
    }
    else  strip.setPixelColor(NUMPIXEL - i, strip.Color(0, 0, 0) );
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

  for (int i = 0; i < 500; i++) {
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
        strip.setPixelColor(NUMPIXEL - i, strip.Color(128, 255, 0) );
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

  for (int i = 0; i < 500; i++) {

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
        strip2.setPixelColor(NUMPIXEL2 - i, strip2.Color(128, 255, 0) );
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

  Serial.print("armSensorMin :");
  Serial.print(armSensorMin);
  Serial.print("\t");
  Serial.print("armSensorMax :");
  Serial.print(armSensorMax);
  Serial.println();
  Serial.println("calibration arm sensor ends");
}




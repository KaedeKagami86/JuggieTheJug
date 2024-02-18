// import libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #define are macros (like constants) that  won't change. They're used here to set pin numbers.
// setting up force sensor, ultrasonic sensor, moving motor 1, moving motor 2
#define echoPin 10 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 11 // attach pin D3 Arduino to pin Trig of HC-SR04
#define FORCE_SENSOR_PIN A3 // the FSR and 10K pulldown are connected to A3
#define motor1input1 2 // In1
#define motor1input2 3 // In2
#define motor2input1 4 // In3
#define motor2input2 5 // En2

// define screen size (if your OLED 123x64)
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//define constants
const unsigned long TimeInterval = 5.0; // 5sec = 5000 millisec (it actually displays 4)
const long ExpectedDistance = 10.0; // how far away we want the target to be
const int accidental_touch_value = 20; // from 0 to 20, just in case it's LIGHTLY touched

// defines variables
// for ultrasonic
long duration; // variable for the duration of sound wave travel
long distance;  // variable for the distance measurement
long diff;

// for timer
unsigned long StartTime;
unsigned long TimeElapsed;
unsigned long pervious_time = 0;
int CountDown = 100; //whatever just to make the countdown work later

// OLED object
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin

  //Set all the pin as OUTPUT
  pinMode(motor1input1,OUTPUT);
  pinMode(motor1input2,OUTPUT);
  pinMode(motor2input1,OUTPUT);
  pinMode(motor2input2,OUTPUT);

  //Set all the pin to Logic HIGH to initially turn off the motor
  digitalWrite(motor1input1,HIGH);
  digitalWrite(motor1input2,HIGH);
  digitalWrite(motor2input1,HIGH);
  digitalWrite(motor2input2,HIGH);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display
  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 0);        // position to display
  StartTime = millis();

}

void loop() {
  // put your main code here, to run repeatedly:

}


// ---------- CHASE_TARGET & DISTANCE HELPERS----------
void chase_target() {
  diff = difference();
  if (diff < 10) {
    digitalWrite(motor1input1, HIGH);
    digitalWrite(motor2input1, HIGH);
  } else {
    digitalWrite(motor1input1, LOW);
    digitalWrite(motor2input1, LOW);
  }
}

long current_distance() {
  // Clears the trigPin condition
  // delay POTENTIALLY causes problem?? probably not, but just to be cautious
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2.0; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance; //in cm, should be an int
}

int difference() {
  int distance = current_distance();
  return distance - ExpectedDistance;
}

// ---------- DISPLAY_TIME & TIME HELPERS ----------
void display_time() {
  TimeElapsed = time_elapsed();
  if ((TimeElapsed != pervious_time) && (CountDown > 0)) {
    pervious_time = TimeElapsed;
    CountDown = TimeInterval - TimeElapsed;
    oled.setCursor(60, 10);  
    oled.clearDisplay();
    oled.print(CountDown);
    oled.display();   // show on OLED
  }
}

unsigned long time_elapsed() {
  unsigned long CurrentTime = millis();
  return (CurrentTime - StartTime)/1000; 
}

void reset_time() {
  TimeElapsed = 0;
}

// ---------- FORCE HELPER ----------
bool force_sensed() {  // run this in loop!!!!!!
  int analogReading = analogRead(FORCE_SENSOR_PIN);
  if (analogReading < accidental_touch_value)       
    return false;
  else {
    return true;
  }
}

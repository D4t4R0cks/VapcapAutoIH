#include <math.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <Adafruit_MLX90614.h>

// On my board:
//Yellow is red, and goes on green.
//Red is green and goes on Red
//Green is blue and goes on Blue

//IRTherm therm; // Create an IRTherm object to interact with throughout
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Arduino pin numbers
const int SW_pin = 10; // digital pin connected to switch output
const int MOSFET_pin = 9;
const int greenpin = 3;
const int bluepin = 5;
const int redpin = 6;
const int boardLedPin = 13;

int safetyReached = 0;  // Heave we hit a safety limit
int pushPower = 0;     // Is the heater on
int maxsafetyTime = 15000; // How long the heater can remain on
int colortest = 0;     // 0 regular funciton  ; 1 test RGB
int  automaticTemp = 0; // 0 manual ; 1 Automatic
int waitForSecondClick = 3000;
int calibrationRun = 1; // Are we in the initial loop of the Automatic mode where we "record" the heat temp.
unsigned long pushedTime = 0; // How lon the heater has been on
int increase = 1; // How much to increase the LED color on each cycle
int intensity = 0; // How bright the color is
unsigned long timeSinceBlink = 0;
unsigned long currentMillis = 0;
double objectTemp; // Temperature of the object infront of sensor
double sensorTemp; // Ambient Temperature, or temperature of the sensor itself.
int releaseTemp = 180; // Default releasing temperature if no calibration run has happened.

// Function prototype otherwise it won't compile if the file is in another tab
void sleepColor(char color = 'w', int colorChange = 1, int blinkDelay = 0);

void setup()
{
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(redpin, OUTPUT);
  pinMode(MOSFET_pin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(boardLedPin, OUTPUT);

  Serial.begin(9600); // Initialize Serial to log output
  mlx.begin();

  Serial.print("All init");

  SetupManualOrAutomaticMode();
}


void loop()
{
  // colorTest(); // This is the mode for testing RGB values from the serial monitor
  ManualOrAutomaticMode();
}


void manualMode()
{
  int sw = digitalRead(SW_pin);

  if (sw == LOW && !safetyReached && !pushPower)
  {
    Serial.println("Pushed On!");
    Serial.println(sw);
    TurnHeaterOn();
  }
  else if (pushPower && sw != LOW)
  {
    Serial.println(sw);
    Serial.println("Pushed OFF");
    TurnHeaterOff();
    if (calibrationRun)
    {
      releaseTemp =  mlx.readObjectTempF();;
      calibrationRun = 0;
      Serial.println("Calibration Mode OFF");
      Serial.println("Temp:" + releaseTemp);
    }
    delay(400);
  }
}


void automaticMode()
{

  int sw = digitalRead(SW_pin);

  if (sw == LOW && !safetyReached && !pushPower)
  {
    Serial.println("Pushed On!");
    TurnHeaterOn();
    // digitalWrite(boardLedPin, HIGH);
    delay(400);
    // digitalWrite(boardLedPin, LOW);
  }
  else if ((sw == LOW && pushPower) )
  {
    Serial.println("Pushed OFF");
    TurnHeaterOff();
    delay(400);
  }
}

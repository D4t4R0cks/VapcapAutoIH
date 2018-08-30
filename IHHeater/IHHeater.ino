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

  int awaitingCommand = 1;
  automaticTemp = 0;
  int sw;

  unsigned long delayBetweenPush = 0;

  int firstClick = 0;
  while (awaitingCommand)
  {

    if (!firstClick)
    {
      sleepColor('w', 1, 5);
    }
    else
    {
      sleepColor('g');
    }

    sw = digitalRead(SW_pin);
    //Serial.println("In Loop");

    if (!firstClick && sw == LOW)
    {
      Serial.println("First Push");
      automaticTemp = 0;
      firstClick = 1;
      currentMillis = millis();
      delay(400);
    }
    if (firstClick)
    {
      unsigned long span = millis() - currentMillis;

      if (span < waitForSecondClick)
      {
        sw = digitalRead(SW_pin);
        if (sw == LOW)
        {
          automaticTemp = 1;
          awaitingCommand = 0;
          break;
        }
      }
      else
      {
        awaitingCommand = 0;
      }
    }
  }

  Serial.print("Mode is:");
  Serial.println(automaticTemp);
  delay(1000);
  currentMillis = millis();
}


double objectTemp;
double sensorTemp;
int releaseTemp = 200;
void readTemp()
{
  int red = 0;
  int blue = 0;
  int green = 0;

  objectTemp = mlx.readObjectTempF();
  sensorTemp = mlx.readAmbientTempF();

  Serial.println("F");

  if (automaticTemp && (objectTemp > releaseTemp) )
  {
    Serial.println("AUTO OFF!");
    delay(2000);
    TurnHeaterOff();
  }
  else if (objectTemp > 195)
  {
    red = 200;
    Serial.println("EMERGENCY MAX TEMP OFF!");
    TurnHeaterOff();
    safetyReached = 1;
  }

  else if (objectTemp > 156 && objectTemp < 174)
  {
    red = 100;
    Serial.println("High");
  }

  else if (objectTemp > 136 && objectTemp < 156)
  {
    red = 100;
    green = 70;
    Serial.println("Med");
  }

  else if (objectTemp > 116 && objectTemp < 136)
  {
    red = 50;
    green = 50;
    Serial.println("Med");
  }

  else if (objectTemp < 116 && objectTemp > (sensorTemp + 10))
  {
    blue = 50;
    Serial.println("Low");
  }

  else if (objectTemp < sensorTemp + 10 || objectTemp > sensorTemp - 10)
  {
    green = 50 ;
    Serial.println("Low");
    safetyReached = 0;
  }

  //Serial.println("Object:");
  //Serial.print("100,110,120,130,140,150,160,170,180,");
  Serial.print(objectTemp);
  //Serial.println("Ambient:");
  Serial.print(",");
  Serial.print(sensorTemp);
  Serial.print(",");
  Serial.println(releaseTemp);


  if (sensorTemp > 117)
  {
    Serial.println("AMBIENT EMERGENCY OFF!");
    TurnHeaterOff();
    safetyReached = 1;
  }

  if (!pushPower)
  {
    red = 100;
    blue = 100;
    green = 100;
  }

  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}



void loop()
{
  unsigned long span = millis() - currentMillis;

  if (span > 500)
  {
    readTemp();
    currentMillis = millis();
  }

  //colorTest();

  if (automaticTemp)
  {
    if (calibrationRun)
    {
      //Serial.println("In Calibration Mode.");
      manualMode();
    }
    else
    {
      automaticMode();
    }
  }
  else
  {
    calibrationRun = 0;
    manualMode();
  }

  if ( pushPower && (millis() - pushedTime) > maxsafetyTime)
  {
    Serial.println("Too Long!");
    TurnHeaterOff();
    safetyReached = 1;
  }
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

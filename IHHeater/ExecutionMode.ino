void SetupManualOrAutomaticMode(int firstModeAutomatic)
{
  int awaitingCommand = 1;
  automaticTemp = 0;
  int sw;
  if(!firstModeAutomatic)
  {
    waitForSecondClick = 1000;
  }

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
  
  if(!firstModeAutomatic)
  {
    automaticTemp = 1 - automaticTemp;
  }
  
  Serial.println(automaticTemp);
  delay(1000);
  currentMillis = millis(); 
}

void BothModes()
{
    if(!pushPower)
    {
      SetupManualOrAutomaticMode(0);
    }  

    //Remeber automaticTemp got switched around for calling SetupManualOrAutomaticMode with 0
     if(automaticTemp)
    {
      TurnHeaterOn();
    }
    
    ManualOrAutomaticMode();
    
}


void ManualOrAutomaticMode()
{

  unsigned long span = millis() - currentMillis;

  if (span > 500)
  {
    readTemp();
    currentMillis = millis();
  }

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

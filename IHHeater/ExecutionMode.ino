void BothModes()
{
  
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

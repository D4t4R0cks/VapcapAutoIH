
void sleepColor(char color = 'w', int colorChange = 1, int blinkDelay = 0)
{
  unsigned long span = millis() - timeSinceBlink;
  if (span > blinkDelay)
  {
    timeSinceBlink = millis();
  }
  else
  {
    return;
  }

  if (increase)
  {
    if (intensity == 200)
    {
      increase = 0;
    }
    else
    {
      intensity = intensity + colorChange;
    }
  }
  else
  {
    if (intensity == 0)
    {
      increase = 1;
    }
    else
    {
      intensity = intensity - colorChange;
    }
  }
  if (color == 'w')
  {
    analogWrite(redpin, intensity);
    analogWrite(greenpin, intensity);
    analogWrite(bluepin, intensity);
  }
  else if (color == 'r')
  {
    analogWrite(redpin, intensity);
    analogWrite(greenpin, 0);
    analogWrite(bluepin, 0);
  }
  else if (color == 'g')
  {
    analogWrite(redpin, 0);
    analogWrite(greenpin, intensity);
    analogWrite(bluepin, 0);
  }
  else if (color == 'b')
  {
    analogWrite(redpin, 0);
    analogWrite(greenpin, 0 );
    analogWrite(bluepin, intensity);
  }
}

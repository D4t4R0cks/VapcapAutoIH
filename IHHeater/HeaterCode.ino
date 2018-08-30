void TurnHeaterOff()
{
  digitalWrite(MOSFET_pin, LOW);

  pushPower = 0;
  pushedTime = 0;

}

void TurnHeaterOn()
{
  if (!safetyReached)
  {
    digitalWrite(MOSFET_pin, HIGH);
  }
  else
  {
    digitalWrite(MOSFET_pin, LOW);
  }

  pushPower = 1;
  pushedTime = millis();

}

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

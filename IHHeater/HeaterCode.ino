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

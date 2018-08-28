void colorTest()
{
  String input = Serial.readString();

  int r = 0;
  int b = 0;
  int g = 0;


  while (colortest || input == "c")
  {
    input = Serial.readString();
    Serial.print(input);

    if (input == "r")
    {
      r = r - 5;
    }
    else if (input == "b")
    {
      b = b - 5;
    }
    else if (input == "g")
    {
      g = g - 5;
    }
    if (input == "rr")
    {
      Serial.print("Adding red");
      r = r + 5;
    }
    else if (input == "bb")
    {
      b = b + 5;
    }
    else if (input == "gg")
    {
      g = g + 5;
    }
    else if (input == "?")
    {
      String valuered = "Values: R:";
      String valueblue = " B:";
      String valuegreen = " G:";

      Serial.println(valuered + r + valueblue + b + valuegreen + g);
    }
    else if (input == "x")
    {
      colortest = 0;
    }
    else if (input == "c")
    {
      colortest = 1;
    }

    analogWrite(redpin, r);
    analogWrite(greenpin, g);
    analogWrite(bluepin, b);
  }
}

//www.elegoo.com
//2016.12.08

const int LED_PIN = 13;
const int TILT_PIN = 2;

void setup()
{
  pinMode(LED_PIN, OUTPUT); // initialize the ledPin as an output
  pinMode(TILT_PIN, INPUT);
  digitalWrite(TILT_PIN, HIGH);
}

void loop()
{
  int digitalVal = digitalRead(TILT_PIN);
  if (HIGH == digitalVal)
  {
    digitalWrite(LED_PIN, LOW); // turn the led off
  }
  else
  {
    digitalWrite(LED_PIN, HIGH); // turn the led on
  }
}

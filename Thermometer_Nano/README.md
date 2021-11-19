# Arduino Nano Thermometer
* Prints current temperature and humidity on OLED display, plus the 24 hours min/max temperature.
* Couldn't do min/max on humidity due to memory constraint (OLED's library took 1K). Even for the temperature I needed to cut the resolution to half hour buckets to save memory. 
* If the program uses too much memory, the DHT11 library fails to read the sensors (had to keep it under 1.6K RAM for this)

![image](media/thermometer.jpg)

## Bill of Materials
* Arduino Nano (AtMega328P)
* DHT11 (3 pins)
  * 3.3V
  * data to D2 (don't connect to analog)
* 10K resistor
  * connecting DHT11 data pin to power
* 128x32 OLED IIC I2C 0.91 inch
  * 3.3V
  * data to A4, clock to A5

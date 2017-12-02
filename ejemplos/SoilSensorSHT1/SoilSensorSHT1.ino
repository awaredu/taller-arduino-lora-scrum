/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */

#include <SHT1x.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  10
#define clockPin 11
SHT1x sht1x(dataPin, clockPin);

void setup()
{
   SerialUSB.begin(38400); // Open serial connection to report values to host
   SerialUSB.println("Starting up");
}

void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  SerialUSB.print("Temperature: ");
  SerialUSB.print(temp_c, DEC);
  SerialUSB.print("C / ");
  SerialUSB.print(temp_f, DEC);
  SerialUSB.print("F. Humidity: ");
  SerialUSB.print(humidity);
  SerialUSB.println("%");

  delay(2000);
}

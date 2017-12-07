# Código adaptado para placas Seeeduino con GPS integrado

Este código es un ejemplo extraido de la librería [TinyGPS](https://github.com/mikalhart/TinyGPS "Descargar TinyGPS") y adaptado para usarlo en el Seeeduino Lora que usamos en el curso. Los cambios son solo en la nomenclatura de los puertos seriales, que en el seeeduino son [distintos](https://github.com/manglez/taller-arduino-lora-scrum/blob/master/coop/diferencias_arduino_seeeduino.md "Diferencias entre Arduino y Seeeduino").

```
#include <TinyGPS.h>

TinyGPS gps;

void setup()
{
  SerialUSB.begin(115200);
  Serial.begin(9600);
  
  SerialUSB.print("Simple TinyGPS library v. ");
  SerialUSB.println(TinyGPS::library_version());
  SerialUSB.println("by Mikal Hart");
  SerialUSB.println();
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      // SerialUSB.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    SerialUSB.print("LAT=");
    SerialUSB.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SerialUSB.print(" LON=");
    SerialUSB.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    SerialUSB.print(" SAT=");
    SerialUSB.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    SerialUSB.print(" PREC=");
    SerialUSB.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  SerialUSB.print(" CHARS=");
  SerialUSB.print(chars);
  SerialUSB.print(" SENTENCES=");
  SerialUSB.print(sentences);
  SerialUSB.print(" CSUM ERR=");
  SerialUSB.println(failed);
  if (chars == 0)
    SerialUSB.println("** No characters received from GPS: check wiring **");
}
```

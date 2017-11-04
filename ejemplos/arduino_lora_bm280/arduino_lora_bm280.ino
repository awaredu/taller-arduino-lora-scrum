#include <Seeed_BME280.h>
#include <Wire.h>

BME280 bme280;

void setup()
{
  SerialUSB.begin(115200);
  if(!bme280.init()){
    Serial.println("Error al inicial el BME280!");
  }
}

void loop()
{
  float pressure;
  
  //obtener he imprimir temperaturas
  SerialUSB.print("Temp: ");
  SerialUSB.print(bme280.getTemperature());
  SerialUSB.println("C");
  
  //obtener he imprimir presion atmosferica
  SerialUSB.print("Presion atmosferica: ");
  SerialUSB.print(pressure = bme280.getPressure());
  SerialUSB.println("Pa");

  //calcular altitude con la presion atmosferica
  SerialUSB.print("Altitud: ");
  SerialUSB.print(bme280.calcAltitude(pressure));
  SerialUSB.println("m");

  //obtener he imprimir humedad
  SerialUSB.print("Humidity: ");
  SerialUSB.print(bme280.getHumidity());
  SerialUSB.println("%");

  delay(1000); // esperamos un segundo
}


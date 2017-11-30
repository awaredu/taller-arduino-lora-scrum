
#include <LoRaWan.h>
#include <CayenneLPP.h>
#include <Seeed_BME280.h>
#include <Wire.h>


char *appKey = "E5729D852CBA2B2DECB56CAC49853C2B";
char *appEui = "70B3D57ED00085F0";
char *devEui = "0070F3466D0ABF10";

BME280 bme280;
CayenneLPP lpp(51);


void setup(void)
{
    // Iniciar BME280
    pinMode(38, OUTPUT);    // Dar voltaje a módulos grove
    digitalWrite(38, HIGH);
    bme280.init();

    // Iniciar conexión LORA
    lora.init();

    lora.setId(NULL, devEui, appEui);
    lora.setKey(NULL, NULL, appKey);

    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);

    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);

    lora.setReceiceWindowFirst(0);
    lora.setReceiceWindowSecond(869.5, DR3);

    lora.setPower(20);

    while (!lora.setOTAAJoin(JOIN));

    // Iniciar Cayenne
    lpp.reset();
    lpp.addDigitalOutput(7, 1);
    lora.transferPacket(lpp.getBuffer(), lpp.getSize());
}

void loop(void)
{
    bool result = false;
    int pressure;
    float temperature;
    float humidity;

    // Obtener temperatura, presión y humedad
    temperature = bme280.getTemperature();
    pressure = bme280.getPressure();
    humidity = bme280.getHumidity();

    // Preparar payload de Cayenne
    lpp.reset();
    lpp.addTemperature(1, temperature);
    lpp.addBarometricPressure(2, pressure);
    lpp.addRelativeHumidity(3, humidity);

    // Transferir datos a través de LORA
    result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());
}

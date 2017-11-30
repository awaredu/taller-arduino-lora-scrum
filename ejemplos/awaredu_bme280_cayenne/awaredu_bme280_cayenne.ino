
#include <LoRaWan.h>
#include <CayenneLPP.h>
#include <Seeed_BME280.h>
#include <Wire.h>

BME280 bme280;
CayenneLPP lpp(51);

char buffer[256];

void setup(void)
{
    SerialUSB.begin(115200);
    while (!SerialUSB);
    SerialUSB.println("start");

    // iniciar BME280
    pinMode(38, OUTPUT); // dar voltaje a modulos grove
    digitalWrite(38, HIGH);
    if (!bme280.init())
    {
        SerialUSB.println("Error al iniciar el BME280!");
    }

    lora.init();

    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer);

    memset(buffer, 0, 256);
    lora.getId(buffer, 256, 1);
    SerialUSB.print(buffer);
    //setId(char *DevAddr, char *DevEUI, char *AppEUI)
    lora.setId(NULL, "00DED1753860840C", "70B3D57ED0008380");
    //char *NwkSKey, char *AppSKey, char *AppKey
    lora.setKey(NULL, NULL, "45B909512EE2A6C8EED5F7D488FC6D7A");

    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);

    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);

    lora.setReceiceWindowFirst(0);
    lora.setReceiceWindowSecond(869.5, DR3);

    lora.setPower(20);

    while (!lora.setOTAAJoin(JOIN));

    // preparar outputs
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

    //obtener he imprimir temperaturas
    SerialUSB.print("Temp: ");
    SerialUSB.print(temperature = bme280.getTemperature());
    SerialUSB.println("C");

    //obtener he imprimir presion atmosferica
    SerialUSB.print("Presion atmosferica: ");
    SerialUSB.print(pressure = bme280.getPressure());
    SerialUSB.println("Pa");

    //obtener he imprimir humedad
    SerialUSB.print("Humidity: ");
    SerialUSB.print(humidity = bme280.getHumidity());
    SerialUSB.println("%");

    // preparar payload de cayenne
    lpp.reset();
    lpp.addTemperature(1, temperature);
    lpp.addBarometricPressure(2, pressure);
    lpp.addRelativeHumidity(3, humidity);

    result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());

    if (result)
    {
        short length;
        short rssi;

        memset(buffer, 0, 256);
        length = lora.receivePacket(buffer, 256, &rssi);

        if (length)
        {
            SerialUSB.print("Length is: ");
            SerialUSB.println(length);
            SerialUSB.print("RSSI is: ");
            SerialUSB.println(rssi);
            SerialUSB.print("Data is: ");
            for (unsigned char i = 0; i < length; i++)
            {
                SerialUSB.print("0x");
                SerialUSB.print(buffer[i], HEX);
                SerialUSB.print(" ");
            }
            SerialUSB.println();
        }
    }

    short length;
    short rssi;

    //0x7 0x0 0x0 0xFF
    //Cayenne LPP
    // Channel | Type | Value
    // 0x7 = 7 | 0 = DIGITAL INPUT | current status? | new status?
    memset(buffer, 0, 256);
    length = lora.receivePacket(buffer, 256, &rssi);
    if (length)
    {
        SerialUSB.print("Length is: ");
        SerialUSB.println(length);
        SerialUSB.print("RSSI is: ");
        SerialUSB.println(rssi);
        SerialUSB.print("Data is: ");
        for (unsigned char i = 0; i < length; i++)
        {
            SerialUSB.print("0x");
            SerialUSB.print(buffer[i], HEX);
            SerialUSB.print(" ");
        }
        SerialUSB.println();
    }
}


#include <LoRaWan.h>
#include <CayenneLPP.h>
#include <Seeed_BME280.h>
#include <Wire.h>

BME280 bme280;
CayenneLPP lpp(51);

unsigned char data[10] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA,
};
char buffer[256];

void setup(void)
{
    SerialUSB.begin(115200);
    while (!SerialUSB);
    SerialUSB.println("start");
    //while(!SerialUSB);
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
    lora.setId(NULL, "00141131EE0F9A8E", "70B3D57ED0007FCA");
    //char *NwkSKey, char *AppSKey, char *AppKey
    lora.setKey(NULL, NULL, "2283F14DF4D8A3BCEF0FC461A35DED4D");

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

    //obtener he imprimir temperaturas
    SerialUSB.print("Temp: ");
    SerialUSB.print(temperature = bme280.getTemperature());
    SerialUSB.println("C");

    //obtener he imprimir presion atmosferica
    SerialUSB.print("Presion atmosferica: ");
    SerialUSB.print(pressure = bme280.getPressure());
    SerialUSB.println("Pa");

    lpp.reset();
    lpp.addTemperature(1, temperature);
    lpp.addBarometricPressure(2, pressure);

    result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());
    //result = lora.transferPacket(data, 10, 10);

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

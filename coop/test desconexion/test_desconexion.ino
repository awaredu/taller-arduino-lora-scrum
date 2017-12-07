#include <LoRaWan.h>
#include <CayenneLPP.h>

CayenneLPP lpp(51);
char buffer[256];

void setup(void)
{
    SerialUSB.begin(115200);
    while(!SerialUSB);
    
    lora.init();
    
    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer); 
    
    memset(buffer, 0, 256);
    lora.getId(buffer, 256, 1);
    SerialUSB.print(buffer);
    
    //setId(char *DevAddr, char *DevEUI, char *AppEUI)
    lora.setId(NULL, "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX");
    //char *NwkSKey, char *AppSKey, char *AppKey
    lora.setKey(NULL, NULL, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);
    
    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);
    
    lora.setReceiceWindowFirst(0, 868.1);
    lora.setReceiceWindowSecond(869.5, DR3);
    
    lora.setPower(20);
}

void loop(void)
{   
    bool result = false;
    bool connect_flag = false;
    short length;
    short rssi;
    
    while(!lora.setOTAAJoin(JOIN,10))
    {
        SerialUSB.println("trying to connect, please wait...");
        delay(3000);
    }

    SerialUSB.println("connected!");
    connect_flag = true;
    
    while(connect_flag)
    {
        lpp.reset();
        lpp.addAnalogInput(1, millis()/1000);

        result = lora.transferPacketWithConfirmed(lpp.getBuffer(), lpp.getSize(),10);
        if(result)
        {
            memset(buffer, 0, 256);
            length = lora.receivePacket(buffer, 256, &rssi);
            SerialUSB.print("RSSI: ");
            SerialUSB.println(rssi);
            SerialUSB.println();
        }
        else
        {
            SerialUSB.println("disconnected from Lora, trying to reconnect");
            connect_flag = false;
        }        
        delay(1000);
    }
}


#include <LoRaWan.h>


unsigned char data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA,};
char buffer[256];


void setup(void)
{
  SerialUSB.begin(115200);
    //while(!SerialUSB);
    
    lora.init();
    
    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer); 
    
    memset(buffer, 0, 256);
    lora.getId(buffer, 256, 1);
    SerialUSB.print(buffer);
    //setId(char *DevAddr, char *DevEUI, char *AppEUI)
    //lora.setId("26012693", "0059F5F67E05AD07", "70B3D57ED0007FD1");
    lora.setId("", "0059F5F67E05AD07", "70B3D57ED0007FD1");
    //char *NwkSKey, char *AppSKey, char *AppKey
    //lora.setKey("890131B3DD351C8F5BEA5F08FDAED5C8", "4829B5CD607D92338DD7846DF95A60E3", "5B77ED7337FF6AC19280C0C80D7C525C");
    lora.setKey("", "", "5B77ED7337FF6AC19280C0C80D7C525C");
    
    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);
    
    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);
    
    lora.setReceiceWindowFirst(0);
    lora.setReceiceWindowSecond(869.5, DR3);
    
    lora.setPower(20);
    
    while(!lora.setOTAAJoin(JOIN));
}

void loop(void)
{   
    bool result = false;
    
    result = lora.transferPacket("Hello World!", 10);
    //result = lora.transferPacket(data, 10, 10);
    
    if(result)
    {
        short length;
        short rssi;
        
        memset(buffer, 0, 256);
        length = lora.receivePacket(buffer, 256, &rssi);
        
        if(length)
        {
            SerialUSB.print("Length is: ");
            SerialUSB.println(length);
            SerialUSB.print("RSSI is: ");
            SerialUSB.println(rssi);
            SerialUSB.print("Data is: ");
            for(unsigned char i = 0; i < length; i ++)
            {
                SerialUSB.print("0x");
                SerialUSB.print(buffer[i], HEX);
                SerialUSB.print(" ");
            }
            SerialUSB.println();
        }
    }
}



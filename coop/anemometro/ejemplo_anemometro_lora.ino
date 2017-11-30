/*
 * Weather Station demo code
 * 
 * Sensors included: wind speed, wind direction
 * 
 * This program measures every sensor once per second and calculates 
 * average values every 10 seconds before sending data over the lora
 * network
 */


// libraries
#include <LoRaWan.h>
#include <CayenneLPP.h>

CayenneLPP lpp(51);


// Hardware pin definitions
const byte WINDSPEED_PIN = 3;
const byte WINDDIR_PIN = A0;

//Global Variables
volatile long lastWindIRQ = 0;
volatile byte windClicks  = 0;
long lastWindCheck = 0;
long lastSecond;
byte sample_index;

//weather averaged values
int   winddir_10;     // [0-360 degrees wind direction]
float windspeed_10;   // [km/h instantaneous wind speed]

// weather accumulated values (for average calculation)
int   winddir_sum[10];
float windspeed_sum;

char buffer[256];



//Interrupt routines
void windspeedIRQ()
// Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3
{
    if (millis() - lastWindIRQ > 10) // Ignore bounce glitches (240km/h max reading)
    {
        lastWindIRQ = millis();
        windClicks++;
    }
}


void setup()
{
    SerialUSB.begin(115200);
    while (!SerialUSB);
    SerialUSB.println("Weather Station Demo");

    pinMode(WINDSPEED_PIN, INPUT);

    lora.init();
    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer);

    //setId(char *DevAddr, char *DevEUI, char *AppEUI)
    lora.setId(NULL, "00C2527A621F4049", "70B3D57ED00083F3");
    //char *NwkSKey, char *AppSKey, char *AppKey
    lora.setKey(NULL, NULL, "339C4E7107DF04079CE19F255B822D9B");

    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);

    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);
    
    //lora.setReceiceWindowFirst(0, 868.1);
    lora.setReceiceWindowFirst(0);
    lora.setReceiceWindowSecond(869.5, DR3);

    lora.setPower(20);

    while (!lora.setOTAAJoin(JOIN));

    // cayenne configuration
    lpp.reset();
    lpp.addDigitalOutput(7, 1);
    lora.transferPacket(lpp.getBuffer(), lpp.getSize());


    windspeed_sum   = 0.0;

    lastSecond = millis();
    sample_index = 0;
        
    // attach external interrupt pins to IRQ functions
    attachInterrupt(3, windspeedIRQ, FALLING);

    // turn on interrupts
    interrupts();

    SerialUSB.println("Weather Station online!");
}


void loop()
{
  bool result = false;
  int winddir;
  float windspeed;
  
  // trigger the measurements once per second
  if(millis() - lastSecond >= 1000)
    {
    lastSecond += 1000;

    // instant values
    winddir   = get_wind_direction();
    windspeed = get_wind_speed();
    
    SerialUSB.print("winddir=");
    SerialUSB.print(winddir);
    SerialUSB.print(", windspeed=");
    SerialUSB.print(windspeed, 1);
    SerialUSB.println();
        
    // accumulated values
    winddir_sum[sample_index] = winddir;
    windspeed_sum += windspeed;

    sample_index++;
    if(sample_index >= 10)
    {
      winddir_10   = winddir_avg(winddir_sum,10);
      windspeed_10 = windspeed_sum / 10.0;

      windspeed_sum = 0;

      sample_index = 0;
      
      //Report all readings every 10 seconds
      SerialUSB.print("Average: ");
      SerialUSB.print("winddir=");
      SerialUSB.print(winddir_10);
      SerialUSB.print(", windspeed=");
      SerialUSB.print(windspeed_10, 1);
      SerialUSB.println();

      //setup cayenne payload
      lpp.reset();
      lpp.addTemperature(1, windspeed_10);
      lpp.addBarometricPressure(2, winddir_10);

      result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());
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
      if(length)
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
  }
  delay(50);
}


//Returns the instataneous wind speed
float get_wind_speed()
{
    float deltaTime = (millis() - lastWindCheck) / 1000.0;
    float wspeed = (float)windClicks / deltaTime;
    wspeed *= 1.2; //1 turn/sec = 2 click/sec = 2.4km/h

    windClicks = 0;
    lastWindCheck = millis();

    return(wspeed);
}


//Read the wind direction sensor, return heading in degrees
int get_wind_direction()
{
    unsigned int adc = analogRead(WINDDIR_PIN); // get the current reading from the sensor
    
    if (adc < 92) return (113);
    if (adc < 105) return (68);
    if (adc < 125) return (90);
    if (adc < 170) return (158);
    if (adc < 230) return (135);
    if (adc < 280) return (203);
    if (adc < 350) return (180);
    if (adc < 440) return (23);
    if (adc < 540) return (45);
    if (adc < 622) return (248);
    if (adc < 675) return (225);
    if (adc < 750) return (338);
    if (adc < 815) return (0);
    if (adc < 865) return (293);
    if (adc < 923) return (315);
    if (adc < 985) return (270);
    return (-1); // error, disconnected?
}


//Calculate wind direction average using the Mitsuta method
//because it doesn't require trig functions
int winddir_avg(int* directions, int nsamples)
{
  long sum = directions[0];
  int D = directions[0];
  for(int i=1 ; i<nsamples ; i++)
  {
    int delta = directions[i] - D;
    if(delta < -180)
      D += delta + 360;
    else if(delta > 180)
      D += delta - 360;
    else
      D += delta;

    sum += D;
  }
  
  float diravg = sum / (float)nsamples;
  if(diravg >= 360)
    diravg -= 360;
  if(diravg < 0)
    diravg += 360;

  return(diravg);
}



void printWeather_10()
{
    SerialUSB.print("winddir=");
    SerialUSB.print(winddir_10);
    SerialUSB.print(", windspeed=");
    SerialUSB.print(windspeed_10, 1);
    SerialUSB.println();
}



void sendWeather()
{
}


/*
 * Weather Station demo code
 * 
 * Sensors included: wind speed, wind direction
 * 
 * This program measures each sensor once per second
 */



// Hardware pin definitions
const byte WINDSPEED_PIN = 3;
const byte WINDDIR_PIN = A0;

//Global Variables
volatile long lastWindIRQ = 0;
volatile byte windClicks  = 0;

long lastWindCheck = 0;
long lastSecond;



void setup()
{
    SerialUSB.begin(115200);
    while (!SerialUSB);
    SerialUSB.println("Weather Station Demo");

    pinMode(WINDSPEED_PIN, INPUT);

    lastSecond = millis();
    
    // attach external interrupt pins to IRQ functions
    attachInterrupt(3, windspeedIRQ, FALLING);

    // turn on interrupts
    interrupts();

    SerialUSB.println("Weather Station online!");
}



void loop()
{
  // trigger the measurements once per second
  if(millis() - lastSecond >= 1000)
    {
    lastSecond += 1000;

    // instant values
    int   winddir   = get_wind_direction();
    float windspeed = get_wind_speed();
    
    SerialUSB.print("winddir=");
    SerialUSB.print(winddir);
    SerialUSB.print(", windspeed=");
    SerialUSB.print(windspeed, 1);
    SerialUSB.println();
    }
  delay(50);
}



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



// return speed in km/h
float get_wind_speed()
{
    float deltaTime = (millis() - lastWindCheck) / 1000.0;
    float wspeed = (float)windClicks / deltaTime;
    wspeed *= 1.2; //1 turn/sec = 2 click/sec = 2.4km/h

    windClicks = 0;
    lastWindCheck = millis();

    return(wspeed);
}



// return heading in degrees
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

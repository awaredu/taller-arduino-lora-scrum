#include <CayenneLPP.h>
#include <LoRaWan.h>
#include <Seeed_BME280.h>

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

BME280 bme280;
CayenneLPP lpp(60);

unsigned long inicio = 0;
int interval = 2000;
char *devEui = "007806192C9937B7";
char *appEui = "70B3D57ED0008661";
char *appKey = "5FD6FC39D0AC399E86DC3AD7842B0F56";
bool led = false;

void setup()
{

  lpp.reset();
  SerialUSB.begin(115200);
  //Inicializa LoRaWan
  lora.init();
  lora.setId(NULL, devEui, appEui);
  lora.setKey(NULL, NULL, appKey);

  lora.setDeciveMode(LWOTAA);
  lora.setDataRate(DR0, EU868);

  lora.setAdaptiveDataRate(true);

  lora.setChannel(0, 868.1);
  lora.setChannel(1, 868.3);
  lora.setChannel(2, 868.5);
  lora.setChannel(3, 867.1);
  lora.setChannel(4, 867.3);
  lora.setChannel(5, 867.5);
  lora.setChannel(6, 867.7);

  lora.setReceiceWindowFirst(0);
  lora.setReceiceWindowSecond(869.5, DR3);
  lora.setPower(20);

  lora.setOTAAJoin(JOIN);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
  if (!bme280.init())
  {
    //print "Error";
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  float presion;
  float humedad;
  float temperatura;
  float altitud;
  float rocio;
  unsigned long ahora;
  bool result;
  short length, rssi;
  char buffer[256];

  ahora = millis();
  //Cada cierto tiempo (interval) activa la lectura
  //de los sensores y los envia a la red Lora
  if ((ahora - inicio) > interval)
  {
    inicio = ahora;
    temperatura = bme280.getTemperature();
    presion = bme280.getPressure();
    humedad = bme280.getHumidity();
    altitud = bme280.calcAltitude(presion);
    rocio = calculaRocio(presion, temperatura, humedad);

    lpp.reset();
    lpp.addTemperature(1, temperatura);
    lpp.addRelativeHumidity(2, humedad);
    lpp.addBarometricPressure(3, presion);
    lpp.addGPS(4, 28.681210, -17.765681, altitud);
    lpp.addAnalogOutput(5, rocio);
    lpp.addAnalogOutput(6, calculaRocio1(presion, temperatura, humedad));
    result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());

    //Only to test we try to receive a json object
    //to switch on and off a led on the Arduino board
    length = lora.receivePacket(buffer, 256, &rssi);
    if (length)
    {
      //for (unsigned int i=0; i<length;i++){
      //  SerialUSB.println(buffer[i]);
      led = !led;
      digitalWrite(13, led);
    }
  }
}

float calculaRocio(float presion, float temperatura, float humedad)
{
  float rocio;
  float t1, t2, t3;

  t1 = log(100 / humedad);
  t2 = 15 * t1 - 2.1 * temperatura + 2711.5;
  t3 = temperatura + 273.16;

  rocio = (t3 * t2) / ((t3 * t1) / 2 + t2) - 273.16;

  return rocio;
}

float calculaRocio1(float presion, float temp, float hum)
{
  float t1, t2, t3;
  float rocio;

  t1 = log(hum / 100);
  t2 = (17.67 * temp) / (243.5 + temp);
  rocio = (243.5 * (t1 + t2)) / (17.67 - (t1 + t2));
  return rocio;
}

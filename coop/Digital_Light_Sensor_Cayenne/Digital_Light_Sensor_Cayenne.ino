/*
 * Digital_Light_Sensor.ino
 * A library for TSL2561
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : zhangkun
 * Create Time:
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#include <LoRaWan.h>
#include <CayenneLPP.h>

CayenneLPP lpp(51);
char buffer[256];

char *appKey="8EA85224D106A3C00066EDA1551B41BD"
char *appEui="70B3D57ED0008848"
char *devEui="00B5CCF5FE85E09E"


void setup()
{
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("start");

  pinMode(38, OUTPUT); // dar voltaje a modulos grove
  digitalWrite(38, HIGH);
  Wire.begin();
  
  TSL2561.init();
  lora.init();

  memset(buffer, 0, 256);
  lora.getVersion(buffer, 256, 1);
  SerialUSB.print(buffer);

  memset(buffer, 0, 256);
  lora.getId(buffer, 256, 1);
  SerialUSB.print(buffer);

  lora.setId(NULL, devEui, appEui);
  lora.setKey(NULL, Null, appKey);

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

void loop()
{
  int lux= TSL2561.readVisibleLux();
  
  SerialUSB.print("The Light value is: ");
  SerialUSB.println(lux);
  
  lpp.reset();
  lpp.addLuminosity(1, lux);
  result = lora.transferPacket(lpp.getBuffer(), lpp.getSize());
  delay(1000);

}



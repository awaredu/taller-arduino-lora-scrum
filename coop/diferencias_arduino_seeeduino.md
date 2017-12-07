# DIFERENCIAS ENTRE ARDUINO Y SEEEDUINO

Mientras intentaba conectar el anemometro al seeeduino encontre algunos problemas debidos a que los arduinos y los seeeduinos no son exactamente iguales. Este documento contiene las diferencias que he encontrado por si alguien quiere comprobar la compatibilidad de sus shields de arduino con la placa seeeduino.

## Hardware

|                       |     Arduino         |     Seeeduino Lora     |
|-----------------------|---------------------|------------------------|
|microcontrolador       |ATMEGA (8bit) a 16MHz|ATSAMD21 (32bit) a 48MHz|
|voltaje de la logica   | 5V                  | 3.3V                   |
|I/O digitales          | 20 (6 PWM)          | 20 (18 PWM)            |
|Entradas analogicas    | 6 (10bit)           | 6 (12bit)              |
|Salidas analogicas     | 0                   | 1 (10bit)              |
|interrupciones hardware| 2                   | 19                     |
|UART (puerto serie)    | 1                   | 2                      |
|Memoria flash          | 32kB                | 256kB                  |
|SRAM                   | 2kB                 | 32kB                   |
|EEPROM                 | 1kB                 | 0kB                    |
|uso pin A4             | SDA (I2C interface) | voltaje de la bateria  |
|uso pin A5             | SCL (I2C interface) | estado de la batería   |

En el Seeeduino el pin `5V` solo tiene voltaje cuando está conectado el cable USB, que además hace que se cargue la bateria si tenemos una puesta.

## Software

A nivel de software el Seeeduino es compatible con la mayoría de librerías de arduino, la diferencia principal que he encontrado es que el arduino tiene 1 puerto serie mientras que el seeeduino tiene 2+1, y eso hace que los ejemplos de arduino que usen el puerto serie, para imprimir datos en la consola por ejemplo, haya que modificarlos para usarlos en el seeeduino.

En arduino, al haber solo un puerto serie, todas funciones relacionadas se llaman `Serial.*`, por ejemplo `Serial.begin()` o `Serial.println()`. En el seeeduino tenemos dos puertos series por hardware que se llaman `Serial` (conectado al GPS si la placa lo lleva) y `Serial1` (conectado al módulo LoRa). El Seeeduino tiene un puerto para la consola aparte, que se llama `SerialUSB`. Por tanto, para usar un programa de ejemplo de arduino que escriba en la consola, en el seeeduino hay que cambiar todas las funciones `Serial.*()` por `SerialUSB.*()`

   ```
   Arduino                     --->  Seeeduino
   --------------------------------------------------------------
   Serial.begin(115200);       --->  SerialUSB.begin(115200);
   Serial.print(temperatura);  --->  SerialUSB.print(temperatura);
   ```

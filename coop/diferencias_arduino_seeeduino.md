# DIFERENCIAS ENTRE ARDUINO Y SEEEDUINO

Mientras intentaba conectar el anemometro al seeeduino encontre algunos problemas debidos a que los arduinos y los seeeduinos no son exactamente iguales. Este documento contiene las diferencias que he encontrado a nivel de electronica por si alguien quiere comprobar la compatibilidad de sus shields de arduino con la placa seeeduino.

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

En el Seeeduino el pin `5V` solo tiene voltaje cuando está conectado el cable USB, que además hace que se cargue la bateria si tenemos una conectada.

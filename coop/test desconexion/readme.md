# Test de desconexión y reconexión a la red Lora

Este es un programa de ejemplo de como se puede detectar la desconexión de la red Lora para dejar de transmitir datos e intentar la reconexión.

El esquema del programa es:

1. Espera a que el usuario abra la consola de arduino

   (ya sabemos que esta linea se puede comentar para usar el seeeduino sin conexión al ordenador)
   
2. Inicia y configura la red Lora (como hemos visto en otros programas de ejemplo)

3. Intenta conectarse a la red Lora, si no se conecta sigue intentándolo hasta que lo consigue

   `while(!lora.setOTAAJoin(JOIN))` ---> `while(!lora.setOTAAJoin(JOIN,10))`

   Aquí he añadido un timeout de 10 segundos (por defecto son 5 segundos). Por cómo está programada la librería a veces decía que se había conectado aunque en realidad no lo había hecho, aumentando el timeout mejora el comportamiento

4. Envía el paquete Cayenne (tiempo transcurrido desde que se activo el Seeeduino) y espera a que el servidor/gateway le devuelva el ACK
   * Si lo consigue espera un segundo y vuelve a enviar otro dato
   * Si no lo consigue vuelve al paso 3 para intentar reconectarse
   
   He cambiado `result = lora.transferPacket(lpp.getBuffer(), lpp.getSize())`
   
   por `result = lora.transferPacketWithConfirmed(lpp.getBuffer(), lpp.getSize(),10)`
   
   La segunda función espera a recibir un ACK del servidor/Gateway Lora, si no llega sabremos que se ha perdido la conexión. Además he cambiado el timeout a 10 segundos porque cuando la señal es débil el ACK tardaba más de 5 segundos en llegar.

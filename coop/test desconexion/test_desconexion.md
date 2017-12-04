# Test de desconexión y reconexión a la red Lora

Este es un programa de ejemplo de como se puede detectar la desconexión de la red Lora para dejar de transmitir datos e intentar la reconexión.

El esquema del programa es:

1. Espera a que el usuario abra la consola de arduino

   (ya sabemos que esta linea se puede comentar para usar el seeeduino sin conexión al ordenador)
   
2. Inicia y configura la red Lora (como hemos visto en otros programas de ejemplo)

3. Intenta conectarse a la red Lora, si no se conecta sigue intentándolo hasta que lo consigue

4. Envía el paquete Cayenne (tiempo transcurrido desde que se activo el Seeeduino) y espera a que el servidor/gateway le devuelva el ACK
   * Si lo consigue espera un segundo y vuelve a enviar otro dato
   * Si no lo consigue vuelve al paso 3 para intentar reconectarse

    ######################
    TRABAJO ORIGINAL DE:
    ######################
   
   @copyright   Samco, https://github.com/samuelballantyne, junio de 2020
   
   @copyright   Licencia Pública General Reducida de GNU
   
   @author [Sam Ballantyne](samuelballantyne@hotmail.com)
   
   @version  V1.0
   
   @date  2020
   
   Licencia Pública General de GNU v3.0
   
   https://github.com/samuelballantyne/IR-Light-Gun/blob/master/LICENSE
   
  ———————————————————————————————————————————————————————————————————————
   ######################
   
   # MODIFICADO Por Makoto : makotoworkshop.org
   
   revision + arduino portable + app de testeo 4ir + traduccion por www.mundoyakara.com
   
   ######################
   

   Funciona con una configuración de 4 fuentes de luz infrarroja (2 arriba y 2 abajo de la pantalla)
   
   Cargar en un Arduino Pro Micro (32u4 - 3.3 8.000Mhz)
   
   
   Ok - Reemplazo del teclado HID por un Joystick HID (Stick XY y botones (Inicio, Recarga, Gatillo))
   
   Ok - Modo de recarga automática si el mouse sale de la pantalla.
   
   Ok - Calibración/pausa activada presionando los 3 botones simultáneamente (Inicio, Recarga, Gatillo)
   
   Ok - Interruptor de selección para desactivar el modo de recarga automática.
   
   Ok - Modo Ratón - Modo Joystick - Modo Híbrido (Solo movimiento de ratón) y (botón de ratón + botón de joystick) (para supermodel2)
   
   Ok - Interruptor de selección para desactivar el Ratón.
   
   Ok - Interruptor de selección para desactivar el Joystick.
   
   Ok - Interruptor de selección para activar el modo híbrido.
   
   Ok - Botón de selección del modo de disparo automático (para 1 presión del gatillo = 1 disparo, 2 disparos, 3 disparos, 4 disparos, 5 disparos, 6 disparos, Ametralladora)
   
   Ok - Gestión de LED para el modo de disparo automático (Rojo, Verde, Azul, Amarillo, Violeta, Cian, Blanco, fundido de colores)
   
           Modo de disparo: Led 4 colores  2,3v (rojo) o 3,4v (verde y azul). 20mA.
           
          - 1 disparo  -> 1 activación del Solenoide -> Amarillo
          
          - 2 disparos -> 2 activaciones -> Verde
          
          - 3 disparos -> 3 activaciones -> Azul
          
          - 4 disparos -> 4 activaciones -> Blanco
          
          - 5 disparos -> 5 activaciones -> Cian
          
          - 6 disparos -> 6 activaciones -> Violeta
          
          - Ametralladora -> activaciones automáticas -> Fundido de colores
          
          - 1 disparo (mantenido) -> activaciones automáticas -> Rojo
          
   Ok - Gestión del LED de Fuego (led rojo que parpadea en cada disparo)
   
   Ok - Soporte del modo automático para el Ratón.
   
   Ok - Soporte del modo automático para el Joystick.
   
   Ok - Hacer parpadear el led en disparo estándar mantenido.
   
   Ok - Si recarga manual, el botón de recarga, pues recarga (botón de joystick y clic de ratón)
   
   - Solucionar bug: si se activa la recarga y el disparo al mismo tiempo, al pasar al modo de disparo automático "Ametralladora", la pistola comienza a disparar sin parar
      
          CÓMO CALIBRAR:
     
            Paso 1: Presiona los botones START + RECARGAR

            Paso 2: Pulsa el gatillo

            Paso 3: Dispara al centro de la pantalla (intenta hacerlo lo más preciso posible)

            Paso 4: El ratón debería bloquearse en el eje vertical, (usa los botones de Start y Recarga para ajustar el ratón arriba/abajo)

            Paso 5: Pulsa el gatillo

            Paso 6: El ratón debería bloquearse en el eje horizontal, (usa los botones de Inicio y Recarga para ajustar el ratón izquierda/derecha)

            Paso 7: Pulsa el gatillo para finalizar

            Paso 8: Los ajustes se guardan ahora en la EEPROM


Aquí tienes la traducción al español de tu texto:

"Proyecto derivado de https://github.com/samuelballantyne/IR-Light-Gun/tree/master/Samco_4IR_Beta/Samco_2.0_4IR_32u4_BETA

Todo este proyecto se describe aquí:

https://burogu.makotoworkshop.org/index.php?post/2023/01/31/irgun_arduino
https://burogu.makotoworkshop.org/index.php?post/2023/03/02/irgun_arduino2
https://burogu.makotoworkshop.org/index.php?post/2023/03/04/irgun_arduino3
Video de demostración aquí:

https://peertube.makotoworkshop.org/w/bESF7cy5ayA5ZS5VjgZZ54
Características:

Calibración rápida
Pausa/recalibración
Corrección de inclinación
Este proyecto utiliza bibliotecas de DFRobot y Jonathan Edgecombe. Las bibliotecas originales se pueden encontrar en:

https://github.com/DFRobot/DFRobotIRPosition
https://github.com/jonathanedgecombe/absmouse"
/* ######################
   # TRABAJO ORIGINAL DE:
   ######################
   @file Samco_2.0_4IR_32u4_BETA.ino
   
   @brief Esquema del Light Gun de 10 botones para configuración de 4 LED
   
   @n Archivo INO para la configuración de Samco Light Gun con 4 LED
   
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
   
   rev y traduccion por www.mundoyakara.com
   
   ######################
   
   LightGunIR_4IR_Makoto_V2.3.ino - 12/2022
   
   Funciona con una configuración de 4 fuentes de luz infrarroja (2 arriba y 2 abajo de la pantalla)
   
   Cargar en un Arduino Pro Micro (32u4 - Leonardo)
   
   
   Ok - Reemplazo del teclado HID por un Joystick HID (Stick XY y botones (Inicio, Recarga, Gatillo))
   
   Ok - Modo de recarga automática si el mouse sale de la pantalla.
   
   Ok - Calibración/pausa activada presionando los 3 botones simultáneamente (Inicio, Recarga, Gatillo)
   
   Ok - Interruptor de selección para desactivar el modo de recarga automática.
   
   Ok - Modo Ratón - Modo Joystick - Modo Híbrido (Solo movimiento de ratón) y (botón de ratón + botón de joystick) (para supermodel2)
   
   Ok - Interruptor de selección para desactivar el Ratón.
   
   Ok - Interruptor de selección para desactivar el Joystick.
   
   Ok - Interruptor de selección para activar el modo híbrido.
   
   Ok - Gestión del Solenoide, que se activa en cada disparo.  https://arduinodiy.wordpress.com/2012/05/02/using-mosfets-with-ttl-levels/
   
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
   
   Ok - Gestión de 3 LED indicadores para indicar la activación/desactivación de los modos: Led 1+2 Híbrido, Led 1 Solo Ratón, Led 2 Solo Joystick, Led 3 Recarga)
   
   Ok - Soporte del modo automático para el Ratón.
   
   Ok - Soporte del modo automático para el Joystick.
   
   Ok - Modo de depuración automática cuando el Híbrido está activado.
   
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


Forked project from https://github.com/samuelballantyne/IR-Light-Gun/tree/master/Samco_4IR_Beta/Samco_2.0_4IR_32u4_BETA

This entire project is describe here : 
- https://burogu.makotoworkshop.org/index.php?post/2023/01/31/irgun_arduino
- https://burogu.makotoworkshop.org/index.php?post/2023/03/02/irgun_arduino2
- https://burogu.makotoworkshop.org/index.php?post/2023/03/04/irgun_arduino3

Video Démo here :
- https://peertube.makotoworkshop.org/w/bESF7cy5ayA5ZS5VjgZZ54

Quick description : 
   - Work with DF Robot IR positioning camera, or WiiMote IR Cam Hack
   - Work with 4 LEDs IR sources setup (2 up and 2 down to the screen)
  -  Upload to an Arduino Pro Micro (32u4 - Leonardo) this file : LightGunIR_4IR_Makoto_V2.3.ino

Features :  
- Quick calibration  
- Pause/recaibration  
- Tilt correction  

This project used libraries from DFRobot & Jonathan Edgecome. Original Libraries can be found at :
- https://github.com/DFRobot/DFRobotIRPosition
- https://github.com/jonathanedgecombe/absmouse

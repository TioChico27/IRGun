/* ######################
@copyright Samco, https://github.com/samuelballantyne, junio de 2020
@copyright Licencia Pública General Reducida de GNU
@author Sam Ballantyne
Licencia Pública General de GNU v3.0
https://github.com/samuelballantyne/IR-Light-Gun/blob/master/LICENSE

——————————————————————————————————————————————————————————————————————— ######################
MODIFICADO Por Makoto : makotoworkshop.org
rev y traduccion por www.mundoyakara.com

#####################################

REVISADO EM 13/05/2024 por TioChico27
- Ao ler a calibração da EEPROM era adicionado um valor incorreto a soma dos valores (512 ao invés de 256 usado na gravação);
- Não recarregava a arma no emulador M2 a não ser no modo automático pois o pulso era muito curto, agora o sinal de recarga se mantém enquanto o botão de recarga ficar acionado. 
	E a princípio com isso se resolveu o bug de acionar o modo metralhadora automaticamente quando se apertava gatilho e recarga ao mesmo tempo no modo de recarga automática;
- No modo disparo contínuo o led de disparo ficava piscando da mesma forma que no modo metralhadora e acabava por confundir qual modo estava configurado. Agora o led fica aceso 
	demonstrando o disparo contínuo;
- Altera o modo de disparo inicial para o modo mantido, seguido de 1, 2, 3, 4, 5, 6 disparos e metralhadora;
- Desativação dos LEDs de modo mouse, joystick, híbrido, recarga automática, RGB e do código responsável pelo acionamento do solenóide;

######################

LightGunIR_4IR_Makoto_V2.3.ino - 12/2022

Funciona con una configuración de 4 fuentes de luz infrarroja (2 arriba y 2 abajo de la pantalla)
Cargar en un Arduino Pro Micro (32u4 3.3 8mhz - usando la version portable)
Ok - Reemplazo del teclado HID por un Joystick HID (Stick XY y botones (Inicio, Recarga, Gatillo))
Ok - Modo de recarga automática si el mouse sale de la pantalla.
Ok - Calibración/pausa activada presionando los 3 botones simultáneamente (Inicio, Recarga, Gatillo)
Ok - Interruptor de selección para desactivar el modo de recarga automática.
Ok - Modo Ratón - Modo Joystick - Modo Híbrido (Solo movimiento de ratón) y (botón de ratón + botón de joystick) (para supermodel2)
Ok - Interruptor de selección para desactivar el Ratón.
Ok - Interruptor de selección para desactivar el Joystick.
Ok - Interruptor de selección para activar el modo híbrido.
Ok - Gestión del Solenoide, que se activa en cada disparo. https://arduinodiy.wordpress.com/2012/05/02/using-mosfets-with-ttl-levels/
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

Solucionar bug: si se activa la recarga y el disparo al mismo tiempo, al pasar al modo de disparo automático "Ametralladora", la pistola comienza a disparar sin parar

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


*/


#include <HID.h>
#include <Wire.h>
#include <Joystick.h>   // Biblioteca Joystick de Matthew Heironimus V2.0.7
#include <AbsMouse.h>
#include <DFRobotIRPosition.h>
#include <SamcoBeta.h>
#include <EEPROM.h>

int xCenter = 512;    // Abre el monitor serial y actualiza estos valores para calibrar manualmente
int yCenter = 450;
float xOffset = 147;
float yOffset = 82;
// CALIBRACIÓN: Centro de la cámara x/y: 421, 356     Desplazamientos x/y: 121.00, 71.00

// Definición del Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
                   3, 0,                  // Cantidad de botones, cantidad de interruptores tipo "hat"
                   true, true, false,     // Ejes X e Y, pero sin eje Z
                   false, false, false,   // Sin Rx, Ry o Rz
                   false, false,          // Sin timón o acelerador
                   false, false, false);  // Sin acelerador, freno o dirección
const int pinToButtonMap = 14;            // Constante que asigna el pin físico a un botón del joystick (pines 14-15-16)
int LastButtonState[3] = {0, 0, 0};       // Último estado de los 3 botones

// Variables para la Cámara IR
int condition = 1;
int finalX;                 // Valores después de corrección de inclinación
int finalY;
int see0;   // Leds IR N°1 vistos por la cámara (de lo contrario = 0)
int see1;   // Leds IR N°2 vistos por la cámara (de lo contrario = 0)
int see2;   // Leds IR N°3 vistos por la cámara (de lo contrario = 0)
int see3;   // Leds IR N°4 vistos por la cámara (de lo contrario = 0)

// Variables para el Mouse
int MoveXAxis;              // Posición del mouse sin restricciones
int MoveYAxis;
int conMoveXAxis;           // Posición del mouse restringida
int conMoveYAxis;
int count = -2;             // Establecer un conteo inicial

// Declaración de las Entradas
#define _tiggerPin 14             // Etiqueta Pin para botones del Joystick
#define _startPin 15               // También botón de calibración -
#define _reloadPin  16             // También botón de calibración +
#define SwitchAutoReload  4       // Modo de recarga automática Encendido/Apagado
#define SwitchSuspendMouse  7     // (Activo por defecto si no está conectado) Desactiva el Mouse (HID está presente, pero los datos ya no se envían al mouse)
#define SwitchSuspendJoystick 8  // (Activo por defecto si no está conectado) Desactiva el joystick

// Declaración de las Salidas
#define LedFire A0
//#define LedSuspendMouse A1
//#define LedSuspendJoystick  A2
//#define LedAutoReload A3
//#define LedRouge  9
//#define LedVerte  6
//#define LedBleu 5
//#define Solenoid 10
#define CadenceTir 90
#define ToggleLED(x) digitalWrite(x, !digitalRead(x))

// Variables diversas
unsigned int NBdeTirs = 0;
unsigned int fire = 0;
//const int R = 1;
//const int V = 1;
//const int B = 1;
//unsigned int varR = 255;
//unsigned int varV = 0;
//unsigned int varB = 0;
//unsigned int SequenceRouge = 1;
//unsigned int SequenceJaune = 0;
//unsigned int SequenceVert = 0;
//unsigned int SequenceCyan = 0;
//unsigned int SequenceBleu = 0;
//unsigned int SequenceViolet = 0;
unsigned int Automatic = 1;
unsigned int rafaleCount = 0;

// Estado de los botones
unsigned int SuspendMouse = 0;
unsigned int SuspendJoystick = 0;
unsigned int AutoReload = 0;
unsigned int LastAutoReload = 0;
int ButtonPlus = 0;
int LastButtonPlus = 0;
int ButtonMinus = 0;
int LastButtonMinus = 0;
int ButtonValid = 0;
int LastButtonValid = 0;
int ButtonState_calibration = 0;
int LastButtonState_calibration = 0;
int LastButtonState_tiggerPin = 0;
int LastButtonState_reloadPin = 0;
int LastButtonState_startPin = 0;

int plus = 0;
int minus = 0;

// Declaración Cámara IR
DFRobotIRPosition myDFRobotIRPosition;
SamcoBeta mySamco;
int res_x = 1024;              // ACTUALIZACIÓN: Estos valores no necesitan cambiar
int res_y = 768;               // ACTUALIZACIÓN: Estos valores no necesitan cambiar

void setup() {

 myDFRobotIRPosition.begin();               // Iniciar la cámara IR

Serial.begin(9600);                        // Para guardar la calibración (asegúrate de que tu monitor serial tenga la misma velocidad de baudios)

loadSettings();

AbsMouse.init(res_x, res_y);

// Interruptores
pinMode(SwitchAutoReload, INPUT_PULLUP);
pinMode(SwitchSuspendMouse, INPUT_PULLUP);
pinMode(SwitchSuspendJoystick, INPUT_PULLUP);

// LEDs
//pinMode(LedSuspendMouse, OUTPUT);
//pinMode(LedSuspendJoystick, OUTPUT);
//pinMode(LedAutoReload, OUTPUT);
pinMode(LedFire, OUTPUT);
//pinMode(LedRouge, OUTPUT);
//pinMode(LedVerte, OUTPUT);
//pinMode(LedBleu, OUTPUT);

// Mecánica
//pinMode(Solenoid, OUTPUT);
//digitalWrite(Solenoid, LOW); //  LOW = Repos

// Botones del Joystick
pinMode(_tiggerPin, INPUT_PULLUP);         // Configurar modos de pin
pinMode(_reloadPin, INPUT_PULLUP);
pinMode(_startPin, INPUT_PULLUP);
Joystick.setXAxisRange(0, 255);
Joystick.setYAxisRange(0, 255);
Joystick.begin();

AbsMouse.move((res_x / 2), (res_y / 2));          // Establecer la posición del ratón en el centro de la pantalla

delay(500);

// Apagar LED RGB
//ledRVB(0, 0, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Programa #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {

  /* ------------------ INICIAR/PAUSAR EL RATÓN ---------------------- */

  if (count > 3 ) {

    skip();
    mouseCount();
    //  PrintResults();

  }

  /* ---------------------- CENTRAR --------------------------- */
  // Comienzo de la procedura de calibración

  else if (count > 2 ) {

    AbsMouse.move((res_x / 2), (res_y / 2));

    mouseCount();
    getPosition();

    xCenter = finalX;
    yCenter = finalY;

    //  PrintResults();

  }

  /* -------------------- DESPLAZAMIENTO ------------------------- */

  else if (count > 1 ) {

    mouseCount();
    AbsMouse.move(conMoveXAxis, conMoveYAxis);
    getPosition();

    MoveYAxis = map (finalY, (yCenter + ((mySamco.H() * (yOffset / 100)) / 2)), (yCenter - ((mySamco.H() * (yOffset / 100)) / 2)), 0, res_y);
    conMoveXAxis = res_x / 2;
    conMoveYAxis = constrain (MoveYAxis, 0, res_y);

    if (plus == 1) {
      yOffset = yOffset + 1;
      delay(10);
    } else {
    }

    if (minus == 1) {
      yOffset = yOffset - 1;
      delay(10);
    } else {
    }

     PrintResults();

  }

  else if (count > 0 ) {

    mouseCount();
    AbsMouse.move(conMoveXAxis, conMoveYAxis);
    getPosition();

    MoveXAxis = map (finalX, (xCenter + ((mySamco.H() * (xOffset / 100)) / 2)), (xCenter - ((mySamco.H() * (xOffset / 100)) / 2)), 0, res_x);
    conMoveXAxis = constrain (MoveXAxis, 0, res_x);
    conMoveYAxis = res_y / 2;

    if (plus == 1) {
      xOffset = xOffset + 1;
      delay(10);
    } else {
    }

    if (minus == 1) {
      xOffset = xOffset - 1;
      delay(10);
    } else {
    }

     PrintResults();

  }

  else if (count > -1) {

    count = count - 1;

    EEPROM.write(0, xCenter - 256);
    EEPROM.write(1, yCenter - 256);
    EEPROM.write(2, xOffset);
    EEPROM.write(3, yOffset);
  }
  // Fin de la procedura de calibración
 /* ---------------------- ¡VAMOS! --------------------------- */
  // Uso normal del arma

  else {

    // Activación de los movimientos del ratón (si 0 = Interruptor desactivado > ratón desactivado)
    SuspendMouse = digitalRead(SwitchSuspendMouse);
    if ((SuspendMouse == 1) && (SuspendJoystick == 0)) {
      //      Serial.println("Ratón Activado");
//      digitalWrite(LedSuspendMouse, HIGH); // enciende el LED
      AbsMouse.move(conMoveXAxis, conMoveYAxis);  // Movimientos enviados al HID
      mouseButtons();                             // clics enviados al HID
      //    Serial.print("conMoveXAxis :");
      //    Serial.print(conMoveXAxis);
      //    Serial.print(",  conMoveYAxis :");
      //    Serial.println(conMoveYAxis);

    }
    else {      // desactivación completa del ratón
      //      Serial.println("Ratón desactivado");
      //      Serial.println(" ");
//      digitalWrite(LedSuspendMouse, LOW);  // apaga el LED
    }

    // Activación de los movimientos del joystick, 1 por defecto (si 0 = Interruptor desactivado > joystick desactivado)
    SuspendJoystick = digitalRead(SwitchSuspendJoystick);
    if ((SuspendMouse == 0) && (SuspendJoystick == 1)) {
      //      Serial.println("Joystick Activado");
//      digitalWrite(LedSuspendJoystick, HIGH); // enciende el LED
      Joystick_device_Buttons();   // Botones enviados al HID
      Joystick_device_Stick();   // Movimientos enviados al HID
    }
    else {      // desactivación completa del joystick
      //      Serial.println("Joystick desactivado");
      //      Serial.println(" ");
//      digitalWrite(LedSuspendJoystick, LOW);  // apaga el LED
      Joystick.setXAxis(127);   // Centrar el joystick
      Joystick.setYAxis(127);
    }

    // Activación del modo híbrido (Solo movimiento del ratón) y (botón del ratón + botón del joystick)
    if ((SuspendMouse == 1) && (SuspendJoystick == 1)) {
      //      Serial.println("modo híbrido activado");
//      digitalWrite(LedSuspendMouse, HIGH); // enciende el LED
//     digitalWrite(LedSuspendJoystick, HIGH); // enciende el LED
      AbsMouse.move(conMoveXAxis, conMoveYAxis);  // Movimientos enviados al HID
      Hybride_Buttons();   // Botones enviados al HID
    }

    getPosition();  // Recuperación de los valores de la cámara IR

    MoveXAxis = map (finalX, (xCenter + ((mySamco.H() * (xOffset / 100)) / 2)), (xCenter - ((mySamco.H() * (xOffset / 100)) / 2)), 0, res_x);
    MoveYAxis = map (finalY, (yCenter + ((mySamco.H() * (yOffset / 100)) / 2)), (yCenter - ((mySamco.H() * (yOffset / 100)) / 2)), 0, res_y);
    conMoveXAxis = constrain (MoveXAxis, 0, res_x);
    conMoveYAxis = constrain (MoveYAxis, 0, res_y);

    //    PrintResults();
    reset();

  }
  ModeAutomatic();
//  LedRGBautomatic();
}

/*        -----------------------------------------------        */
/* --------------------------- MÉTODOS ------------------------- */
/*        -----------------------------------------------        */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Gestion de la Posición de la Cámara IR #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPosition() {    // Obtener la posición ajustada en inclinación de la cámara de posición IR

  myDFRobotIRPosition.requestPosition();
  if (myDFRobotIRPosition.available()) {
    mySamco.begin(myDFRobotIRPosition.readX(0), myDFRobotIRPosition.readY(0), myDFRobotIRPosition.readX(1), myDFRobotIRPosition.readY(1), myDFRobotIRPosition.readX(2), myDFRobotIRPosition.readY(2), myDFRobotIRPosition.readX(3), myDFRobotIRPosition.readY(3), xCenter, yCenter);
    finalX = mySamco.X();
    finalY = mySamco.Y();
    see0 = mySamco.testSee(0);
    see1 = mySamco.testSee(1);
    see2 = mySamco.testSee(2);
    see3 = mySamco.testSee(3);
    //    Serial.print("finalX: ");
    //    Serial.print(finalX);
    //    Serial.print(",  finalY: ");
    //    Serial.print(finalY);

  }
  else {
    Serial.println("¡Dispositivo no disponible!");
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Gestion de los Movimientos del Joystick #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Joystick_device_Stick() {
  int xAxis = map(conMoveXAxis, 0, 1023, 0, 255); // Con inversión de ejes requerida
  int yAxis = map(conMoveYAxis, 0, 768, 0, 255);

  //  Serial.print("Joystick xAxis = ");
  //  Serial.print(xAxis);
  //  Serial.print(",  yAxis = ");
  //  Serial.println(yAxis);

  Joystick.setXAxis(xAxis);
  Joystick.setYAxis(yAxis);
}

/*
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Gestion de las Luces RGB #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ArduinoInitiationLedsDiversTestLedRVBac
//---- Función para combinar colores ON/OFF ----
void ledRVB(int Rouge, int Vert, int Bleu) {
  if (Rouge == 1) digitalWrite(LedRouge, LOW); // enciende el color
  if (Rouge == 0) digitalWrite(LedRouge, HIGH); // apaga el color

  if (Vert == 1) digitalWrite(LedVerte, LOW); // enciende el color
  if (Vert == 0) digitalWrite(LedVerte, HIGH); // apaga el color

  if (Bleu == 1) digitalWrite(LedBleu, LOW); // enciende el color
  if (Bleu == 0) digitalWrite(LedBleu, HIGH); // apaga el color
}

//---- Función para la variación progresiva de los colores ----
void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) { // recibe valores 0-255 por color
  analogWrite(LedRouge, 255 - pwmRouge); // ancho de pulso deseado en el pin 0 = 0% y 255 = 100% encendido
  analogWrite(LedVerte, 255 - pwmVert); // ancho de pulso deseado en el pin 0 = 0% y 255 = 100% encendido
  analogWrite(LedBleu, 255 - pwmBleu); // ancho de pulso deseado en el pin 0 = 0% y 255 = 100% encendido
}

//---- Función para fundido de colores arco iris ----
void FonduCouleurs() {
//  Secuencia :
//  255,0,0 rojo
//  255,255,0 amarillo
//  0,255,0 verde
//  0,255,255 cian
//  0,0,255 azul
//  255,0,255 violeta
//  Comenzamos en rojo con varR,varV,varB = 255,0,0

  ledRVBpwm(varR,varV,varB); // genera ancho de pulso deseado para el color
    
  if (SequenceRouge == 1) {  // 255,0,0 Rojo
    varV++; // aumenta el Verde
    if (varV == 255 ) { // obtenemos Amarillo (255,255,0)
      SequenceRouge = 0;
      SequenceJaune = 1;  // Activamos la siguiente secuencia
      SequenceVert = 0;
      SequenceCyan = 0;
      SequenceBleu = 0;
      SequenceViolet = 0;  
    }
  }
  
  if (SequenceJaune == 1) { // 255,255,0 Amarillo
    varR--;   // disminuimos el Rojo 
    if (varR == 0 ) {   // solo queda el Verde  (0,255,0)
      SequenceRouge = 0;
      SequenceJaune = 0;
      SequenceVert = 1;   // Activamos la siguiente secuencia
      SequenceCyan = 0;
      SequenceBleu = 0;
      SequenceViolet = 0;  
    }
  }
  
  if (SequenceVert == 1) {  // 0,255,0 Verde
    varB++;   // aumentamos el azul
    if (varB == 255 ) {   // obtenemos Cian  (0,255,255)
      SequenceRouge = 0;
      SequenceJaune = 0;
      SequenceVert = 0;
      SequenceCyan = 1;   // Activamos la siguiente secuencia
      SequenceBleu = 0;
      SequenceViolet = 0;  
    }
  }
    
  if (SequenceCyan == 1) {  // 0,255,255 Cian
    varV--;   // disminuimos el Verde
    if (varV == 0 ) {    // obtenemos Azul (0,0,255)
      SequenceRouge = 0;
      SequenceJaune = 0;
      SequenceVert = 0;
      SequenceCyan = 0;
      SequenceBleu = 1;   // Activamos la siguiente secuencia
      SequenceViolet = 0;  
    }
  }
  
  if (SequenceBleu == 1) {  // 0,0,255 Azul
    varR++;   // aumentamos el Rojo
    if (varR == 255 ) {     // obtenemos Violeta (255,0,255)
      SequenceRouge = 0;
      SequenceJaune = 0;
      SequenceVert = 0;
      SequenceCyan = 0;
      SequenceBleu = 0;
      SequenceViolet = 1;   // Activamos la siguiente secuencia 
    }
  }
  
  if (SequenceViolet == 1) {  // 255,0,255 Violeta
    varB--;   // disminuimos el Azul
    if (varB == 0 ) {     // obtenemos Rojo (255,0,0)
      Serial.print("SequenceRouge");
      SequenceRouge = 1;   // Activamos la siguiente secuencia 
      SequenceJaune = 0;
      SequenceVert = 0;
      SequenceCyan = 0;
      SequenceBleu = 0;
      SequenceViolet = 0;
    }
  }
//  Serial.print("");
//  Serial.print("varR : ");
//  Serial.print(varR);
//  Serial.print(" | varV : ");
//  Serial.print(varV);
//  Serial.print(" | varB : ");
//  Serial.println(varB);
//  delay(4);
}

void LedRGBautomatic() {
  switch (Automatic) {
    case 2:
      ledRVB(R, V, 0); // amarillo
      break;
    case 3:
      ledRVB(0, V, 0); // verde
      break;
    case 4:
      ledRVB(0, 0, B); // azul
      break;
    case 5:
      ledRVB(R, V, B); // blanco
      break;
    case 6:
      ledRVB(0, V, B); // cian
      break;
    case 7:
      ledRVB(R, 0, B); // violeta
      break;
    case 8:
      FonduCouleurs();  // Fundido de colores rojo-naranja-amarillo-verde-cian-azul-violeta-rosa
      break;
    case 1:
      ledRVB(R, 0, 0); // rojo
      break;
    default:
      // Si nada más coincide, se ejecuta el caso predeterminado.
      // El caso predeterminado es opcional.
      break;
  }
}
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Gestión de Eventos Opto-Mecánicos #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void clignotteLED() {
  static unsigned long initTime = 0;
  unsigned long maintenant = millis();
  if (maintenant - initTime >= 90) {
    initTime = maintenant;
    digitalWrite(LedFire, HIGH);
  }
  else {
    digitalWrite(LedFire, LOW);    // apagar el LED haciendo que el voltaje sea BAJO
  }
}

void ActionneSolenoid() {
  /*static unsigned long initTime = 0;
  unsigned long maintenant = millis();
  if (maintenant - initTime >= CadenceTir) {
    initTime = maintenant;
    // A diferencia del LED, el mecanismo necesita algo de tiempo para cambiar de estado
    unsigned long startTime = millis();
    unsigned long duree = 25; // mantener durante 25 µs para que el mecanismo tenga tiempo de cambiar de estado
    while (millis() < startTime + duree) {
      digitalWrite(Solenoid, HIGH);  // activado
    }
  }
  else {
    digitalWrite(Solenoid, LOW);   // reposo
  }
  delay(15);  // retraso para la "respiración" (de lo contrario, la cadencia de disparo no funciona correctamente)
  digitalWrite(Solenoid, LOW);
  */
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//# Gestión del Modo de Disparo #
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ModeAutomatic() {  // Selección del modo de disparo: 1 tiro, 2 tiros, 3 tiros, 4 tiros, 5 tiros, 6 tiros, Ametralladora, Mantener un tiro
  int ButtonState_reloadPin = digitalRead(_reloadPin);
  AutoReload = digitalRead(SwitchAutoReload);
  if (AutoReload == 0)  // Si el modo Reload está activado con el interruptor Reload
  {
    //LedRGBautomatic();
    if (ButtonState_reloadPin == 0) {
      static unsigned long initTime = 0;
      unsigned long maintenant = millis();
      if (maintenant - initTime >= 300) {
        initTime = maintenant;
        Automatic++;
        if (Automatic >= 9) {
          Automatic = 1;
        }
      }
    }
  }
}

void TIRenRafaleSouris() {
  if (fire == 1) {
    static unsigned long initTime = 0;
    unsigned long maintenant = millis();
    if (maintenant - initTime >= CadenceTir) {
      initTime = maintenant;
      AbsMouse.press(MOUSE_LEFT);
      digitalWrite(LedFire, HIGH);
      ActionneSolenoid();
      rafaleCount++;
      if (rafaleCount == NBdeTirs) {
        fire = 0;
        rafaleCount = 0;
      }
    }
    else {
      AbsMouse.release(MOUSE_LEFT);
      digitalWrite(LedFire, LOW);
    }
  }
  else {  // Si fire es diferente de 1, apagar el LED y no activar el disparo
    AbsMouse.release(MOUSE_LEFT);
    digitalWrite(LedFire, LOW);
  }
  delay(10);  // retraso para la "respiración" (de lo contrario, se bloquea...)
}

void TIRenRafaleJoyStick() {
  if (fire == 1) {
    static unsigned long initTime = 0;
    unsigned long maintenant = millis();
    if (maintenant - initTime >= CadenceTir) {
      initTime = maintenant;
      Joystick.setButton(0, HIGH);  // Botón presionado
      digitalWrite(LedFire, HIGH);
      ActionneSolenoid();
      rafaleCount++;
      if (rafaleCount == NBdeTirs) {
        fire = 0;
        rafaleCount = 0;
      }
    }
    else {
      Joystick.setButton(0, LOW);  // Botón liberado
      digitalWrite(LedFire, LOW);
    }
  }
  else {  // Si fire es diferente de 1, apagar el LED y no activar el disparo
    Joystick.setButton(0, LOW);  // Botón liberado
    digitalWrite(LedFire, LOW);
  }
  delay(15);  // retraso para la "respiración" (de lo contrario, la cadencia de disparo no funciona correctamente)
}

void TIRenRafaleHybride() {
  if (fire == 1) {
    static unsigned long initTime = 0;
    unsigned long maintenant = millis();
    if (maintenant - initTime >= CadenceTir) {  // bucle para un intervalo de tiempo
      initTime = maintenant;
      AbsMouse.press(MOUSE_LEFT);
      Joystick.setButton(0, HIGH);  // Botón presionado y luego
      digitalWrite(LedFire, HIGH);
      ActionneSolenoid();
      rafaleCount++;
      if (rafaleCount == NBdeTirs) {
        fire = 0;
        rafaleCount = 0;
      }
    }
    else {
      AbsMouse.release(MOUSE_LEFT);
      Joystick.setButton(0, LOW);  // Botón liberado inmediatamente
      digitalWrite(LedFire, LOW);
    }
  }
  else {  // Si fire es diferente de 1, apagar el LED y no activar el disparo
    AbsMouse.release(MOUSE_LEFT);
    Joystick.setButton(0, LOW);  // Botón liberado
    digitalWrite(LedFire, LOW);
  }
  delay(15);  // retraso para la "respiración" (de lo contrario, la cadencia de disparo no funciona correctamente)
}
//#############################
//# Gestión de Botones del Mouse #
//#############################
void mouseButtons() {    // Valores enviados al dispositivo HID para el mouse

  int ButtonState_tiggerPin = digitalRead(_tiggerPin);
  int ButtonState_reloadPin = digitalRead(_reloadPin);
  int ButtonState_startPin = digitalRead(_startPin);

  switch (Automatic) {
    case 2:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 1;
      TIRenRafaleSouris();
      break;
    case 3:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 2;
      TIRenRafaleSouris();
      break;
    case 4:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 3;
      TIRenRafaleSouris();
      break;
    case 5:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 4;
      TIRenRafaleSouris();
      break;
    case 6:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 5;
      TIRenRafaleSouris();
      break;
    case 7:
      if (ButtonState_tiggerPin != LastButtonState_tiggerPin) {
        if (ButtonState_tiggerPin == LOW) {
          fire = 1;
        }
        LastButtonState_tiggerPin = ButtonState_tiggerPin;
      }
      NBdeTirs = 6;
      TIRenRafaleSouris();
      break;
    case 8:   // Disparo automático de ametralladora
      if (ButtonState_tiggerPin == LOW) {
        fire = 1;
      }
      NBdeTirs = 1;
      TIRenRafaleSouris();
      break;
    case 1:   // 1 tiro con disparo sostenido (especial Alien3, Terminator, etc.)
      if (ButtonState_tiggerPin == LOW) {
        AbsMouse.press(MOUSE_LEFT);  // Botón presionado y sostenido para algunos juegos (Alien, T2, etc.)
        //clignotteLED();
		digitalWrite(LedFire, HIGH);
        ActionneSolenoid();
      }
      else {
        AbsMouse.release(MOUSE_LEFT);  // Botón liberado
        digitalWrite(LedFire, LOW);
      }
      delay(10);
      break;
    default:
      break;
  }

  //if (ButtonState_reloadPin != LastButtonState_reloadPin) { // _reloadPin
    if (ButtonState_reloadPin == LOW) {
      AbsMouse.press(MOUSE_RIGHT);
      //delay(50);
    }
    else {
      AbsMouse.release(MOUSE_RIGHT);
    }
    delay(10);
   // LastButtonState_reloadPin = ButtonState_reloadPin;
  //}

  if (ButtonState_startPin != LastButtonState_startPin) { // _startPin
    if (ButtonState_startPin == LOW) {
      AbsMouse.press(MOUSE_MIDDLE);
    }
    else {
      AbsMouse.release(MOUSE_MIDDLE);
    }
    delay(10);
    LastButtonState_startPin = ButtonState_startPin;
  }

  /* Modo RECARGA automática si el cursor sale de la pantalla, gestionado por el software */
  AutoReload = digitalRead(SwitchAutoReload);
  if (AutoReload == 0)  // Si el modo Reload está activado con el interruptor Reload
  {
//    digitalWrite(LedAutoReload, HIGH);  // enciende el LED Autoreload

    if ((see0 == 0) || (see1 == 0) || (see2 == 0) || (see3 == 0))  // si se sale de la pantalla
    {
      //Serial.println("Reload !!! Clic Derecho");
      AbsMouse.press(MOUSE_RIGHT);
    }
    else
    {
      AbsMouse.release(MOUSE_RIGHT);
    }
    delay(10);
  }
  else
  {
    //Serial.println("Modo Reload desactivado");
//    digitalWrite(LedAutoReload, LOW);  // Apaga el LED Autoreload
    //AbsMouse.release(MOUSE_RIGHT);
  }
  delay(10);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//###########################
//# Gestión de botones híbridos #
//###########################
void Hybride_Buttons() {    // Valores de botones del Mouse + Joystick enviados al dispositivo HID

  int ButtonState_tiggerPin = !digitalRead(0 + pinToButtonMap); // pin 14
  int ButtonState_startPin = !digitalRead(1 + pinToButtonMap); // pin 15
  int ButtonState_reloadPin = !digitalRead(2 + pinToButtonMap); // pin 16

  // Rutina para disparo en ráfaga (presión momentánea)
  switch (Automatic) {
    case 2:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 1;
      TIRenRafaleHybride();
      break;
    case 3:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 2;
      TIRenRafaleHybride();
      break;
    case 4:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 3;
      TIRenRafaleHybride();
      break;
    case 5:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 4;
      TIRenRafaleHybride();
      break;
    case 6:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 5;
      TIRenRafaleHybride();
      break;
    case 7:
      if (ButtonState_tiggerPin != LastButtonState[0]) {
        if (ButtonState_tiggerPin == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = ButtonState_tiggerPin;
      }
      NBdeTirs = 6;
      TIRenRafaleHybride();
      break;
    case 8:   // Autodisparo de ametralladora
      if (ButtonState_tiggerPin == HIGH) {
        fire = 1;
      }
      NBdeTirs = 1;
      TIRenRafaleHybride();
      break;
    case 1:   // 1 disparo con mantenimiento (especial Alien3, Terminator, etc.)
      if (ButtonState_tiggerPin == HIGH) {
        AbsMouse.press(MOUSE_LEFT);
        Joystick.setButton(0, HIGH);  // Botón presionado
        digitalWrite(LedFire, HIGH);
        ActionneSolenoid();
      }
      else {
        AbsMouse.release(MOUSE_LEFT);
        Joystick.setButton(0, LOW);  // Botón liberado
        digitalWrite(LedFire, LOW);
      }
      delay(15);
      break;
    default:
      break;
  }

  if (ButtonState_startPin != LastButtonState[1]) { // _startPin
    if (ButtonState_startPin == LOW) {
      AbsMouse.press(MOUSE_MIDDLE);
      Joystick.setButton(1, HIGH);  // Botón presionado
    }
    else {
      AbsMouse.release(MOUSE_MIDDLE);
      Joystick.setButton(1, LOW);  // Botón liberado
    }
    delay(15);
    LastButtonState[1] = ButtonState_startPin;
  }

  if (ButtonState_reloadPin != LastButtonState[2]) { // _reloadPin
    if (ButtonState_reloadPin == LOW) {
      AbsMouse.press(MOUSE_RIGHT);
      Joystick.setButton(2, HIGH);  // Botón presionado
    }
    else {
      AbsMouse.release(MOUSE_RIGHT);
      Joystick.setButton(2, LOW);  // Botón liberado
    }
    delay(15);
    LastButtonState[2] = ButtonState_reloadPin;
  }

  /* Modo de recarga automática si el cursor sale de la pantalla, gestionado por el software */
  AutoReload = digitalRead(SwitchAutoReload);
  if (AutoReload == 0)  // Si el modo de recarga automática está activado con el interruptor AutoReload
  {
//    digitalWrite(LedAutoReload, HIGH);  // Enciende el LED Autoreload

    if ((see0 == 0) || (see1 == 0) || (see2 == 0) || (see3 == 0))  // si se sale de la pantalla
    {
      //Serial.println("Recargar botón !!!");
      AbsMouse.press(MOUSE_RIGHT);
      Joystick.setButton(2, HIGH); // pin 16
    }
    else
    {
      AbsMouse.release(MOUSE_RIGHT);
      Joystick.setButton(2, LOW); // pin 16
    }
    delay(15);
  }
  else if (LastButtonState[2] == LOW)
  {
    //Serial.println("Modo de recarga desactivado");
//    digitalWrite(LedAutoReload, LOW);  // Apaga el LED Autoreload
    AbsMouse.release(MOUSE_RIGHT);
    Joystick.setButton(2, LOW);
  }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//###############################
//# Gestión de botones del Joystick #
//###############################
void Joystick_device_Buttons() {    // Valores de los botones del Joystick enviados al dispositivo HID

  //  for (int index = 0; index < 3; index++)
  //  {
  //    int currentButtonState = !digitalRead(index + pinToButtonMap);
  //    if (currentButtonState != LastButtonState[index])
  //    {
  //      Joystick.setButton(index, currentButtonState);
  //      LastButtonState[index] = currentButtonState;
  //    }
  //  }

  int Etat_bouton_Tir = !digitalRead(0 + pinToButtonMap); // pin 14
  int Etat_bouton_start = !digitalRead(1 + pinToButtonMap); // pin 15
  int Etat_bouton_Reload = !digitalRead(2 + pinToButtonMap); // pin 16

  // Rutina para disparar (mantener presionado)
  //  if (Etat_bouton_Tir != LastButtonState[0]) {
  //    Joystick.setButton(0, Etat_bouton_Tir);
  //    LastButtonState[0] = Etat_bouton_Tir;
  //  }

  // Rutina alternativa para disparar (mantener presionado)
  //  if (Etat_bouton_Tir != LastButtonState[0]) {
  //    if (Etat_bouton_Tir == HIGH) {
  //      Joystick.setButton(0, HIGH);  // Botón presionado
  //    }
  //    else {
  //      Joystick.setButton(0, LOW);  // Botón liberado
  //    }
  //    LastButtonState[0] = Etat_bouton_Tir;
  //  }

  // Rutina para disparo en ráfaga (presión momentánea)
  switch (Automatic) {
    case 2:   // 1 disparo
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 1;
      TIRenRafaleJoyStick();
      break;
    case 3:   // 2 disparos
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 2;
      TIRenRafaleJoyStick();
      break;
    case 4:   // 3 disparos
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 3;
      TIRenRafaleJoyStick();
      break;
    case 5:   // 4 disparos
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 4;
      TIRenRafaleJoyStick();
      break;
    case 6:   // 5 disparos
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 5;
      TIRenRafaleJoyStick();
      break;
    case 7:   // 6 disparos
      if (Etat_bouton_Tir != LastButtonState[0]) {
        if (Etat_bouton_Tir == HIGH) {
          fire = 1;
        }
        LastButtonState[0] = Etat_bouton_Tir;
      }
      NBdeTirs = 6;
      TIRenRafaleJoyStick();
      break;
    case 8:   // Autodisparo MachineGun
      if (Etat_bouton_Tir == HIGH) {
        fire = 1;
      }
      NBdeTirs = 1;
      TIRenRafaleJoyStick();
      break;
    case 1:   // 1 disparo con mantenimiento (especial Alien3, Terminator...)
      if (Etat_bouton_Tir == HIGH) {
        Joystick.setButton(0, HIGH);  // Botón presionado
        //clignotteLED();
		digitalWrite(LedFire, HIGH);
        ActionneSolenoid();
      }
      else {
        Joystick.setButton(0, LOW);  // Botón liberado
        digitalWrite(LedFire, LOW);
      }
      delay(15);
      break;
    default:
      break;
  }

  if (Etat_bouton_start != LastButtonState[1]) {
    Joystick.setButton(1, Etat_bouton_start);
    LastButtonState[1] = Etat_bouton_start;
  }

  if (Etat_bouton_Reload != LastButtonState[2]) {
    Joystick.setButton(2, Etat_bouton_Reload);
    LastButtonState[2] = Etat_bouton_Reload;
  }

  /* Modo de recarga automática si el cursor sale de la pantalla, gestionado por el software */
  AutoReload = digitalRead(SwitchAutoReload);
  if (AutoReload == 0)  // Si el modo de recarga automática está activado con el interruptor AutoReload
  {
//    digitalWrite(LedAutoReload, HIGH);  // Enciende el LED Autoreload

    if ((see0 == 0) || (see1 == 0) || (see2 == 0) || (see3 == 0))  // si se sale de la pantalla
    {
      //Serial.println("Recargar botón !!!");
      Joystick.setButton(2, HIGH); // pin 16
    }
    else
    {
      Joystick.setButton(2, LOW); // pin 16
    }

  }
  else if (LastButtonState[2] == LOW)
  {
    //Serial.println("Modo de recarga desactivado");
//    digitalWrite(LedAutoReload, LOW);  // Apaga el LED Autoreload
    Joystick.setButton(2, LOW);
  }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//###################################
//# Gestión de botones de calibración #
//###################################

void go() {    // Configurar botón de inicio de calibración

  // ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin));
  ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin) | digitalRead(_reloadPin));
  if (ButtonState_calibration != LastButtonState_calibration) {
    if (ButtonState_calibration == LOW) {
      count--;
    }
    else { // no hacer nada
    }
    delay(50);
  }
  LastButtonState_calibration = ButtonState_calibration;
}

void mouseCount() {    // Procedimiento de calibración

  ButtonValid = digitalRead(_tiggerPin);
  ButtonPlus = digitalRead(_startPin);
  ButtonMinus = digitalRead(_reloadPin);

  if (ButtonValid != LastButtonValid) { // validación
    if (ButtonValid == LOW) {
      count--;
    }
    else {
    }
    delay(10);
  }

  if (ButtonPlus != LastButtonPlus) { // ajuste +
    if (ButtonPlus == LOW) {
      plus = 1;
    }
    else {
      plus = 0;
    }
    delay(10);
  }

  if (ButtonMinus != LastButtonMinus) { // ajuste -
    if (ButtonMinus == LOW) {
      minus = 1;
    }
    else {
      minus = 0;
    }
    delay(10);
  }

  LastButtonValid = ButtonValid;
  LastButtonPlus = ButtonPlus;
  LastButtonMinus = ButtonMinus;
}


void reset() {    // Botón de pausa/recalibración

  // ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin));
  ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin) | digitalRead(_reloadPin));
  if (ButtonState_calibration != LastButtonState_calibration) {
    if (ButtonState_calibration == LOW) {
      count = 4;
      delay(50);
    }
    else { // no hacer nada
    }
    delay(50);
  }
  LastButtonState_calibration = ButtonState_calibration;
}


void skip() {    // Botón de reanudar

  //ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin));
  ButtonState_calibration = (digitalRead(_startPin) | digitalRead(_tiggerPin) | digitalRead(_reloadPin));
  if (ButtonState_calibration != LastButtonState_calibration) {
    if (ButtonState_calibration == LOW) {
      count = 0;
      delay(50);
    }
    else { // no hacer nada
    }
    delay(50);
  }
  LastButtonState_calibration = ButtonState_calibration;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//##########################
//# Gestión de la memoria EEPROM #
//##########################
void loadSettings() {     // Recargar los valores de calibración desde la memoria EEPROM
  if (EEPROM.read(1023) == 'T') {
    // los ajustes se han inicializado, leerlos
    xCenter = EEPROM.read(0) + 256;
    yCenter = EEPROM.read(1) + 256;
    xOffset = EEPROM.read(2);
    yOffset = EEPROM.read(3);
  } else {
    // primera ejecución, los ajustes nunca se establecieron
    EEPROM.write(0, xCenter - 256);
    EEPROM.write(1, yCenter - 256);
    EEPROM.write(2, xOffset);
    EEPROM.write(3, yOffset);
    EEPROM.write(1023, 'T');
  }
}

void PrintResults() {    // Imprimir resultados para guardar la calibración
  Serial.print("CALIBRACIÓN:");
  Serial.print("     Centro de la cámara x/y: ");
  Serial.print(xCenter);
  Serial.print(", ");
  Serial.print(yCenter);
  Serial.print("     Desplazamientos x/y: ");
  Serial.print(xOffset);
  Serial.print(", ");
  Serial.println(yOffset);
}

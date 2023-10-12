/*
    Este es un ejemplo de código Arduino para la Cámara Infrarroja de Posicionamiento de DFRobot (SKU: SEN0158).
    Básicamente, es una cámara que puede "ver" luces infrarrojas y determinar las coordenadas de esas luces, similar a lo que se encuentra dentro de un Wiimote.
    Puede seguir hasta cuatro luces infrarrojas a la vez.
    Puedes encontrar más información sobre este sensor en https://www.dfrobot.com/index.php?route=product/product&product_id=1088

    LO QUE HACE ESTE BOCETO:
    Muestra los valores de coordenadas x e y para cuatro puntos en el puerto serie.
    Cuando un punto no se detecta (bloqueado, fuera de rango, etc.), el sensor muestra 1023 tanto para los valores de x como para los de y.

    CONEXIÓN:
            Arduino     Cámara Infrarroja de Posicionamiento
    ===============     =====================
                 5V --> Cable rojo
                GND --> Cable negro
    SDA (A4 en UNO) --> Cable amarillo
    SCL (A5 en UNO) --> Cable verde
    
    HISTORIAL DE MODIFICACIONES:
    // modificado para usar con pistolas de luz SAMCO por Sam Ballantyne, octubre de 2023.
    // modificado para usar con pistolas de luz SAMCO por Sam Ballantyne, noviembre de 2019.
    // modificado para http://DFRobot.com por kurakura, enero de 2015.
    // modificado para http://DFRobot.com por Lumi, enero de 2014.
    // modificado la salida para el programa Wii-BlobTrack de RobotFreak en http://www.letsmakerobots.com/user/1433
    // Código de prueba de sensor IR de Wii Remote por kako en http://www.kako.com
*/

#include <Wire.h>
#include <DFRobotIRPosition.h>

DFRobotIRPosition myDFRobotIRPosition; // declare a IRCam object

int positionX[4]; // RAW Sensor Values
int positionY[4];

int oneY = 0; // Re-mapped so left sensor is always read first
int oneX = 0;
int twoY = 0;
int twoX = 0;

int caliPin = 14; // Set Calibration Pin
int leftPin = 15; // Set Left Mouse Pin
int rightPin = 16; // Set Right Mouse Pin
int middlePin = 4; // Set Middle Mouse Pin
int pedalPin = 7; // Set pedal Pin
int mousePin = 8; // Set Mouse Pin

int caliButton = 0;
int leftButton = 0;
int rightButton = 0;
int middleButton = 0;
int pedalButton = 0;
int mouseButton = 0;

int buttonState1 = 0; // Set Button states
int lastButtonState1 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonState3 = 0;
int lastButtonState3 = 0;
int buttonState4 = 0;
int lastButtonState4 = 0;
int buttonState5 = 0;
int lastButtonState5 = 0;

void setup() {
  digitalWrite(5, HIGH); // This pin is used to power the IR Camera when using Samco PCB
  delay(500);
  myDFRobotIRPosition.begin(); // initialize the object in setup()
  Serial.begin(9600);

  pinMode(caliPin, INPUT_PULLUP); // Set pin modes
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(middlePin, INPUT_PULLUP);
  pinMode(pedalPin, INPUT_PULLUP);
  pinMode(mousePin, INPUT_PULLUP); // Set Mouse Pin
}

void loop() {

  getPosition();
  mouseButtons();
  pedalButtons();
  PrintResults();
}

void mouseButtons() { // Setup Cali, Left, Right, Middle, and Mouse buttons
  buttonState1 = digitalRead(caliPin);
  buttonState2 = digitalRead(leftPin);
  buttonState3 = digitalRead(rightPin);
  buttonState4 = digitalRead(middlePin);
  buttonState5 = digitalRead(mousePin);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      caliButton = 0;
    } else {
      caliButton = 255;
    }
    delay(10);
  }

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      leftButton = 0;
    } else {
      leftButton = 255;
    }
    delay(10);
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      rightButton = 0;
    } else {
      rightButton = 255;
    }
    delay(10);
  }

  if (buttonState4 != lastButtonState4) {
    if (buttonState4 == LOW) {
      middleButton = 0;
    } else {
      middleButton = 255;
    }
    delay(10);
  }

  if (buttonState5 != lastButtonState5) {
    if (buttonState5 == LOW) {
      mouseButton = 0;
    } else {
      mouseButton = 255;
    }
    delay(10);
  }

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;
  lastButtonState5 = buttonState5;
}

void pedalButtons() { // Setup Cali, Left, Right, Middle, and Mouse buttons
  buttonState5 = digitalRead(pedalPin);

  if (buttonState5 != lastButtonState5) {
    if (buttonState5 == LOW) {
      pedalButton = 0;
    } else {
      pedalButton = 255;
    }
    delay(10);
  }

  lastButtonState5 = buttonState5;
}

void PrintResults() { // Print results for debugging
  Serial.print(oneX);
  Serial.print(",");
  Serial.print(oneY);
  Serial.print(",");
  Serial.print(twoX);
  Serial.print(",");
  Serial.print(twoY);
  Serial.print(",");
  Serial.print(positionX[2]);
  Serial.print(",");
  Serial.print(positionY[2]);
  Serial.print(",");
  Serial.print(positionX[3]);
  Serial.print(",");
  Serial.print(positionY[3]);
  Serial.print(",");
  Serial.print(caliButton);
  Serial.print(",");
  Serial.print(leftButton);
  Serial.print(",");
  Serial.print(rightButton);
  Serial.print(",");
  Serial.print(middleButton);
  Serial.print(",");
  Serial.print(pedalButton);
  Serial.print(",");
  Serial.print(mouseButton);
  Serial.println();
  delay(20);
}

void getPosition() { // Get tilt adjusted position from IR postioning camera
  myDFRobotIRPosition.requestPosition();

  if (myDFRobotIRPosition.available()) {
    for (int i = 0; i < 4; i++) {
      positionX[i] = myDFRobotIRPosition.readX(i);
      positionY[i] = map(myDFRobotIRPosition.readY(i), 0, 768, 768, 0);
    }

    if (positionX[0] > positionX[1]) {
      oneY = positionY[0];
      oneX = positionX[0];
      twoY = positionY[1];
      twoX = positionX[1];
    } else if (positionX[0] < positionX[1]) {
      oneY = positionY[1];
      oneX = positionX[1];
      twoY = positionY[0];
      twoX = positionX[0];
    } else {
      oneY = 1023;
      oneX = 0;
      twoY = 1023;
      twoX = 0;
    }
  }
}

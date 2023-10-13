#include <Wire.h>
#include <DFRobotIRPosition.h>

DFRobotIRPosition myDFRobotIRPosition; // declare a IRCam object

int positionX[4]; // RAW Sensor Values
int positionY[4];

int oneY = 0; // Re-mapped so left sensor is always read first
int oneX = 0;
int twoY = 0;
int twoX = 0;
int redPin = 9;
int greenPin = 6;
int bluePin = 5;
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
int buttonState = 0; // Variable para almacenar el estado del botón
int lastButtonState = 0; // Variable para almacenar el estado previo del botón
int currentColor = 0; // Variable para rastrear el color actual


void setup() {
  delay(500);
  myDFRobotIRPosition.begin(); // initialize the object in setup()
  Serial.begin(9600);
pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(caliPin, INPUT_PULLUP); // Set pin modes
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(middlePin, INPUT_PULLUP);
  pinMode(pedalPin, INPUT_PULLUP);
  pinMode(mousePin, INPUT_PULLUP); // Set Mouse Pin
  setColor(255, 0, 0); // Enciende el LED RGB en rojo al inicio

}

void loop() {

  getPosition();
  mouseButtons();
  pedalButtons();
  PrintResults();
  buttonState = digitalRead(rightPin);

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      // Cambia el color del LED RGB al presionar rightPin
      changeColor();
    }
    lastButtonState = buttonState;
  }
}
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void changeColor() {
  currentColor++; // Incrementa el color actual
  if (currentColor > 2) {
    currentColor = 0; // Reinicia el ciclo cuando alcanza el tercer color
  }

  if (currentColor == 0) {
    setColor(255, 0, 0); // Rojo
  } else if (currentColor == 1) {
    setColor(0, 255, 0); // Verde
  } else if (currentColor == 2) {
    setColor(255, 255, 0); // Azul
  }
}

void mouseButtons() { // Setup Cali, Left, Right, Middle, and Mouse buttons
  buttonState1 = digitalRead(caliPin);
  
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      caliButton = 0;
      digitalWrite(A0, HIGH);  // Enciende el LED en el pin A0 cuando caliButton está presionado (LOW).
    } else {
      caliButton = 255;
      digitalWrite(A0, LOW);   // Apaga el LED en el pin A0 cuando caliButton se suelta (HIGH).
    }
    delay(10);
  }

  lastButtonState1 = buttonState1;

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

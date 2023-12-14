import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import javax.swing.JOptionPane; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class sketch_4 extends PApplet {




Serial myPort;  // El puerto serial
int lf = 10;    // Linefeed in ASCII
String myString = null;
String selectedPort = "";

int p1x = 1023;
int p1y = 1023;
int p2x = 1023;
int p2y = 1023;
int p3x = 1023;
int p3y = 1023;
int p4x = 1023;
int p4y = 1023;
int cali = color(255, 0, 0);
int left = color(255, 0, 0);
int right = color(255, 0, 0);
int middle = color(255, 0, 0);
int mouse = color(255, 0, 0);
int alt = color(255, 0, 0);
 // declare variables to hold color for the four points
 int p1color = color( 255, 0, 0 ); // RED
 int p2color = color( 0, 255, 0 ); // GREEN
 int p3color = color( 0, 0, 255 ); // BLUE
 int p4color = color( 0, 255, 255 ); // BLUE
 
 
public void setup() {
  
  frameRate(30);

  // Obtener la lista de puertos COM disponibles
  String[] portList = Serial.list();

  // Mostrar ventana de selección de puerto COM
  selectedPort = (String) JOptionPane.showInputDialog(
    null,
    "Selecciona un puerto COM:",
    "Selección de Puerto",
    JOptionPane.QUESTION_MESSAGE,
    null,
    portList,
    portList[0]
  );

  // Verificar si se seleccionó un puerto
  if (selectedPort == null || selectedPort.equals("")) {
    println("No se seleccionó un puerto. La aplicación se cerrará.");
    exit();
  }

  // Inicializar el puerto serial seleccionado
  myPort = new Serial(this, selectedPort, 9600);
  myPort.clear();
}
 
 
public void draw() {
  // preparation stage
  while( myPort.available() > 0 ) {
     myString = myPort.readStringUntil(lf);
     if( myString != null) {
       convertmyStringToCoordinates();
     }
  }

  
  // drawing stage   
  background(88); // repaint the whole drawing area with dark grey color (77,77,77), making the whole window clear and clean

   buttonTest( 280, 668, cali );
  buttonTest( 380, 668, left );
  buttonTest( 480, 668, right );
  buttonTest( 580, 668, middle );
  buttonTest( 680, 668, alt );
  buttonTest( 780, 668, mouse );
  textSize(12);
  textAlign(CENTER);
  fill ( 255, 255, 255 );
  text("GATILLO", 280, 710); 
  text("START", 380, 710);
  text("RECARGA", 480, 710);
  text("AUTO", 580, 710); 
  text("JOYSTICK", 680, 710);
  text("MOUSE", 780, 710);
  textAlign(CENTER);
    text("SI ESTOS BOTONES APARCEN EN ROJO, TODOS AL MISMO TIEMPO,", 512, 735); 
  text("DEBES CERRAR LA APP, DESCONECTAR Y CONECTAR EL PRO MICRO Y ABRIR LA APP", 512, 750); 
  text("www.mundoyakara.com", 512, 15); 
  textSize(35);
  text("IR GUN MAKOTO YAKARIZADO", 512, 50); 
     translate(width / 2, height / 2); // Mueve el origen al centro de la ventana.
  rotate(PI); // Rota 180 grados.

  // Restaura el origen y la rotación después de dibujar.
 translate(-width / 2, -height / 2);

  // immediately draw the circles after clearing, we've done the time-consuming preparation beforehand in convertmyStringToCoordinates() so this will give us minimal lag ( hopefully no flickering ).
  drawCircle( p1x, p1y, p1color );
  drawCircle( p2x, p2y, p1color );
  drawCircle( p3x, p3y, p1color );
  drawCircle( p4x, p4y, p1color );
  

}  



    
public void drawCircle( int xval, int yval, int c ){ 
  if( xval != 1023 && yval != 1023 ){ // only draw when both x and y is not 1023. When x=1023 and y=1023, the point is NOT detected by the IR camera, i.e. out of range
    ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
    fill( c );  // Set the fill color
    ellipse(xval, yval, 20, 20); //draws an ellipse with with horizontal diameter of 20px andvertical diameter of 20px. 
  }
}

public void buttonTest( int xval, int yval, int c ){ 
    ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
    fill( c );  // Set the fill color
    ellipse(xval, yval, 20, 20); //draws an ellipse with with horizontal diameter of 20px andvertical diameter of 20px. 
  }

  


public void convertmyStringToCoordinates() {
  println(myString); // display the incoming string

  // the next line does many things - it creates an array of integers named output, splits the string into 8 pieces of text, using comma as the delimiter, converts each of the 8 pieces of text into numbers and store them into the array in a sequential manner.  
  int[] output = PApplet.parseInt (split(trim(myString), ',')); 
  
  // now we need to copy the values from the array into global variables p1x..p4y, and make them available outside of this procedure.
  // because we need to access them at the drawing stage, later in the draw() cycle
  p1x = output[0];
  p1y = output[1];
       
  p2x = output[2];
  p2y = output[3];
       
  p3x = output[4];
  p3y = output[5];
       
  p4x = output[6];
  p4y = output[7];
  
  cali = output[8];
  left = output[9];
  right = output[10];
  middle = output[11];
  alt = output[12];
  mouse = output[13];}
  public void settings() {  size(1023, 768); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "sketch_4" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}

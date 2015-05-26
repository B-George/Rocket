/*
  Benjamin George
  Seattle University | Electrical and Computer Engineering
  
  3-Axis accelerometer data logger for Seattle University SEDS team test rocket
  uses arduino nano, adafruit adxl326 accelerometer module (ACM) and 
  sainsmart MicroSD card Adapter (MCA)
  
 Reads analog XYZ input pins, maps the results to a range from 0 to 255
 and outputs result to 3 discrete CSV files for MatLab analysis.
 Also prints the results to the serial monitor.

 The circuit:
 * ACM connected to analog 0-2, +5V and Gnd, with 3Vo connected to AREF pin
   Xout A2
   Yout A1
   Zout A0
   
   MCA connecte to digital 10-13, +5V and Gnd  
   MOSI 11
   MISO 12
   SCK  13
   CS   10
   
*/
#include <SPI.h>
#include <SD.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int Xpin = A2;
const int Ypin = A1;
const int Zpin = A0; 
const int CS = 10;

int Xval, Yval, Zval = 0;  // value read from the pot
int Xout, Yout, Zout = 0;  //value output to the CSV file

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  analogReference(EXTERNAL); // set reference voltage to 3.3V out from a.c.m.
  
  pinMode(10, OUTPUT); // set CS pin to output
  
  //************************************** TEST
  Serial.print("Initializing SD card");
  if(!SD.begin(CS)) {
     Serial.println("Card failed or not present");
     return;
  }
  Serial.println("Card initilized");
} //************************************ end TEST
     
void loop() {
  
       // read the analog in value:
    Xval = analogRead(Xpin);
    Yval = analogRead(Ypin);
    Zval = analogRead(Zpin);
  // map it to the range of the analog out:
    Xout = map(Xval, 0, 1023, -256, 255); // G-force == mapped value/16
    Yout = map(Yval, 0, 1023, -256, 255);
    Zout = map(Zval, 0, 1023, -256, 255);
   
  // print result to serial monitor ********(TEST)
    Serial.print("X = " );
    Serial.print(Xval);
    Serial.print("\t X output = ");
    Serial.println(Xout);
    Serial.print("Y = " );
    Serial.print(Yval);
    Serial.print("\t Y output = ");
    Serial.println(Yout);
    Serial.print("Z = " );
    Serial.print(Zval);
    Serial.print("\t Z output = ");
    Serial.println(Zout);
  // *********************************  end TEST
  
  String Xstring, Ystring, Zstring = "";
  Xstring += String(Xout);
  Xstring += ",";
  Ystring += String(Yout);
  Ystring += ",";
  Zstring += String(Zout);
  Zstring += ",";
  
  File Xfile = SD.open("X.csv", FILE_WRITE);
  if(Xfile) {
    Xfile.println(Xstring);
    Xfile.close();
    // print to serial port ****************TEST
    Serial.println(Xstring);
  }else{
    Serial.println("error opening X.csv");
  }

  File Yfile = SD.open("Y.csv", FILE_WRITE);
  if(Yfile) {
    Yfile.println(Ystring);
    Yfile.close();
    // print to serial port ****************TEST
    Serial.println(Ystring);
  }else{
    Serial.println("error opening Y.csv");
  }
  
  File Zfile = SD.open("Z.csv", FILE_WRITE);
  if(Zfile) {
    Zfile.println(Zstring);
    Zfile.close();
    // print to serial port ****************TEST
    Serial.println(Zstring);
  }else{
    Serial.println("error opening Z.csv");
  }  
  
  // wait 20 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  
  delay(20);
}



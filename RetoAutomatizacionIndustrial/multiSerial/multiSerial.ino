
//Lectura de gps via serial de software y envia a esp8266 via serial de hardware

//https://docs.arduino.cc/learn/built-in-libraries/software-serial
//https://docs.arduino.cc/tutorials/communication/SoftwareSerialExample

#include <SoftwareSerial.h>

int rxPin=9;
int txPin=10;

SoftwareSerial gps_serial(rxPin,txPin); // RX, TX

void setup(){
  // Open serial communications and wait for port to open:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial){
    ; // wait for serial port to connect. Needed for Native USB only
  }

  // set the data rate for the SoftwareSerial port
  gps_serial.begin(9600);
}

void loop(){ // run over and over
  if (gps_serial.available()){
    Serial.write(gps_serial.read());
  }
}
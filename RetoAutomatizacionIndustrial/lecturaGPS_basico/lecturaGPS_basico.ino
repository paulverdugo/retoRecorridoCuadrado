#include <SoftwareSerial.h>

SoftwareSerial gps(A0,A1);
char dato=' ';

void setup(){
 Serial.begin(115200);            
 gps.begin(9600); 
}


void loop(){
  if(gps.available()){
    dato=gps.read();
    Serial.print(dato);
  }
}

//https://naylampmechatronics.com/blog/45_tutorial-mpu6050-acelerometro-y-giroscopio.html
// ej 5 

// Librerias I2C para controlar el mpu6050
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
MPU6050 sensor;
// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int gx, gy, gz;
long tiempo_prev,tiempo_init, dt;
float girosc_ang_z;
float girosc_ang_z_prev;
bool giro=true;

int PinIN1 = 6;
int PinIN2 = 7;
int PinIN3 = 8;
int PinIN4 = 9;
int ENA = 5;
int ENB = 10;
int speed = 255;

void setup(){
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  delay(2000);
  Serial.begin(9600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor
  tiempo_prev=millis();
  tiempo_init=millis();
}

void loop(){
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
  digitalWrite(ENA,speed);
  digitalWrite(ENB,speed);
  // Leer las velocidades angulares 
  sensor.getRotation(&gx, &gy, &gz);
  //Calcular angulo rotacion:
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;
  girosc_ang_z_prev=girosc_ang_z;

  Serial.print(" Rotacion en Z: ");
  Serial.println(girosc_ang_z);
  if(millis()>tiempo_init+2000 && giro==true){
    delay(1000);
    digitalWrite(PinIN1, HIGH);
    digitalWrite(PinIN2, LOW);
    digitalWrite(PinIN3, LOW);
    digitalWrite(PinIN4, HIGH);
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    if(girosc_ang_z>-90){
      giro=false;
    }
  }
  delay(100);
}
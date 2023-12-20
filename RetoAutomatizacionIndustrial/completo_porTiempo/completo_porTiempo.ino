//leer distancia, leer giro, leer encoders, controlar motores

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

//motores izquierda
int ENA = 5;
int PinIN1 = 6;
int PinIN2 = 7;
//motores derecha
int PinIN3 = 8;
int PinIN4 = 9;
int ENB = 10;
int speed = 255;
bool recto=true;
float distancia=1500; //tiempo en ms llendo recto

void setup() {
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

  if(millis()>=tiempo_init+distancia){
    giro();
    tiempo_init=millis();
  }
}
void giro(){
  stop();
  restart_giro();
  delay(1500);
  while(abs(girosc_ang_z)<90){
    // Leer las velocidades angulares 
    sensor.getRotation(&gx, &gy, &gz);
    //Calcular angulo rotacion:
    dt = millis()-tiempo_prev;
    tiempo_prev=millis();
    girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;
    girosc_ang_z_prev=girosc_ang_z;
    digitalWrite(PinIN1, HIGH);
    digitalWrite(PinIN2, LOW);
    digitalWrite(PinIN3, LOW);
    digitalWrite(PinIN4, HIGH);
    digitalWrite(ENA,speed);
    digitalWrite(ENB,speed);
  }
  stop();
  restart_giro();
  delay(1500);
}
void restart_giro(){
  Wire.begin();
  sensor.initialize();
  girosc_ang_z_prev=0.0;
  girosc_ang_z=0.0;
}
void stop(){
  digitalWrite(ENA,0);
  digitalWrite(ENB,0);
}
//leer distancia, leer giro, leer encoders, controlar motores

float set_distancia=8; //distancia por recorrer (mts)
int delay_giro=5000; //tiempo por detenerse antes y despues de giro (ms)
int speed = 150; //pwm motores base

// Librerias I2C para controlar el mpu6050
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <SoftwareSerial.h>

MPU6050 sensor; //SCL-A5 SDA-A4

int rx=12; 
int tx=13; 
SoftwareSerial Serial2(rx,tx);
float tiempo_wifi=0;

//Valores para calculos encoder
int resol = 20;
float diam = 0.066;
float dist_prom;
int dist_accum=0;

//Encoder izquierdo 
float distL = 0.00;
volatile long encoderTicksL = 0;
int encoderPinL=2;
 
//Encoder derecho 
float distR = 0.00;
volatile long encoderTicksR = 0;
int encoderPinR=3;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int gx, gy, gz;
long tiempo_prev,tiempo_init, dt;
float girosc_ang_z;
float girosc_ang_z_prev;

//motores izquierda
int ENA = 5;
int PinIN1 = 7;
int PinIN2 = 8;
//motores derecha
int PinIN3 = 9;
int PinIN4 = 10;
int ENB = 6;

// Constantes del PID
float Kd = 29.61;
float Ki = 0.6073;
float Kp = 14.65;
float PID = 0;
int error = 0;
int error_a = 0;

void setup() {
  //encoders
  pinMode(encoderPinR, INPUT);
  pinMode(encoderPinL,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinL), updateEncoderL, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinR), updateEncoderR, RISING);
  //motores
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  //
  Serial.begin(9600);    //Iniciando puerto serial
  Serial2.begin(9600);
  Wire.begin();           //Iniciando I2C  
  sensor.initialize(); //Iniciando el IMU
  delay(2000);  
  Serial.print(0); 
  Serial2.print(0); 
}

void loop(){
  //PID orientacion (go straight)
  dt = millis() - tiempo_prev;
  tiempo_prev = millis();
  int gx, gy, gz;
  sensor.getRotation(&gx, &gy, &gz);
  girosc_ang_z = (gz / 131) * dt / 1000.0 + girosc_ang_z_prev;
  girosc_ang_z_prev = girosc_ang_z;
  error = 0 - girosc_ang_z;
  float p = Kp * error;
  float i = Ki * error * 0.1;
  float d = (Kd * (error - error_a)) / 0.01;
  PID = p + i + d;
  // Aplicar PID a los motores
  int PWM_a = speed - PID;
  int PWM_b = speed + PID;
  // Asegurar que los valores de PWM estén en el rango adecuado
  PWM_a = constrain(PWM_a, 0, 255);
  PWM_b = constrain(PWM_b, 0, 255);
  error_a = error; // Actualizar el error anterior

  //motores fwd
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  digitalWrite(ENA,PWM_a);
  digitalWrite(ENB,PWM_b);

  //calcular distancia encoders
  distR=(encoderTicksR)/resol*(PI*diam);
  distL=(encoderTicksL)/resol*(PI*diam);
  dist_prom = (distR+distL)/2.0;
  int distancia_allTime = int(dist_prom*100)+ dist_accum;
  if(millis()>=tiempo_wifi+20000){
    Serial2.print(distancia_allTime);
    Serial.println(distancia_allTime);
    tiempo_wifi=millis();
  }
  if(dist_prom >= set_distancia){ //condicion para girar
    giro();
    dist_accum=distancia_allTime;
    encoderTicksR=0; //resetear distancias
    encoderTicksL=0;
  }
}
void giro(){
  stop();
  restart_giro();
  delay(delay_giro);
  while(abs(girosc_ang_z)<86){
    digitalWrite(PinIN1, HIGH);
    digitalWrite(PinIN2, LOW);
    digitalWrite(PinIN3, LOW);
    digitalWrite(PinIN4, HIGH);
    digitalWrite(ENA,250);
    digitalWrite(ENB,250);
    // Leer las velocidades angulares 
    sensor.getRotation(&gx, &gy, &gz);
    //Calcular angulo rotacion:
    dt = millis()-tiempo_prev;
    tiempo_prev=millis();
    girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;
    girosc_ang_z_prev=girosc_ang_z;
  }
  stop();
  restart_giro();
  delay(delay_giro);
}
void restart_giro(){
  Wire.begin();
  sensor.initialize();
  girosc_ang_z_prev=0.0;
  girosc_ang_z=0.0;
}
void stop(){
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
}
void updateEncoderR() {
  encoderTicksR++;
}
void updateEncoderL(){
  encoderTicksL++;
}
#include <Wire.h>
#include <MPU6050.h>

MPU6050 sensor;

int motorIzquierdo1 = 8;
int motorIzquierdo2 = 9;
int motorIzquierdoPWM = 10;

int motorderecho1 = 6;
int motorderecho2 = 7;
int motorderechoPWM = 5;

float giroscAngZ, giroscAngZPrev;
long tiempoPrev, dt;

// Constantes del PID
float Kd = 9.02;  // Ajusta estos valores según sea necesario
float Ki = 6.39;
float Kp = 23.67;

float PID = 0;

int error = 0;
int error_a = 0;

int vel_baseA = 150;  // Ajusta estas velocidades según sea necesario
int vel_baseB = 150;

void setup() {
  // Motor Set Up
  pinMode(motorIzquierdo1, OUTPUT);
  pinMode(motorIzquierdo2, OUTPUT);
  pinMode(motorderecho1, OUTPUT);
  pinMode(motorderecho2, OUTPUT);
  pinMode(motorIzquierdoPWM, OUTPUT); // A
  pinMode(motorderechoPWM, OUTPUT);   // B

  Serial.begin(9600);
  Wire.begin();
  sensor.initialize();

  if (sensor.testConnection())
    Serial.println("Sensor iniciado correctamente");
  else
    Serial.println("Error al iniciar el sensor");

  delay(2000);
}

void loop() {
  dt = millis() - tiempoPrev;
  tiempoPrev = millis();

  int gx, gy, gz;
  sensor.getRotation(&gx, &gy, &gz);

  giroscAngZ = (gz / 131) * dt / 1000.0 + giroscAngZPrev;
  giroscAngZPrev = giroscAngZ;

  error = 0 - giroscAngZ;

  float p = Kp * error;
  float i = Ki * error * 0.1;
  float d = (Kd * (error - error_a)) / 0.01;
  PID = p + i + d;

  // Aplicar PID a los motores
  int PWM_a = vel_baseA + PID;
  int PWM_b = vel_baseB - PID;

  // Asegurar que los valores de PWM estén en el rango adecuado
  PWM_a = constrain(PWM_a, 0, 255);
  PWM_b = constrain(PWM_b, 0, 255);

  // Controlar motores
  analogWrite(motorIzquierdoPWM, PWM_b);
  analogWrite(motorderechoPWM, PWM_a);

  // Actualizar el error anterior
  error_a = error;

  digitalWrite(motorIzquierdo1, LOW);
  digitalWrite(motorIzquierdo2, HIGH);
  digitalWrite(motorderecho1, LOW);
  digitalWrite(motorderecho2, HIGH);

  // Leer la velocidad angular/ángulo del eje Z del giroscopio
  Serial.print("\tÁngulo Z: ");
  Serial.println(giroscAngZ);

  Serial.print("\tPWM left: ");
  Serial.println(PWM_b);

  Serial.print("\tPWM right: ");
  Serial.println(PWM_a);

  delay(10);
}
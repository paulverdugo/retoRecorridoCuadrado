#include <Wire.h>
#include <MPU6050.h>

MPU6050 sensor;

int motorIzquierdo1 = 7;
int motorIzquierdo2 = 8;
int motorIzquierdoPWM = 5;

int motorderecho1 = 9;
int motorderecho2 = 10;
int motorderechooPWM = 6;

int pwmIzquierdo = 100;
int pwmDerecho = 250;
int pwmIzquierdo2 = 250;
int pwmDerecho2 = 100;
int tiempoDeCambio = 500;
int tiempo = millis();

long tiempoPrev = millis();
float giroscAngZ, giroscAngZPrev;

void setup() {

  pinMode(motorIzquierdo1, OUTPUT);
  pinMode(motorIzquierdo2, OUTPUT);
  pinMode(motorderecho1, OUTPUT);
  pinMode(motorderecho2, OUTPUT);
  pinMode(motorIzquierdoPWM, OUTPUT);
  pinMode(motorderechooPWM, OUTPUT);

  Serial.begin(9600);
  Wire.begin();
  sensor.initialize();

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");

  delay(2000);
}

void loop() {

  digitalWrite(motorIzquierdo1, HIGH);
  digitalWrite(motorIzquierdo2, LOW);
  digitalWrite(motorderecho1, HIGH);
  digitalWrite(motorderecho2, LOW);



  // Cambiar valores de PWM cada medio segundo
  if (millis() >= tiempo+tiempoDeCambio ) {
    int temp = pwmIzquierdo;
    pwmIzquierdo = pwmDerecho;
    pwmDerecho = temp;
    tiempo=millis();
  }

  // Aplicar PWM a los motores izquierdos
  analogWrite(motorIzquierdoPWM, pwmIzquierdo);

  // Aplicar PWM a los motores derechos
  analogWrite(motorderechooPWM, pwmDerecho);

  //delay (1000);

    // Aplicar PWM a los motores izquierdos
  analogWrite(motorIzquierdoPWM, pwmIzquierdo2);

  // Aplicar PWM a los motores derechos
  analogWrite(motorderechooPWM, pwmDerecho2);


  // Leer la velocidad angular/ángulo del eje Z del giroscopio
  float dt = millis() - tiempoPrev;
  tiempoPrev = millis();

  int gx, gy, gz;
  sensor.getRotation(&gx, &gy, &gz);

  giroscAngZ = (gz / 131) * dt / 1000.0 + giroscAngZPrev;
  giroscAngZPrev = giroscAngZ;

  // Imprimir valores en el Monitor Serial
  Serial.print(pwmIzquierdo);
  Serial.print("\t");
  Serial.println(giroscAngZ);

  delay(10);
}

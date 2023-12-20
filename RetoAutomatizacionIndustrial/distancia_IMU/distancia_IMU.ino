#include <Wire.h>
#include <MPU6050.h>

int PinIN1 = 6;
int PinIN2 = 7;
int PinIN3 = 8;
int PinIN4 = 9;
int ENA = 5;
int ENB = 10;
int speed = 255;

MPU6050 mpu;

bool arranque=true;
float speed_acum=0.0,distance,time_start;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Error al conectar con el MPU6050");
    while (1);
  }
    // put your setup code here, to run once:
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  delay(4000);
  time_start=millis();
}

void loop() {
  if(millis()>time_start+1500)arranque=false;

    // put your main code here, to run repeatedly:
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  digitalWrite(ENA,speed);
  digitalWrite(ENB,speed);

  int16_t accX, accY, accZ;

  mpu.getMotion6(nullptr, nullptr, nullptr, &accX, &accY, &accZ);

  // Escala los valores a la unidad de gravedad (1g = 9.8 m/s^2)
  float accX_m_s2 = accX / 16384.0 * 9.8;
  float accY_m_s2 = accY / 16384.0 * 9.8;
  float accZ_m_s2 = accZ / 16384.0 * 9.8;

  // Resta la gravedad para obtener la aceleración lineal
  accZ_m_s2 = accZ_m_s2 - 9.8;

  // Establece el valor de referencia inicial (cero absoluto) al comienzo del programa
  static float initialAccZ = accZ_m_s2;
  
  // Calcula la variación en la aceleración con respecto al valor inicial
  float accelerationVariation = accZ_m_s2 - initialAccZ;

  // Asegúrate de que la variación no sea negativa
  accelerationVariation = max(0, accelerationVariation);

  // Calcula la distancia usando la fórmula MRUA
  float elapsedTime = 0.1;  // Tiempo transcurrido en segundos
  if(arranque){
    float speed = accelerationVariation * elapsedTime;
    speed_acum = speed_acum+speed;
  }
  distance = 0.8 * speed_acum * elapsedTime;
  static float totalDistance = 0.0;  // Variable para almacenar la distancia total
  totalDistance += distance;

  Serial.print("Distancia recorrida: ");
  Serial.print(distance);
  Serial.print(" metros | Distancia total: ");
  Serial.print(totalDistance);
  Serial.println(" metros");

  delay(50);  // Ajusta el intervalo según sea necesario
}
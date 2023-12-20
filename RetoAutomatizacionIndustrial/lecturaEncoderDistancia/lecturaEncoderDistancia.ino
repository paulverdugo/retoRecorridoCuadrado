#define encoderPinL 2
#define encoderPinR 3

//Valores para calculos
int resol = 20;
float diam = 0.066;
unsigned long time1 = millis();
unsigned long timem = 50;

//Encoder izquierdo 
float distL = 0.00;
volatile long encoderTicksL = 0;

//Encoder derecho 
float distR = 0.00;
volatile long encoderTicksR = 0; // Counts the number of encoder ticks

//Motor 1
int in1 = 6;
int in2 = 7;
int ena = 5;

//Motor 2
int in3 = 8;
int in4 = 9;
int enb = 10;

void setup() {
  pinMode(encoderPinR, INPUT);
  pinMode(encoderPinL,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinL), updateEncoderL, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinR), updateEncoderR, RISING);
  Serial.begin(9600);

   //Motor 1
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(ena,OUTPUT);

  //Motor 2
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enb,OUTPUT);
  
  //Velocidad 1
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,150);

  //Velocidad 2
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,150);
}

void loop() {
  unsigned long time2 = millis();
  if(time2-time1 >= timem)
  {
    distR=(encoderTicksR)/resol*(PI*diam);
    distL=(encoderTicksL)/resol*(PI*diam);
    float distf = (distR+distL)/2.0;

    Serial.print("Derecha: ");
    Serial.print(distL);
    Serial.print(" Izquierda: ");
    Serial.print(distR);
    Serial.print(" Distancia: ");
    Serial.println(distf);
  }
}

void updateEncoderR() {
  encoderTicksR++;
}
void updateEncoderL(){
  encoderTicksL++;
}
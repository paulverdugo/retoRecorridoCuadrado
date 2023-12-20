int PinIN1 = 6;
int PinIN2 = 7;
int PinIN3 = 8;
int PinIN4 = 9;
int ENA = 5;
int ENB = 10;
int speed = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  delay(1000);
}
  
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  digitalWrite(ENA,speed);
  digitalWrite(ENB,speed);
}


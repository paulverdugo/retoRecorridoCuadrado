// Define the pins for the encoder
const int encoder = 2;
// Variables for encoder and speed calculation
volatile long cont = 0;
unsigned long prevTime = 0;
const int encoderResolution = 40; // Number of pulses per revolution
const unsigned long sampleInterval = 50; // Time interval for speed calculation (in milliseconds)

void setup() {
  Serial.begin(9600);
  // Enable interrupt on digital pin 2
  pinMode(encoder,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder), updateEncoder, CHANGE);
}
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= sampleInterval) {
    // Calculate speed in RPM
    long pulse = cont/2.0; //modificable
    cont = 0.0;
    float elapsedTime = (float)(currentTime - prevTime) / 60000.0; // Convert to minutes
    float speed = (pulse / (float)encoderResolution) / elapsedTime; // revs per second
    Serial.println(speed);
    prevTime = currentTime;
  }
}
void updateEncoder(){
  // Update the pulse count based on encoder transitions
  cont++;
}
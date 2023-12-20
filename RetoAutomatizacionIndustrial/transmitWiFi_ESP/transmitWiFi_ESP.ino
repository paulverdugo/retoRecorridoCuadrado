#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // Siempre se debe incluir esta biblioteca después del resto
#include <SoftwareSerial.h>

int rx=14; //D5
int tx=12; //D6
SoftwareSerial Serial2(rx,tx);

char ssid[] = "paul (2)";
char pass[] = "nopassword"; // el password (contraseña) de la red WiFi.
int keyIndex = 0; // El key Index number de la red (solo para redes WEP)
WiFiClient client;
unsigned long myChannelNumber = 2329910; // número del canal creado en ThingSpeak (Channel ID)

const char * myWriteAPIKey = "V1UGCHOT3XP8R9YI";// API Key para escritura (write API key)
//obtenido de ThingSpeak (el que
// anotó en el bloc de notas)
int distancia;
void setup() {
  Serial.begin(9600); //Initialize serial
  Serial2.begin(9600); //Initialize serial
  while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
}
void loop(){
  // Conectar o reconectar a la WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Tratando de conectar con SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Conectando con red WiFi WPA/WPA2. Cambiar esta linea si se conecta a una
      // red abierta o WEP
      Serial.println("... intentando encontrar red especificada.. intentando nuevamente en 5 segundos... ");
      delay(5000); // Esperar 5 segundos antes de volver a verificar.
    }
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("\nConectado.");
  }
  // Escribir a ThingSpeak. Hay hasta 8 campos (variables) en un canal, lo que permite almacenar 8 datos
  // diferentes por canal. En este ejemplo se escribe en el campo (variable 1).
  if (Serial2.available() > 0) {
    distancia = Serial2.parseInt();
    Serial.println(distancia);
    int x = ThingSpeak.writeField(myChannelNumber, 1, distancia, myWriteAPIKey);
  
  if(x == 200){ // ThingSpeak devuelve 200 si la comunicación y registro del dato fue exitoso.
    Serial.println("Canal actualizado exitosamente.");
  }
  else{
    Serial.println("Problemas actualizando el canal. codigo de error HTTP " + String(x));
  }
  // cambiar el valor. Se comienza en 0 y llega a 99, al llegar a 99 se reinicia en 0 nuevamente.
  }
  //delay(20000); // Espera de 20 segundos para actualizar el canal nuevamente
}
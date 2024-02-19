
#include <ArduinoJson.h>
#include <ESP8266Firebase.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>    


const char* ssid = "TP-LINK_A65538";
const char* password = "86669443"; 

// dato sacado de firebase cultivo Realtime Database 
#define FIREBASE_HOST "cultivo-c71e7-default-rtdb.firebaseio.com"

Firebase firebase(FIREBASE_HOST);


WiFiServer server(80); 


static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;  

int ledRojo = D6;
int bombaRiego = D7;
int ledVerde = D5;
int sensorPin = A0;
int humedadSuelo;
int porcentajeSuelo;
int tiempoBomba;
int porcentajeUsuario;
int count;
char horarioActual;
unsigned long tiempoAnterior;


// Configuración del servidor NTP
const char* ntpServer = "0.south-america.pool.ntp.org";
const int gmtOffset_sec = 0;
const int daylightOffset_sec = - 10800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, daylightOffset_sec, 6000);  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);              
  pinMode(ledRojo, OUTPUT);
  pinMode(bombaRiego, OUTPUT);
  digitalWrite(bombaRiego, HIGH);
  tiempoAnterior = millis();
  
   
  WiFi.begin(ssid, password);           
  Serial.printf("\n\nConectando a la red: %s\n", WiFi.SSID().c_str());
   while (WiFi.status() != WL_CONNECTED) { 
    delay(100);
    Serial.print(".");
    count = count + 1;
    if (count == 30){
      Serial.println(".");
      count = 0;                    
    }
  }
  
         Serial.println("connected...yeey :)");
         Serial.println("");
         Serial.println("WiFi conectada");       
         humedadSuelo = analogRead(sensorPin);
         porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1 ; 
         timeClient.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
   if((millis() - tiempoAnterior) >= 500 ){
    
    unsigned long tiempoInicioLoop = millis();
    timeClient.update();
 // Imprimir la hora local en el formato hh:mm:ss
      Serial.print("este es el horario actual: "); 
      Serial.println(timeClient.getFormattedTime());
  
     firebase.setString("HoraActual", timeClient.getFormattedTime());
     porcentajeUsuario = firebase.getInt("porcentajeUsuario");

  tiempoBomba = firebase.getInt("tiempoBomba");
  int data2 = firebase.getInt("HumedadSuelo/setInt");
  int luz = firebase.getInt("luz");
   humedadSuelo = analogRead(sensorPin);
  porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1;
  firebase.setInt("HumedadSuelo/setInt", humedadSuelo);
  firebase.setInt("HumedadSuelo/porcentaje", porcentajeSuelo);
  
  
  
  
  Serial.print("soy porcentaje cargado por el usuario en numeros:");
  Serial.println(porcentajeUsuario);
  
  Serial.print("soy el tiempo de la bomba cargado por front: ");
  Serial.println(tiempoBomba);
  
 if( luz == 1 && humedadSuelo < porcentajeUsuario){
  
    tiempoBomba = firebase.getInt("tiempoBomba");
    firebase.setInt("estadoBomba", 1);
    firebase.setInt("luz",1);
 
   Serial.println("Me obligaron a regar!!!!");
    digitalWrite(ledRojo,HIGH);
    digitalWrite(ledVerde, LOW);
    digitalWrite(bombaRiego, LOW);
   Serial.print("soy el tiempo bomba antes en el forzado:");
   Serial.println(tiempoBomba);
    
   
    if((millis() - tiempoAnterior) >= tiempoBomba){
      
    
    
    Serial.print("soy el tiempo bomba en el forzado:");
    Serial.println(tiempoBomba);
    digitalWrite(ledRojo,LOW);
     digitalWrite(ledVerde, HIGH);
    digitalWrite(bombaRiego,HIGH);
    Serial.println("YA CORTE DE OBLIGADO");
    firebase.setInt("estadoBomba", 0);
    firebase.setInt("luz", 0);
    humedadSuelo = analogRead(sensorPin);
    porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1;
    firebase.setInt("HumedadSuelo/setInt", humedadSuelo);
    firebase.setInt("HumedadSuelo/porcentaje", porcentajeSuelo);
    tiempoAnterior = millis();
    }
   } else {
 
  if(humedadSuelo > porcentajeUsuario && luz == 0){
   
   digitalWrite(ledVerde, LOW);
   digitalWrite(ledRojo, HIGH);
   digitalWrite(bombaRiego, LOW);
   firebase.setInt("estadoBomba", 1);
   Serial.println("Me puse a regar!!!");
  
   
   if((millis() - tiempoAnterior) >= 100 ){
    
   
   humedadSuelo = analogRead(sensorPin);
   porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1;
   firebase.setInt("HumedadSuelo/setInt", humedadSuelo);
   firebase.setInt("HumedadSuelo/porcentaje", porcentajeSuelo);
   Serial.print("** Humedad: ");
   Serial.println(humedadSuelo);
 
   Serial.print("** Soy el porcentaje de Humedad: ");
   Serial.println(porcentajeSuelo);
   tiempoAnterior = millis();
   }
 }
   else {
  if((millis() - tiempoAnterior) >= 100){
      digitalWrite(bombaRiego, HIGH);
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRojo, LOW);
      firebase.setInt("estadoBomba",0); 
      tiempoAnterior = millis();
  }

 
     
     }
   }
  
  unsigned long tiempoFinLoop = millis();
  Serial.print("Tiempo entre iteraciones del loop: ");
  Serial.println(tiempoFinLoop - tiempoInicioLoop);
 
  tiempoAnterior = tiempoFinLoop;
   }
}

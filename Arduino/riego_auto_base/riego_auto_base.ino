

#include <ArduinoJson.h>

#include <ESP8266Firebase.h>



#include <ESP8266WiFi.h>    




const char* ssid = "Fibertel WiFi014 2.4GHz";
const char* password = "0042502821";  

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
int bombaRiego = D8;
int ledVerde = D5;
int sensorPin = A0;
int humedadSuelo;
int porcentajeSuelo;
int tiempoBomba;
int porcentajeUsuario;


void setup() {
  Serial.begin(115200);
 
  pinMode(ledVerde, OUTPUT);              
  pinMode(ledRojo, OUTPUT);
  pinMode(bombaRiego, OUTPUT);
  digitalWrite(bombaRiego, HIGH);
   
  WiFi.begin(ssid, password);           
  Serial.printf("\n\nConectando a la red: %s\n", WiFi.SSID().c_str());
   while (WiFi.status() != WL_CONNECTED) { 
    delay(100);
    Serial.print(".");                    
  }
  Serial.println("");
  Serial.println("WiFi conectada");       
    
  } 
 
void loop() 
{
  delay(500);
  
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
  
  Serial.print("Humedad: ");
  Serial.println(humedadSuelo);
 
  Serial.print("Soy el porcentaje de Humedad: ");
  Serial.println(porcentajeSuelo);
  
  Serial.print("desde la base la humedad es:\t\t");
  Serial.println(data2);
  
  
  Serial.print("soy luz: ");
  Serial.println(luz);
  
  if( luz == 1 && humedadSuelo < porcentajeUsuario){
    firebase.setInt("estadoBomba", 1);
    firebase.setInt("luz",1);
    Serial.println("Me obligaron a regar!!!!");
    digitalWrite(ledRojo,HIGH);
    digitalWrite(bombaRiego, LOW);
    Serial.print("soy el tiempo bomba antes en el forzado:");
    Serial.println(tiempoBomba);
    delay(tiempoBomba);
    Serial.print("soy el tiempo bomba en el forzado:");
    Serial.println(tiempoBomba);
    digitalWrite(ledRojo,LOW);
    digitalWrite(bombaRiego,HIGH);
    Serial.println("YA CORTE DE OBLIGADO");
    firebase.setInt("estadoBomba", 0);
    firebase.setInt("luz", 0);
    
   } 
  
 
if(humedadSuelo > porcentajeUsuario && luz == 0){
   
   digitalWrite(ledVerde, LOW);
   digitalWrite(ledRojo, HIGH);
   digitalWrite(bombaRiego, LOW);
   firebase.setInt("estadoBomba", 1);
   Serial.println("Me puse a regar!!!");
  
   delay(100);
   
   
 } 
 
 if(humedadSuelo < porcentajeUsuario && luz == 0){
     digitalWrite(bombaRiego, HIGH);
     digitalWrite(ledVerde, HIGH);
     digitalWrite(ledRojo, LOW);
     firebase.setInt("estadoBomba",0);
     delay(100);
     }
 
 
    
  }
   
  
  


 

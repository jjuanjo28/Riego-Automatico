

#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266Firebase.h>
#include <GFButton.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>    




//const char* ssid = "Fibertel WiFi014 2.4GHz";
//const char* password = "0042502821";  

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
char horarioActual;
GFButton buttonOn(D4);

// Configuración del servidor NTP
const char* ntpServer = "0.south-america.pool.ntp.org";
const int gmtOffset_sec = 0;
const int daylightOffset_sec = - 10800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, daylightOffset_sec, 6000);

void setup() {

  Serial.begin(115200);
    pinMode(ledVerde, OUTPUT);              
  pinMode(ledRojo, OUTPUT);
  pinMode(bombaRiego, OUTPUT);
  digitalWrite(bombaRiego, HIGH);
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
   WiFiManager wm;
   wm.setConnectTimeout(240);
 // reinicioTicker.attach(intervaloReinicio, reiniciarESP32);
 // wm.resetSettings();
  if (buttonOn.isPressed()) {
    Serial.println(F("On button is pressed"));
   
  }
     bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP_ESP32","12345678"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
          //if you get here you have connected to the WiFi    
         Serial.println("connected...yeey :)");
         Serial.println("");
         Serial.println("WiFi conectada");       
         humedadSuelo = analogRead(sensorPin);
         porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1 ; 
         timeClient.begin();
        }

  

  
   
 // WiFi.begin(ssid, password);  <----- ESTE LO ANULE RECIEN         
  Serial.printf("\n\nConectando a la red: %s\n", WiFi.SSID().c_str());
   
  } 
 
void loop() 
{
  delay(500);
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
  
  //Serial.print("Humedad: ");
  //Serial.println(humedadSuelo);
 
  //Serial.print("Soy el porcentaje de Humedad: ");
  //Serial.println(porcentajeSuelo);
  
 // Serial.print("desde la base la humedad es:\t\t");
 // Serial.println(data2);
  // Serial.print("soy el tiempoActual: ");
 // Serial.println(tiempoActual);
 // Serial.print("soy el tiempoUltimaLectura: ");
 // Serial.println(tiempoUltimaLectura);
 // Serial.print("soy el IntervaloLectura: ");
 // Serial.println(intervaloLectura);
  
  Serial.print("soy luz: ");
  Serial.println(luz);

   if (buttonOn.isPressed()) {
    Serial.println(F("On button is pressed"));
   }
   
  

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
    
    delay(tiempoBomba);
    
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
    
   } else {
 
  if(humedadSuelo > porcentajeUsuario && luz == 0){
   
   digitalWrite(ledVerde, LOW);
   digitalWrite(ledRojo, HIGH);
   digitalWrite(bombaRiego, LOW);
   firebase.setInt("estadoBomba", 1);
   Serial.println("Me puse a regar!!!");
  
   delay(100);
   humedadSuelo = analogRead(sensorPin);
   porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1;
   firebase.setInt("HumedadSuelo/setInt", humedadSuelo);
   firebase.setInt("HumedadSuelo/porcentaje", porcentajeSuelo);
   Serial.print("** Humedad: ");
   Serial.println(humedadSuelo);
 
   Serial.print("** Soy el porcentaje de Humedad: ");
   Serial.println(porcentajeSuelo);
   
   
 }
   else {
 digitalWrite(bombaRiego, HIGH);
 digitalWrite(ledVerde, HIGH);
 digitalWrite(ledRojo, LOW);
 firebase.setInt("estadoBomba",0);
 delay(100);
     
     }
   }
 
  
 

 
  unsigned long tiempoFinLoop = millis();
  Serial.print("Tiempo entre iteraciones del loop: ");
  Serial.println(tiempoFinLoop - tiempoInicioLoop);
   WiFi.forceSleepBegin();
delay(10000);
Serial.printf("Estado del wifi: %d", WiFi.status());
Serial.printf(" Desconectado \n");
WiFi.forceSleepWake();
delay(10000);
Serial.printf("Estado del wifi: %d", WiFi.status());
Serial.printf(" Conectado \n"); 
  }
   
  
  


 



#include <ArduinoJson.h>

#include <ESP8266Firebase.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>    


#include <GFButton.h>


//const char* ssid = "JJ tel";
//const char* password = "12345678";  

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
 
GFButton buttonOn(D3);               
int ledRojo = D6;
int bombaRiego = D7;
int ledVerde = D5;
int sensorPin = A0;
int humedadSuelo;
int porcentajeSuelo;
int tiempoBomba;
int porcentajeUsuario;
int count;
char horarioNow;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.south-america.pool.ntp.org", - 10800);

unsigned long tiempoUltimaLectura = 0;
unsigned long intervaloLectura = 3600000; 

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
    count = count + 1;
    if (count == 30){
      Serial.println(".");
      count = 0;                    
    }
  }
  Serial.println("");
  Serial.println("WiFi conectada");       
  humedadSuelo = analogRead(sensorPin);
  porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1 ; 
  } 
 
void loop() 
{
  delay(500);
  unsigned long tiempoInicioLoop = millis();
  timeClient.update();
  unsigned long tiempoActual = timeClient.getEpochTime() * 1000;  // Multiplicar por 1000 para convertir segundos a milisegundos
 
  firebase.setString("HoraActual", timeClient.getFormattedTime());
  porcentajeUsuario = firebase.getInt("porcentajeUsuario");
  
  tiempoBomba = firebase.getInt("tiempoBomba");
  int data2 = firebase.getInt("HumedadSuelo/setInt");
  int luz = firebase.getInt("luz");
  
  
  
  
  
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

 if (tiempoActual - tiempoUltimaLectura >= intervaloLectura) {
  Serial.print("** soy el tiempoActual: ");
  Serial.println(tiempoActual);
  Serial.print("** soy el tiempoUltimaLectura: ");
  Serial.println(tiempoUltimaLectura);
  Serial.print("** soy el IntervaloLectura: ");
  Serial.println(intervaloLectura);
  
  humedadSuelo = analogRead(sensorPin);
  porcentajeSuelo = ((humedadSuelo * 100) /1024 - 100) * -1;
  firebase.setInt("HumedadSuelo/setInt", humedadSuelo);
  firebase.setInt("HumedadSuelo/porcentaje", porcentajeSuelo);
  Serial.print("Aca me puse a verificar cada una hora!!!!");
  tiempoUltimaLectura = tiempoActual;
   Serial.print("soy el nuevo tiempoUltimaLectura: ");
  Serial.println(tiempoUltimaLectura);
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
  
  if( luz == 1 && humedadSuelo < porcentajeUsuario){
    
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
    
   } 
  
  if (buttonOn.isPressed()) {
    Serial.println("On button is pressed");
    }

 
  unsigned long tiempoFinLoop = millis();
  Serial.print("Tiempo entre iteraciones del loop: ");
  Serial.println(tiempoFinLoop - tiempoInicioLoop);
    
  }
   
  
  


 

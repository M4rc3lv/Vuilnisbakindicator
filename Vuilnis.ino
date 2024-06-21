#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define WIFI_SSID "Wifi ISSD"
#define WIFI_PASS "Wifi wachtwoord"

#define LEDRING D3

Adafruit_NeoPixel Led(3, LEDRING, NEO_GRB + NEO_KHZ800);
ESP8266WiFiMulti D1Mini;

void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 D1Mini.addAP(WIFI_SSID, WIFI_PASS); 
 Serial.println();
 Serial.println("Wifi OK");
 
 Led.begin();
}

void loop() {
 if(D1Mini.run()== WL_CONNECTED){ 
  Serial.println("Vraag kleur aan PHP...");
  int Kleur = GetKleurFromPHP();
  if(Kleur==Led.Color(128,128,128)) {
   // PHP error (hij werd dan wit)
   for(int i=0; i<3; i++) Led.setPixelColor(i,Led.Color(200,0,0));
   Led.show();
   delay(10000);
   return;
  }
  Serial.print("Kleur=");Serial.println(Kleur);
  
  for(int j=0; j<20*60; j++) {
   for(int i=0; i<3; i++) Led.setPixelColor(i,Kleur);
   Led.show();
   delay(1000);   
  }
 }
}

void Error() {
 for(int j=0; j<30; j++) {
  for(int i=0; i<3; i++) Led.setPixelColor(i,Led.Color(200,0,0)); 
  Led.show();
  delay(500); 
  for(int i=0; i<3; i++) Led.setPixelColor(i,Led.Color(0,0,0)); 
  Led.show();
  delay(500); 
 }
}

int GetKleurFromPHP() {
 if (WiFi.status() == WL_CONNECTED) {
  WiFiClientSecure client;
  client.setInsecure(); 
  //client.connect("marcelv.net",443);
  String Url(String("https://marcelv.net/IoT/vuilnis.php"));
  
  HTTPClient http;   
  http.begin(client,Url);  
  int code=http.GET();
  Serial.print("HttpCode=");Serial.println(code);
  String K = http.getString(); 
  Serial.println(K);
  http.end(); 
  if(K.startsWith("Groen"))
   return Led.Color(0,255,0); 
  else if(K.startsWith("Geel"))
   return Led.Color(225,180,0); 
  else if(K.startsWith("Blauw"))
   return Led.Color(10,20,255); 
  else if(K.startsWith("Zwart"))
   return Led.Color(0,0,0); 
  else
   return Led.Color(128,128,128); // Software (PHP) error 
 }
 return Led.Color(225,0,0); // Geen Wifi error
}

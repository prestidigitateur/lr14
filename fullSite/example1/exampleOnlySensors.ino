#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//датчики
#define sensorD3 D3
#define sensorD5 D5
#define sensorA0 A0


const char* ssid = "network-name";
const char* password = "pass-to-network";

ESP8266WebServer server(80);

void setup(void){
//выставляем режим пинов
  pinMode(sensorD3, INPUT);
  pinMode(sensorD5, INPUT);
  pinMode(sensorA0, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //определяем что будет запускаться при переходе на страницы
  server.on("/", getSensorsHtml);
  server.on("/sensors.html", getSensorsHtml);
 
  //запуск сервера 
  server.begin();
  Serial.println("HTTP server started");
}
//https://prestidigitateur.github.io/htmlToEsp/
void getSensorsHtml(){
  String sensorString = "<!DOCTYPE html> <html> <head> <title>Датчики</title> <meta charset=\"utf-8\"> </head>";
  sensorString += " <body style=\"zoom: 300%\"> <h1>Датчики</h1> <p> <b>Датчик на D3:</b> "+(String)digitalRead(sensorD3)+" <br>";
  sensorString += " <b>Датчик на D5:</b> "+(String)digitalRead(sensorD5)+" <br> <b>Датчик на A0:</b> "+(String)analogRead(sensorA0)+" <br>";
  sensorString += " </p> <a href=\"sensors.html\"><button>update</button></a></body> </html>";

  server.send(200, "text/html", sensorString);
}

//основной цикл  
void loop(void){
  server.handleClient();
} 

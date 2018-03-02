#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//rgb светодиод
#define rgbRed D6
#define rgbBlue D7
#define rgbGreen D8

const char* ssid = "network-name";
const char* password = "pass-to-network";

ESP8266WebServer server(80);

void setup(void){
//выставляем режим пинов

  pinMode(rgbRed, OUTPUT);
  pinMode(rgbBlue, OUTPUT);
  pinMode(rgbGreen, OUTPUT);

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
  server.on("/", [](){getColorsHtml("gray");});
  server.on("/colors.html", [](){getColorsHtml("gray");});
    server.on("/red", [](){getColorsHtml("red");});
    server.on("/blue", [](){getColorsHtml("blue");});
    server.on("/green", [](){getColorsHtml("green");});
 
  //запуск сервера 
  server.begin();
  Serial.println("HTTP server started");
}
//https://prestidigitateur.github.io/htmlToEsp/
void getColorsHtml(String state){
  
   if(state == "green")
    digitalWrite(rgbGreen, HIGH);
  else
    digitalWrite(rgbGreen, LOW);

  if(state == "blue")
    digitalWrite(rgbBlue, HIGH);
  else
    digitalWrite(rgbBlue, LOW);

  if(state == "red")
    digitalWrite(rgbRed, HIGH);
  else
    digitalWrite(rgbRed, LOW);
    
  String colorString = "<!DOCTYPE html> <html> <head> <title>Цвета</title> <meta charset=\"utf-8\"> </head>";
  colorString += " <body style=\"zoom: 300%\"> <h1>Цвета</h1> <h1 style=\"color: "+state+"\">⬤</h1>";
  colorString += " <a href=\"/red\"><button style=\"color: red\">red</button></a> <a href=\"/blue\"><button style=\"color: blue\">blue</button></a> <a href=\"/green\"><button style=\"color: green\">green</button></a>";
  colorString += " </body> </html>";
  server.send(200, "text/html", colorString);
}

//основной цикл  
void loop(void){
  server.handleClient();
} 

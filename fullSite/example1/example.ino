#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//светодиоды
#define ledD0 D0
#define ledD4 D4
//датчики
#define sensorD3 D3
#define sensorD5 D5
#define sensorA0 A0
//rgb светодиод
#define rgbRed D6
#define rgbBlue D7
#define rgbGreen D8

const char* ssid = "network-name";
const char* password = "pass-to-network";

ESP8266WebServer server(80);

void setup(void){
//выставляем режим пинов
  pinMode(ledD0, OUTPUT);
  pinMode(ledD4, OUTPUT);
  pinMode(sensorD3, INPUT);
  pinMode(sensorD5, INPUT);
  pinMode(sensorA0, INPUT);
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
  server.on("/leds.html", [](){getLedsHtml("");});
    server.on("/on0", [](){getLedsHtml("on0");}); 
    server.on("/off0", [](){getLedsHtml("off0");});
    server.on("/on4", [](){getLedsHtml("on4");});
    server.on("/off4", [](){getLedsHtml("off4");});
  server.on("/sensors.html", getSensorsHtml);
 
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
  colorString += " <hr> <a href=\"leds.html\">Светодиоды</a> <a href=\"sensors.html\">Датчики</a> </body> </html>";
  server.send(200, "text/html", colorString);
}

void getLedsHtml(String state){

  if(state == "on0")
   digitalWrite(ledD0, LOW);
  if(state == "off0")
   digitalWrite(ledD0, HIGH);
  if(state == "on4")
   digitalWrite(ledD4, LOW);
  if(state == "off4")
   digitalWrite(ledD4, HIGH);
  
  String ledString = "<!DOCTYPE html> <html> <head> <title>Светодиоды</title> <meta charset=\"utf-8\"> </head>";
  ledString += " <body style=\"zoom: 300%\"> <h1>Светодиоды</h1> <p> <b>Светодиод D0:</b> <text>"+(String)!digitalRead(ledD0)+"</text>";
  ledString += " <a href=\"/on0\"><button>on</button></a> <a href=\"/off0\"><button>off</button></a> <p>";
  ledString += " <b>Светодиод D4:</b> <text>"+(String)!digitalRead(ledD4)+"</text> <a href=\"/on4\"><button>on</button></a> <a href=\"/off4\"><button>off</button></a> </p>";
  ledString += " <hr> <a href=\"sensors.html\">Датчики</a> <a href=\"colors.html\">Цвета</a> </body> </html>";
  server.send(200, "text/html", ledString);
}
void getSensorsHtml(){
  String sensorString = "<!DOCTYPE html> <html> <head> <title>Датчики</title> <meta charset=\"utf-8\"> </head>";
  sensorString += " <body style=\"zoom: 300%\"> <h1>Датчики</h1> <p> <b>Датчик на D3:</b> "+(String)digitalRead(sensorD3)+" <br>";
  sensorString += " <b>Датчик на D5:</b> "+(String)digitalRead(sensorD5)+" <br> <b>Датчик на A0:</b> "+(String)analogRead(sensorA0)+" <br>";
  sensorString += " </p> <a href=\"sensors.html\"><button>update</button></a> <hr> <a href=\"leds.html\">Светодиоды</a> <a href=\"colors.html\">Цвета</a> </body> </html>";

  server.send(200, "text/html", sensorString);
}

//основной цикл  
void loop(void){
  server.handleClient();
} 

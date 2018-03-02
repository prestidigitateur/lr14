#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//светодиоды
#define ledD0 D0
#define ledD4 D4


const char* ssid = "network-name";
const char* password = "pass-to-network";

ESP8266WebServer server(80);

void setup(void){
//выставляем режим пинов
  pinMode(ledD0, OUTPUT);
  pinMode(ledD4, OUTPUT);


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
  server.on("/", [](){getLedsHtml("");});
  server.on("/leds.html", [](){getLedsHtml("");});
    server.on("/on0", [](){getLedsHtml("on0");}); 
    server.on("/off0", [](){getLedsHtml("off0");});
    server.on("/on4", [](){getLedsHtml("on4");});
    server.on("/off4", [](){getLedsHtml("off4");});

 
  //запуск сервера 
  server.begin();
  Serial.println("HTTP server started");
}
//https://prestidigitateur.github.io/htmlToEsp/
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
  ledString += " </body> </html>";
  server.send(200, "text/html", ledString);
}

//основной цикл  
void loop(void){
  server.handleClient();
} 

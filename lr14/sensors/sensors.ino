#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHTesp.h>

DHTesp dht;
#define DHTPin D7
#define LightPin A0
int temp = 0;
int hum = 0;
int light = 0;

const char *ssid = "SRC_WIFI";
const char *password = "seminar1";
IPAddress habAddress(192,168,43,9);
const int port = 8888;

  
ESP8266WebServer webServer(80);
String link = "sensors";

void sendHtmlPage(){

  String myHtml = "<!DOCTYPE html> <html> <head> <title>Внешние датчики</title> <meta charset=\"utf-8\"> </head> <body style=\"zoom: 300%; padding: 10px\"> <h1>Внешние датчики</h1> <h4>Температура: "+String(temp)+"</h4> <h4>Влажность: "+String(hum)+"</h4> <h4>Освещённость: "+String(light)+"</h4> <hr> <a href=\"sensors\"><button>Обновить</button></a> </body> </html>";
  webServer.send(200, "text/html", myHtml);
}

void sendInfo(){
    String msg = link+":"+temp+":"+hum+":"+light;
    
    WiFiClient client;   
    if(client.connect(habAddress, port)){
      client.println(msg);
      client.println();
      client.flush();
      client.stop();
      Serial.println(String("Send: "+ msg));
    }
    else{
      Serial.println("Connection failed");
    } 
}
void setup() {
  delay(1000);
  Serial.begin(115200);

  Serial.println();
  Serial.print("Подключение к ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
    
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println();
  Serial.println("Подключено!");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  webServer.on("/"+link, sendHtmlPage);
  webServer.onNotFound( [](){ webServer.send(200, "text/html", "<a style=\"zoom: 300%;\" href=\"/"+link+"\">Go to "+link+".</a>");  });
  
  webServer.begin();
  Serial.println("HTTP server started");
  
  dht.setup(DHTPin); 
}

void loop(){
  webServer.handleClient();

  delay(dht.getMinimumSamplingPeriod());

  int h = dht.getHumidity();
  if(h != hum){
    hum = h; sendInfo();
  }
  int t = dht.getTemperature();
    if(t != temp){
    temp = t; sendInfo();
  }
  int l = analogRead(LightPin);
    if(l/10 != light/10){
    light = l; sendInfo();
  }
}

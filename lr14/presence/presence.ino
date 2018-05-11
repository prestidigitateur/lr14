#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define Sensor_1 D7
#define Sensor_2 D8
bool sensor1 = 0;
bool sensor2 = 0;
String history;

const char *ssid = "SRC_WIFI";
const char *password = "seminar1";
IPAddress habAddress(192,168,43,9);
const int port = 8888;

  
ESP8266WebServer webServer(80);
String link = "presence";

void sendHtmlPage(){
  
  String myHtml = "<!DOCTYPE html> <html> <head> <title>Присутствие</title> <meta charset=\"utf-8\"> </head> <body style=\"zoom: 300%; padding: 10px\"> <h1>Охранная система</h1> <p> <h3>Присутствие 1 комната</h3> <text>" + String(sensor1?"В комнате кто-то есть":"В комнате никого") + "</text> </p> <p> <h3>Присутствие 2 комната</h3> <text>" + String(sensor1?"В комнате кто-то есть":"В комнате никого") + "</text> </p> <hr> <div style=\"width: 100%\">"+history+"</div> <hr> <a href=\"presence\"><button>Обновить</button></a> </body> </html>";
  webServer.send(200, "text/html", myHtml);
}

void sendInfo(){
    String msg = link+":"+sensor1+":"+sensor2;
    
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
}

void loop(){
  webServer.handleClient();

  bool s1 = digitalRead(Sensor_1);
  if(sensor1 != s1){
    sensor1 = s1;
    history += String(millis())+" : room 1 : " + sensor1+"<br>"; sendInfo();}

   bool s2 = digitalRead(Sensor_2);
  if(sensor2 != s2){
    sensor2 = s2;
    history += String(millis())+" : room 2 : " + sensor2+"<br>"; sendInfo();} 
}  



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


const char *ssid = "SRC_WIFI";
const char *password = "seminar1";
IPAddress habAddress(192,168,43,9);
const int port = 8888;

int lamp1 = 0;
int lamp2 = 0;
  
ESP8266WebServer webServer(80);
WiFiServer server(port);
String link = "lighting";

String n2s(int col){
  String s = String(col, HEX);
  
  if(col < 0x10)
    s = "00000"+s;
  else if(col < 0x100)
    s = "0000"+s;
  else if(col < 0x1000)
    s = "000"+s; 
  else if(col < 0x10000)
    s = "00"+s; 
  else if(col < 0x100000)
    s = "0"+s; 
 
  return s;
}

void sendHtmlPage(){
  for(int i=0; i<webServer.args(); i++){
    if(webServer.argName(i) == "lamp1"){ 
      String s = webServer.arg(i).substring(1);
      Serial.println(s);
      lamp1 = strtoul(s.c_str(),0,16);
      Serial.println(lamp1);

      int red = (lamp1 & 0xFF0000) >> 24;
      int green = (lamp1 & 0x00FF00) >> 16;
      int blue = (lamp1 & 0x0000FF) >> 8;

      analogWrite(D2, red*4);
      analogWrite(D3, green*4);
      analogWrite(D4, blue*4);      
    }
    if(webServer.argName(i) == "lamp2"){ 
      String s = webServer.arg(i).substring(1);
      Serial.println(s);
      lamp2 = strtoul(s.c_str(),0,16);
      Serial.println(lamp2);
      
      int red = (lamp2 & 0xFF0000) >> 24;
      int green = (lamp2 & 0x00FF00) >> 16;
      int blue = (lamp2 & 0x0000FF) >> 8;

      analogWrite(D6, red*4);
      analogWrite(D7, green*4);
      analogWrite(D8, blue*4); 
    }
  }

  
  sendInfo();
  String myHtml = "<!DOCTYPE html> <html> <head> <title>Управление освещением</title> <meta charset=\"utf-8\"> </head> <body style=\"zoom: 300%; padding: 10px\"> <form action=\"lighting\" method=\"post\"> <h1>Управление освещением</h1> <p> <label>Текущее освещение 1 лампы</label><br> <input name=\"lamp1\" type=\"color\" value=\"#"+n2s(lamp1)+"\"/> </p> <p> <label>Текущее освещение 2 лампы</label><br> <input name=\"lamp2\" type=\"color\" value=\"#"+n2s(lamp2)+"\"/> </p> <hr> <button type=\"submit\">Выставить</button> <a href=\"lighting\"><button type=\"button\">Обновить</button></a> </form> </body> </html>";
  webServer.send(200, "text/html", myHtml);
}

void sendInfo(){
    String msg = link+":lamp1:"+lamp1;
    
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

    String msg = link+":lamp2:"+lamp2;
    
    WiFiClient client;   
    if(client.connect(hab, port)){
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
  server.begin();
  Serial.println("HTTP server started"); 

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

void loop(){
  WiFiClient client = server.available();
 
  if(client){
  Serial.println("new client");
  while(client && !client.available()){
    webServer.handleClient();
  }

  String pref = client.readStringUntil(':');
  Serial.println(pref);

  if(pref == link)
  {
    pref = client.readStringUntil(':');
    int color = client.readStringUntil('\r').toInt();

    int red = (color & 0xFF0000) >> 24;
    int green = (color & 0x00FF00) >> 16;
    int blue = (color & 0x0000FF) >> 8;
    
    if(pref == "lamp1")
    {
      analogWrite(D2, red*4);
      analogWrite(D3, green*4);
      analogWrite(D4, blue*4);
      lamp1 = color;
    }
    if(pref == "lamp2")
    {
      analogWrite(D6, red*4);
      analogWrite(D7, green*4);
      analogWrite(D8, blue*4);
      lamp2 = color;
    }

  }
  delay(1);
  client.stop();
 }
  webServer.handleClient();
}  


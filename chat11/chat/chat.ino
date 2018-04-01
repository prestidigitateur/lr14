#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "network-name";
const char *password = "pass-to-network";
const int port = 8888;
  
ESP8266WebServer webServer(80);
WiFiServer server(port);
String messages;

  IPAddress addr;
  String ip;
  String msg;
  

void handleRoot(){
  Serial.println("---");
  Serial.println(webServer.args());
  
  for(int i=0; i<webServer.args(); i++){
    if(webServer.argName(i) == "ip"){ 
      ip = webServer.arg(i);
      addr.fromString(ip);
      Serial.println(ip);
    }
    if(webServer.argName(i) == "msgToSend"){
      msg = webServer.arg(i);
      Serial.println(msg);
    }    
  }
  Serial.println("---");

  String info;
  if(ip.length()>0 && msg.length()>0){
    WiFiClient client;
    if(client.connect(addr, port)){
      delay(500);
      client.print(msg);
      client.write('\r');
      delay(1);
      client.stop();
  
      info = "Send " + ip + " "+ msg;
      Serial.println(info);
      msg = "";
    }
    else{
      info = "Connection failed " + ip;
      Serial.println(info);
    }
  }
  
  String myHtml = "<!DOCTYPE html><html><head>";
   myHtml += "<title>Чатик 😜</title><meta charset=\"utf-8\"></head>";
   myHtml += "<body style=\"zoom: 300%;\" >"+info+"<br><br><hr>"+messages+"<hr>";
   myHtml += "<a href=\"/chat\"><button>Обновить историю сообщений</button></a>";
   myHtml += "<br><br><form method=\"post\" action=\"/chat\">";
   myHtml += "Введите IP:<input type=\"text\" name=\"ip\" style=\"width: 100%\" value=\""+ip+"\"></input>";
   myHtml += "Введите cообщение:<textarea type=\"text\" name=\"msgToSend\" style=\"width: 100%\">"+msg+"</textarea>";
   myHtml += "<button type=\"submit\">Отправить сообщение</button>";
   myHtml += "</form></body></html>";
  webServer.send(200, "text/html", myHtml);
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webServer.on("/chat", handleRoot);
  webServer.onNotFound( [](){
    webServer.send(200, "text/html", "<a style=\"zoom: 300%;\" href=\"/chat\">Go to chat.</a>");
  });
  webServer.begin();
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  WiFiClient client = server.available();
 
  if(client){
  Serial.println("new client");
  while(client && !client.available()){
    webServer.handleClient();
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  messages += req + "<br>";
  delay(1);
  client.stop();
 }
  webServer.handleClient();
}

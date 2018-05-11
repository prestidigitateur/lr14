#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define MotorSpeed  5  
#define MotorDir    2

int curtPos = 0;
const char *ssid = "SRC_WIFI";
const char *password = "seminar1";
IPAddress habAddress(192,168,43,9);
const int port = 8888;
  
ESP8266WebServer webServer(80);
WiFiServer server(port);
String link = "curtains";

void sendHtmlPage(){
  int pos = 0;
  for(int i=0; i<webServer.args(); i++){
    if(webServer.argName(i) == "range"){ 
      pos = webServer.arg(i).toInt();
      Serial.println(pos);    
        
      if(pos > curtPos)
        digitalWrite(MotorDir,HIGH);
      else
        digitalWrite(MotorDir,LOW);
      
      curtPos = pos;  
      analogWrite(MotorSpeed,800);
      pos = abs(pos-curtPos) * 1000;
      
      int logTimer = millis();
      while (logTimer+pos > millis())
        yield();
  }
  }
  
  
  sendInfo();
  String myHtml = "<!DOCTYPE html> <html> <head> <title>Шторы</title> <meta charset=\"utf-8\"> </head> <body style=\"zoom: 300%; padding: 10px\"> <form action="+link+" method=\"post\"> <h1>Шторы</h1> <p> <label>Текущее положение</label><br> <progress value="+String(curtPos)+" max=\"10\" /> </p> <p> <label>Выставить положение в</label><br> <input name=\"range\" type=\"range\" max=\"10\" value="+String(curtPos)+" /> </p> <hr> <button type=\"submit\">Выставить</button> <a href="+link+"><button type=\"button\">Обновить</button></a> </form> </body> </html>";
  webServer.send(200, "text/html", myHtml);
}

void sendInfo(){
    String msg = link+":"+curtPos;
    
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
  server.begin();
  Serial.println("HTTP server started"); 

  pinMode(MotorSpeed, OUTPUT);
  pinMode(MotorDir, OUTPUT);
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
    int val = ((String)client.readStringUntil('\r')).toInt();
    Serial.println(val);
    
    if(val > curtPos)
      digitalWrite(MotorDir,HIGH);
    else
      digitalWrite(MotorDir,LOW);

    curtPos = val;
    analogWrite(MotorSpeed,800);
    val = abs(val-curtPos) * 1000;
    
    int logTimer = millis();
    while (logTimer+val > millis())
      yield();
    

  }
  delay(1);
  client.stop();
 }
  webServer.handleClient();
}  


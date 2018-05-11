#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//curtains
int curtPos = 0;
//lighting
int lamp1 = 0;
int lamp2 = 0;
//presence
bool sensor1 = 0;
bool sensor2 = 0;
String history;
//sensors
int temp = 0;
int hum = 0;
int light = 0;


const char *ssid = "SRC_WIFI";
const char *password = "seminar1";
const int port = 8888;

IPAddress curtainsAddress(192,168,43,244);
IPAddress lightingAddress(192,168,43,244);
  
ESP8266WebServer webServer(80);
WiFiServer server(port);
String link = "hab";

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

void sendInfo(IPAddress adr, String pref, String message){
    String msg = pref+":"+message;
    
    WiFiClient client;   
    if(client.connect(adr, port)){
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

void sendHtmlPage()
{
  for(int i=0; i<webServer.args(); i++)
  {
    if(webServer.argName(i) == "range")
    { 
      curtPos = webServer.arg(i).toInt();
      sendInfo(curtainsAddress, "curtains", String(curtPos));
    }
    else if(webServer.argName(i) == "lamp1")
    {
      String s = webServer.arg(i).substring(1);
      lamp1 = strtoul(s.c_str(),0,16);
      sendInfo(lightingAddress, "lighting", String(lamp1));
    }
    else if(webServer.argName(i) == "lamp2")
    {
      String s = webServer.arg(i).substring(1);
      lamp2 = strtoul(s.c_str(),0,16);
      sendInfo(lightingAddress, "lighting", String(lamp2));
    }    
  }
  
	String myHtml = "<!DOCTYPE html> <html> <head> <title>Умный дом</title> <meta charset=\"utf-8\">";
	myHtml += " <style type=\"text/css\"> html{width: 100vw;} #tab-1, #tab-2, #tab-3, #tab-4 {display: none;} #container input + label{padding: 0.2em; border-radius: 0.5em; background: #fff0f5;} #container input:checked + label { background: #ffbdd3; color: #444; position: relative; z-index: 6; } #content { background: #fff0f5; border-radius: 0.5em; min-height: 20em; position: relative; top: 1em; width: 100%; z-index: 5; } #content div { opacity: 0; padding: 10px; position: absolute; z-index: -100; } #container input#tab-1:checked ~ #content #content-1, #container input#tab-2:checked ~ #content #content-2, #container input#tab-3:checked ~ #content #content-3, #container input#tab-4:checked ~ #content #content-4 { opacity: 1; z-index: 100; } </style>";
	myHtml += " </head> <body style=\"zoom: 250%; padding: 10px\"> <div id=\"container\"> <input id=\"tab-1\" type=\"radio\" name=\"tab-group\" checked=\"checked\" /> <label for=\"tab-1\">Шторы</label> <input id=\"tab-2\" type=\"radio\" name=\"tab-group\" /> <label for=\"tab-2\">Освещение</label> <input id=\"tab-3\" type=\"radio\" name=\"tab-group\" /> <label for=\"tab-3\">Охрана</label> <input id=\"tab-4\" type=\"radio\" name=\"tab-group\" /> <label for=\"tab-4\">Внешние</label>";
	myHtml += " <div id=\"content\"> <div id=\"content-1\"> <form action=\"hab\" method=\"post\"> <h1>Управление шторами</h1> <p> <label>Текущее положение</label><br> <progress value=\""+String(curtPos)+"\" max=\"10\" /> </p> <p> <label>Выставить положение в</label><br> <input name=\"range\" type=\"range\" max=\"10\" value=\""+String(curtPos)+"\" /> </p> <hr> <button type=\"submit\">Выставить</button> <a href=\"hab\"><button type=\"button\">Обновить</button></a> </form> </div>";
	myHtml += "<div id=\"content-2\"> <form action=\"hab\" method=\"post\"> <h1>Управление освещением</h1> <p> <label>Текущее освещение 1 лампы</label><br> <input name=\"lamp1\" type=\"color\" value=\"#"+n2s(lamp1)+"\"/> </p> <p> <label>Текущее освещение 2 лампы</label><br> <input name=\"lamp2\" type=\"color\" value=\"#"+n2s(lamp2)+"\"/> </p> <hr> <button type=\"submit\">Выставить</button> <a href=\"hab\"><button type=\"button\">Обновить</button></a> </form> </div>";
	myHtml += "<div id=\"content-3\"> <h1>Охранная система</h1> <p> <h3>Присутствие 1 комната</h3> <text>" + String(sensor1?"В комнате кто-то есть":"В комнате никого") + "</text> </p> <p> <h3>Присутствие 2 комната</h3> <text>" + String(sensor2?"В комнате кто-то есть":"В комнате никого") + "</text> </p> <hr> <div>"+history+"</div> <hr> <a href=\"hab\"><button>Обновить</button></a> </div>";
	myHtml += "<div id=\"content-4\"> <h1>Внешние датчики</h1> <h4>Температура: "+String(temp)+"</h4> <h4>Влажность: "+String(hum)+"</h4> <h4>Освещённость: "+String(light)+"</h4> <hr> <a href=\"hab\"><button>Обновить</button></a> </div> </div> </div> </body> </html>";
	webServer.send(200, "text/html", myHtml);
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
}

void loop(){
  WiFiClient client = server.available();
 
  if(client){
    Serial.println("new client");
    unsigned int t = millis()+5000;
    while(client && !client.available() && t > millis()){
      webServer.handleClient();
    }

    String pref = client.readStringUntil(':');
    Serial.println(pref);


    if(pref == "curtains"){
     	curtPos = client.readStringUntil('\r').toInt();
    }
    else if(pref == "lighting"){
     	String pref2 = client.readStringUntil(':');
     	int color = client.readStringUntil('\r').toInt();
     	if(pref2=="lamp1")
     		lamp1 = color;
     	else if(pref2=="lamp2")
     		lamp2 = color;
     }
    else if(pref == "presence"){
    	sensor1 = client.readStringUntil(':').toInt(); 
    	sensor2 = client.readStringUntil('\r').toInt();
    }
    else if(pref == "sensors"){
    	temp = client.readStringUntil(':').toInt();
    	hum = client.readStringUntil(':').toInt();
    	light = client.readStringUntil('\r').toInt();
    } 
  }
  webServer.handleClient();
}

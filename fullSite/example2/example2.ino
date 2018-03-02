#include <ESP8266WiFi.h>

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

WiFiServer server(80);

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
  

  //запуск сервера 
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Чекаем пдключение, если его нет, выходим
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  //Ждём запроса
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  //Читаем запрос
  String request = client.readStringUntil('\r');
  client.flush();

  //Создаём ответ
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

  String state = "";
  if(request.indexOf("/colors")>-1){
    state = "gray";
     if(request.indexOf("/colors/red") > -1)
      state = "red";
    if(request.indexOf("/colors/blue") > -1)
      state = "blue";
    if(request.indexOf("/colors/green") > -1)
      state = "green";
  
  
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
    colorString += " <a href=\"/colors/red\"><button style=\"color: red\">red</button></a> <a href=\"/colors/blue\"><button style=\"color: blue\">blue</button></a> <a href=\"/colors/green\"><button style=\"color: green\">green</button></a>";
    colorString += " <hr> <a href=\"/leds\">Светодиоды</a> <a href=\"/sensors\">Датчики</a> </body> </html>";
  
    response += colorString;
  }
  else if (request.indexOf("/leds")>-1){
    if(request.indexOf("/leds/on0") > -1)
      state = "on0";
    if(request.indexOf("/leds/off0") > -1)
      state = "off0";
    if(request.indexOf("/leds/on4") > -1)
      state = "on4";
    if(request.indexOf("/leds/off4") > -1)
      state = "off4";
    
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
    ledString += " <a href=\"/leds/on0\"><button>on</button></a> <a href=\"/leds/off0\"><button>off</button></a> <p>";
    ledString += " <b>Светодиод D4:</b> <text>"+(String)!digitalRead(ledD4)+"</text> <a href=\"/leds/on4\"><button>on</button></a> <a href=\"/leds/off4\"><button>off</button></a> </p>";
    ledString += " <hr> <a href=\"/sensors\">Датчики</a> <a href=\"/colors\">Цвета</a> </body> </html>";

    response += ledString;
  }
  else if (request.indexOf("/sensors")>-1){
    String sensorString = "<!DOCTYPE html> <html> <head> <title>Датчики</title> <meta charset=\"utf-8\"> </head>";
    sensorString += " <body style=\"zoom: 300%\"> <h1>Датчики</h1> <p> <b>Датчик на D3:</b> "+(String)digitalRead(sensorD3)+" <br>";
    sensorString += " <b>Датчик на D5:</b> "+(String)digitalRead(sensorD5)+" <br> <b>Датчик на A0:</b> "+(String)analogRead(sensorA0)+" <br>";
    sensorString += " </p> <a href=\"/sensors\"><button>update</button></a> <hr> <a href=\"/leds\">Светодиоды</a> <a href=\"/colors\">Цвета</a> </body> </html>";

    response += sensorString;
  }
  else{
    response += "<h1>Incorrect request!</h1>";
  }
  response += "\n";
  client.print(response);
  delay(1);
  client.stop();
}

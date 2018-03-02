#include <ESP8266WiFi.h>

//датчики
#define sensorD3 D3
#define sensorD5 D5
#define sensorA0 A0

const char* ssid = "network-name";
const char* password = "pass-to-network";

WiFiServer server(80);

void setup(void){
//выставляем режим пинов
  pinMode(sensorD3, INPUT);
  pinMode(sensorD5, INPUT);
  pinMode(sensorA0, INPUT);

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

  if (request.indexOf("/sensors")>-1){
    String sensorString = "<!DOCTYPE html> <html> <head> <title>Датчики</title> <meta charset=\"utf-8\"> </head>";
    sensorString += " <body style=\"zoom: 300%\"> <h1>Датчики</h1> <p> <b>Датчик на D3:</b> "+(String)digitalRead(sensorD3)+" <br>";
    sensorString += " <b>Датчик на D5:</b> "+(String)digitalRead(sensorD5)+" <br> <b>Датчик на A0:</b> "+(String)analogRead(sensorA0)+" <br>";
    sensorString += " </p> <a href=\"/sensors\"><button>update</button></a> <hr> </body> </html>";

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

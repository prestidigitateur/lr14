#include <ESP8266WiFi.h>

//rgb светодиод
#define rgbRed D6
#define rgbBlue D7
#define rgbGreen D8

const char* ssid = "network-name";
const char* password = "pass-to-network";

WiFiServer server(80);

void setup(void){
//выставляем режим пинов
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
    colorString += " </body> </html>";
  
    response += colorString;
  }
  else{
    response += "<h1>Incorrect request!</h1>";
  }
  response += "\n";
  client.print(response);
  delay(1);
  client.stop();
}

#include <ESP8266WiFi.h>

//светодиоды
#define ledD0 D0
#define ledD4 D4


const char* ssid = "network-name";
const char* password = "pass-to-network";

WiFiServer server(80);

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
  
  if (request.indexOf("/leds")>-1){
    if(request.indexOf("/leds/on0") > -1)
      digitalWrite(ledD0, LOW);
    if(request.indexOf("/leds/off0") > -1)
      digitalWrite(ledD0, HIGH);
    if(request.indexOf("/leds/on4") > -1)
      digitalWrite(ledD4, LOW);
    if(request.indexOf("/leds/off4") > -1)
      digitalWrite(ledD4, HIGH);
         

    String ledString = "<!DOCTYPE html> <html> <head> <title>Светодиоды</title> <meta charset=\"utf-8\"> </head>";
    ledString += " <body style=\"zoom: 300%\"> <h1>Светодиоды</h1> <p> <b>Светодиод D0:</b> <text>"+(String)!digitalRead(ledD0)+"</text>";
    ledString += " <a href=\"/leds/on0\"><button>on</button></a> <a href=\"/leds/off0\"><button>off</button></a> <p>";
    ledString += " <b>Светодиод D4:</b> <text>"+(String)!digitalRead(ledD4)+"</text> <a href=\"/leds/on4\"><button>on</button></a> <a href=\"/leds/off4\"><button>off</button></a> </p>";
    ledString += " </body> </html>";

    response += ledString;
  }
  else{
    response += "<h1>Incorrect request!</h1>";
  }
  response += "\n";
  client.print(response);
  delay(1);
  client.stop();
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "network-name";
const char *password = "pass-to-network";

const int port = 8888;
WiFiServer server(port);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop(void){
  handleSend();
  handleGet();

}
//Проверка, подключился ли к нам клиент
// и есть ли у него соообщение для нас
void handleGet(){
  WiFiClient client = server.available(); 
  if(client){
    Serial.println("new client");
    while(client && !client.available()){
      //ждём пока клиент что нибудь отправит
      //и проверяем нет ли исходящих сообщений
      handleSend();
    }
    String req = client.readStringUntil('\r');
    Serial.print("Get: ");
    Serial.println(req);
    delay(1);
    client.flush();
    client.stop();
   
  }
}


//проверка, есть ли у нас что нибудь
// что можно отправить
void handleSend(){
  if(Serial.available()>0)
  {
    IPAddress addr;
    String ip = Serial.readStringUntil(':');
    addr.fromString(ip);
    String msg = Serial.readStringUntil('\r');
    
    WiFiClient client;   
    if(client.connect(addr, port)){
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
}

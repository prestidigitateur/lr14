#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP rc_ap;
const char *ssid = "network-name";
const char *password = "pass-to-network";


void setup() {
  delay(1000);
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

  rc_ap.begin(80);
}

char incomingPacket[255];
void loop () {
  //отправка сообщения
  if(Serial.available() > 0) {
    //последния цифра ip адреса
    int ip = Serial.readStringUntil(':').toInt();
    IPAddress user(192, 168, 43, ip);
    //сообщение
    String message = Serial.readStringUntil('\r');
    
    Serial.println(String("I send: " + message + "to ip "+ ip).c_str());
    rc_ap.beginPacket(user, 8080);
    rc_ap.write(message.c_str());
    rc_ap.write('\r');
    rc_ap.endPacket();
    
  }
  
  //получение сообщения
  int packetSize = rc_ap.parsePacket();
  if (packetSize) {// если сообщение есть
      int len = rc_ap.read(incomingPacket, 255);
      if (len > 0)
          incomingPacket[len] = 0;
      Serial.printf("incoming packet: %s\n", incomingPacket);
  }

}

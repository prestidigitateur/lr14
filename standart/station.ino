#include <ESP8266WiFi.h>

// название и пароль от существующей точки доступа
const char *ssid = "name";
const char *password = "password";//больше 8 символов!

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
}

void loop(){
	
}
#include <ESP8266WiFi.h>

// название и пароль от создаваемой точки доступа
const char *ssid = "name";
const char *password = "password";//больше 8 символов!

void setup(){
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Запуск точки доступа...");	
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
}

void loop(){

}
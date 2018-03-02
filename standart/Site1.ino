#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

//что-то ваше, ссиды, пасворды, дэфайны

void setup(){
	//можно использовать любой вариант
	//создавать свою точку доступа или подключаться к существующей
	//примеры лежат в соседних файлах

	//после загрузки, достаточно перейти по IP адресу из COM порта в браузере

	//..............................................................


	//определяем что будет запускаться при переходе на страницы
  	server.on("/", indexFunction);
  	server.on("/index", indexFunction);
  	server.on("/about", aboutFunction);
  	server.on("/error", [](){ errorFunction("AAAAAAAA"); });//используй такую конструкцию если хочешь передавать в функцию аргумент(ы)
 
  	//запуск сервера 
  	server.begin();
  	Serial.println("HTTP server started"); 

}

void loop(){
  server.handleClient();
}

void indexFunction(){
	String myHtml = "";//тут твоя html страница
	// важно она должна быть в одну строку и кавычки должны быть экранированы т.е " -> \"
	// можешь использовать сайт goo.gl/6jhzqk
  	server.send(200, "text/html", myHtml);
}

void aboutFunction(){
	String myHtml = "<a href=\"/index\">LINK</a>";//пример ссылки на другую страницу (index в этом случае)
  	server.send(200, "text/html", myHtml);
}

void errorFunction(String err){
	String myHtml = "<h1>";// пример как вставлять информацию с микроконтролльера
	myHtml += err;// например тут может быть digitalRead(...) или analogRead(...),
	myHtml += "</h1>";

  	server.send(200, "text/html", myHtml);
}
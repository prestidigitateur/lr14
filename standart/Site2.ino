#include <ESP8266WiFi.h>

WiFiServer server(80);

//что-то ваше, ссиды, пасворды, дэфайны

void setup(){
	//можно использовать любой вариант
	//создавать свою точку доступа или подключаться к существующей
	//примеры лежат в соседних файлах

	//после загрузки, достаточно перейти по IP адресу из COM порта в браузере

	//..............................................................


	//запуск сервера 
  	server.begin();
  	Serial.println("HTTP server started");
}

void loop() {
  	// Чекаем подключение, если его нет, выходим
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

  	if (request.indexOf("/parGG")>-1){//если в командной строке вашего браузера был параметер parGG то выполнится следующий код
  		//какой либо код
  	}

  	//Создаём ответ
  	String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
	response += "<h1>Твоя html страничка!</h1>";//тут твоя html страница
	// важно она должна быть в одну строку и кавычки должны быть экранированы т.е " -> \"
	// можешь использовать сайт goo.gl/6jhzqk
	response += "\n";

  	client.print(response);
  	delay(1);
  	client.stop();
}
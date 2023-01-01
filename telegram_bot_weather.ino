// бот отправляет сообщение о текущей погоде в ответ на название города

#define WIFI_SSID "ssid" // название Wifi сети
#define WIFI_PASS "pass" // пароль Wifi сети
#define BOT_TOKEN "1234567890:cjfkdshfkjdscnjdskfhjkgnfdjgkfdgnjf" // токен бота, узнать через https://t.me/BotFather
#define OWM_API_KEY "5edd67cb421961279ee47a4f6765f7d9" // API key for OpenWeatherMap, получить здесь https://home.openweathermap.org/api_keys

#include <FastBot.h>

FastBot bot(BOT_TOKEN);

#include <Arduino_JSON.h>

String json_array;
WiFiClient wifiClient;

void setup() {
  connectWiFi();

  bot.attach(newMsg);
}

// обработчик сообщений
void newMsg(FB_msg & msg) {
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());

  String server = "http://api.openweathermap.org/data/2.5/weather?q=" + msg.text + "&APPID=" + OWM_API_KEY + "&units=metric";

  json_array = GET_Request(server.c_str());
  Serial.println(json_array);
  JSONVar my_obj = JSON.parse(json_array);

  if (JSON.typeof(my_obj) == "undefined") {
    Serial.println("Parsing input failed!");
    bot.sendMessage("Parsing input failed!", msg.chatID);
    return;
  }

  Serial.print("JSON object = ");
  Serial.println(my_obj);
  Serial.print("Temperature: ");
  Serial.println(my_obj["main"]["temp"]);
  Serial.print("Pressure: ");
  Serial.println(my_obj["main"]["pressure"]);
  Serial.print("Humidity: ");
  Serial.println(my_obj["main"]["humidity"]);
  Serial.print("Wind Speed: ");
  Serial.println(my_obj["wind"]["speed"]);

  String answer = "Город: " + msg.text + "\n🌡️ Температура: " + JSON.stringify(my_obj["main"]["temp"]) + "\n⬇ Давление: " + JSON.stringify(my_obj["main"]["pressure"]) + "\n🌫️ Видимость: " + JSON.stringify(my_obj["main"]["humidity"]) + "\n🌪 Ветер: " + JSON.stringify(my_obj["wind"]["speed"]) + " м/c";

  bot.sendMessage(answer, msg.chatID);
}

void loop() {
  bot.tick();
}

void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}

String GET_Request(const char * server) {
  HTTPClient http;
  http.begin(wifiClient, server);
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

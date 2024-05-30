#define WIFI_SSID "Arduino" // название Wifi сети
#define WIFI_PASS "00000000" // пароль Wifi сети
#define BOT_TOKEN "6380832709:AAEabmdcH7AE91z_uwJRZtcZ7EBVxKhJIm8" // токен бота, узнать через https://t.me/BotFather
#define OWM_API_KEY "5edd67cb421961279ee47a4f6765f7d9" // API key for OpenWeatherMap, получить здесь https://home.openweathermap.org/api_keys
#define CITY_NAME "Минеральные воды"

#include <Wire.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include "LiquidCrystal_I2C.h"

#include <Arduino_JSON.h>

#include <NTPClient.h>

#include <WiFiUdp.h>


LiquidCrystal_I2C lcd(0x3F, 16, 2); // Set up the display 0x3F or 0x27 
String json_array;
WiFiClient wifiClient;

const char * ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 10800;
const int daylightOffset_sec = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

unsigned long lastTime = 0;
unsigned long lastTime2 = 0;
unsigned long timerDelay = 5000;
unsigned long lastDisplayTime = 0;
bool timeDisplayed = false;

String jsonBuffer;

byte weatherIcons[][8] = {
  {
    B00001,
    B00010,
    B00100,
    B01000,
    B11111,
    B00100,
    B01000,
    B10000
  }, // Thunderstorm
  {
    B00000,
    B10101,
    B00000,
    B01110,
    B01110,
    B01110,
    B00000,
    B10101
  }, // Clear
  {
    B01110,
    B11111,
    B11111,
    B10101,
    B01010,
    B00101,
    B00010,
    B00001
  }, // Rain
  {
    B01000,
    B11100,
    B01000,
    B00000,
    B00000,
    B01010,
    B00100,
    B01010
  }, // Snow
  {
    B10101,
    B11111,
    B01010,
    B10101,
    B11111,
    B01010,
    B10101,
    B11111
  }, // Fog
  {
    B00000,
    B01110,
    B11111,
    B01110,
    B00000,
    B00000,
    B00000,
    B00000
  } // Cloud
};

void setup() {
  connectWiFi();

  lcd.init();
  lcd.backlight(); // Turn on the backlight of the 
  createCustomChars(); // Загрузка кастомных символов

  timeClient.begin(); // Инициализация NTP-клиента
  /*
  lcd.print("hello");
  lcd.setCursor ( 0 , 1 ) ;
  lcd.print ( "LCD 1602" ) ;
  delay ( 5000 ) ;
  lcd.noBacklight ( ) ; // Turn off display backlight 
  delay ( 5000 ) ;
  lcd.backlight ( ) ; // Turn on display backlight 
  */

}

void loop() {

  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + String(CITY_NAME) + "&APPID=" + String(OWM_API_KEY) + "&units=metric";

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      lcd.clear();
      int temp = myObject["main"]["temp"];
      lcd.setCursor(0, 0);
      lcd.print("T:");
      lcd.print(temp);
      lcd.print((char) 223);
      lcd.print("C");

      int feels_like = myObject["main"]["feels_like"];
      lcd.setCursor(8, 0);
      lcd.print("FL:");
      lcd.print(feels_like);
      lcd.print((char) 223);
      lcd.print("C");

      int wind = myObject["wind"]["speed"];
      lcd.setCursor(0, 1);
      lcd.print("W:");
      lcd.print(wind);
      lcd.print("m/s");
      lcd.setCursor(8, 1);


      String weather = myObject["weather"][0]["main"];
      if (weather == "Thunderstorm") {
        lcd.write(byte(0));
        lcd.print("Thunder");
      } else if (weather == "Drizzle") {
        lcd.write(byte(2));
        lcd.print("Drizzle");
      } else if (weather == "Snow") {
        lcd.write(byte(3));
        lcd.print("Snow");
      } else if (weather == "Mist") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Smoke") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Haze") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Dust") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Fog") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Sand") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Dust") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Ash") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Squall") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Tornado") {
        lcd.write(byte(4));
        lcd.print("Fog");
      } else if (weather == "Clear") {
        lcd.write(byte(1));
        lcd.print("SUN");
      } else if (weather == "Clouds") {
        lcd.write(byte(5));
        lcd.print("Clouds");
      } else {
        lcd.write(byte(1));
        lcd.print("Clouds");
      }
    } else {
      Serial.println("WiFi Disconnected");
    }

    lastTime = millis();
  }
  if ((millis() - lastTime2) > 10000) {
    // Обновление времени с NTP-сервера
    timeClient.update();

    displayTimeAndDate();
    Serial.println("123");
    lastTime2 = millis();
  }
}

/*
void loop_tmp ( )
{
    String server = "http://api.openweathermap.org/data/2.5/weather?q=Moscow&APPID=" + String(OWM_API_KEY) + "&units=metric";
    json_array = GET_Request(server.c_str());
    Serial.println(json_array);
    JSONVar my_obj = JSON.parse(json_array);

    if (JSON.typeof(my_obj) == "undefined") {
        Serial.println("Parsing input failed!");
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

    String temp = "-5";
    String wind = "120";

    /*
    // Convert temperature to a string with 2 decimal places
    char tempStr[10];
    dtostrf(my_obj["main"]["temp"], 6, 2, tempStr);
    String up = "T:" + String(temp) + " W:" + String(wind);
    

  
    // Extract temperature and wind speed as integers
    int tempInt = String(json_array["main"]["temp"]).toInt();
    int windSpeedInt = String(json_array["wind"]["speed"]).toInt();

    // Round temperature and wind speed values
    int roundedTemp = round(tempInt);
    int roundedWindSpeed = round(windSpeedInt);

    String up = "T:" + String(roundedTemp) + " W:" + String(roundedWindSpeed);
    
    
    
    
    lcd.setCursor ( 0 , 0 ) ;
    lcd.print(up);
    lcd.setCursor ( 0 , 1 ) ;
    lcd.print ( "LCD 1602" ) ;

    delay(5000); // 5-second delay
}
*/

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

String httpGETRequest(const char * serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
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

void createCustomChars() {
  // Загрузка кастомных символов в память дисплея
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, weatherIcons[i]);
  }
}

void displayTimeAndDate() {
  lcd.clear();

  if(timeClient.isTimeSet()) {

    lcd.setCursor(0, 0);
    lcd.print("Time: " + timeClient.getFormattedTime());

    lcd.setCursor(0, 1);
  }
}

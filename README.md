# ESP8266 Telegram Weather Bot
**A Telegram bot for get current weather for city on Open Weather Map API.**

**A Telegram bot work on ESP8266**

![image](https://user-images.githubusercontent.com/44779327/210181829-86ab4b40-70ea-4cfc-8b6b-3fa8603e3a1d.png)
![image](https://user-images.githubusercontent.com/44779327/210182030-889dd1eb-f1e0-4a7e-8273-b12afe3df3aa.png)



## Features
- [X] Simple information for current weather in city

## ToDo 
- [ ] weather notification.
- [ ] remembering the city
- [ ] sending charts via CharDisplay


### Installation & Configuration
1) Install [FastBot](https://github.com/GyverLibs/FastBot#install) in Arduino IDE
2) Open telegram_bot_weather.ino and change defines
```c++
#define WIFI_SSID "ssid" // название Wifi сети
#define WIFI_PASS "pass" // пароль Wifi сети
#define BOT_TOKEN "1234567890:cjfkdshfkjdscnjdskfhjkgnfdjgkfdgnjf" // токен бота, узнать через https://t.me/BotFather
#define OWM_API_KEY "5edd67cb421961279ee47a4f6765f7d9" // API key for OpenWeatherMap, получить здесь https://home.openweathermap.org/api_keys
```
3) Compile and run
4) Enjoy!




## Copyright & License
- Copyright (©) 2023 by [LencoDigitexer](https://github.com/lencodigitexer)
- Licensed under the terms of the [GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007](./LICENSE)

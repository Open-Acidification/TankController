ESPAsyncWebServer included from https://github.com/me-no-dev/ESPAsyncWebServer

Included to ensure no issues when library versions change. Changes:
 - Put .h files in quotes, not pointy brackets
 - #ifdef out unless ESP8266 so it will compile on AVR
 - Remove auth as we don't need it

Current version: 2540507664d8d7f7a35717cb9e3fa3efd5b0cf6e
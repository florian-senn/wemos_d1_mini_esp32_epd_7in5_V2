#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define ENABLE_GxEPD2_GFX 1

#include <GxEPD2_4G.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "bmp/bmp1.h"

#include "secrets.h"

#define BLACK GxEPD_BLACK
#define DARK GxEPD_DARKGREY
#define LIGHT GxEPD_LIGHTGREY
#define WHITE GxEPD_WHITE

GxEPD2_4G<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2> display(GxEPD2_750_T7(/*CS=5*/ 26, /*DC=*/25, /*RST=*/33, /*BUSY=*/27));

const char HelloWorld[] = "Hello World!";
unsigned long ref_time = millis();
unsigned long wifi_reconnect_span = 10UL * 1000;
unsigned long wifi_reconnect_time = ref_time + wifi_reconnect_span;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWRD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    ESP.restart();
  }
  Serial.println("WiFi connnected");
  WiFi.setHostname(HOSTNAME);

  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PWRD);
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onProgress([](unsigned int progress, unsigned int total) {})
      .onEnd([]() {})
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });
  ArduinoOTA.begin();
  Serial.println("OTA started");

  display.init(115200);
  display.clearScreen();
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);

  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.writeFillRect(0, 40 * 0, 800, 40, BLACK);
    display.writeFillRect(0, 40 * 1, 800, 40, DARK);
    display.writeFillRect(0, 40 * 2, 800, 40, LIGHT);
    display.writeFillRect(0, 40 * 3, 800, 40, WHITE);
    display.writeFillRect(0, 40 * 4, 800, 40, LIGHT);
    display.writeFillRect(0, 40 * 5, 800, 40, DARK);
    display.writeFillRect(0, 40 * 6, 800, 40, BLACK);
    display.writeFillRect(80 * 0, 40 * 6, 80, 240, DARK);
    display.writeFillRect(80 * 1, 41 * 6, 80, 240, LIGHT);
    display.writeFillRect(80 * 2, 42 * 6, 80, 240, WHITE);
    display.writeFillRect(80 * 3, 43 * 6, 80, 240, LIGHT);
    display.writeFillRect(80 * 4, 44 * 6, 80, 240, DARK);
    display.writeFillRect(80 * 5, 45 * 6, 80, 240, BLACK);
    display.writeFillRect(80 * 6, 46 * 6, 80, 240, DARK);
    display.writeFillRect(80 * 7, 47 * 6, 80, 240, LIGHT);
    display.writeFillRect(80 * 8, 48 * 6, 80, 240, WHITE);
    display.writeFillRect(80 * 9, 49 * 6, 80, 240, LIGHT);
    display.writeFillRect(80 * 10, 50 * 6, 80, 240, DARK);
    display.fillCircle(400, 240, 200, WHITE);
    display.fillCircle(400, 90, 50, BLACK);
    display.fillCircle(400, 140, 50, DARK);
    display.fillCircle(400, 190, 50, LIGHT);

    display.setCursor(x, y + 15 * 0);
    display.setTextColor(BLACK);
    display.print(HelloWorld);
    display.setCursor(x, y + 15 * 1);
    display.setTextColor(DARK);
    display.print("IP:" + WiFi.localIP().toString());
    display.setCursor(x, y + 15 * 2);
    display.setTextColor(LIGHT);
    display.print("Hostname:" + String(WiFi.getHostname()));
    display.setCursor(x, y + 15 * 3);
    display.setTextColor(BLACK);
    display.print("MAC:" + WiFi.macAddress());

  } while (display.nextPage());
  delay(20000);
  Serial.println("First Screen finished");
  display.clearScreen();
  display.setRotation(1);
  display.setFullWindow();
  display.epd2.drawImage_4G(bmp1, 2, 0, 0, 800, 480, true, false, true);
  Serial.println("Display painted awesome image. Wow.'");
  display.hibernate();
  Serial.println("Display went hibernatin'");
}

void loop()
{
  if (ref_time - wifi_reconnect_time > wifi_reconnect_span && WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(WIFI_SSID, WIFI_PWRD);
    wifi_reconnect_time = millis();
    Serial.println("WiFi went reconnect!'");
  }
  ArduinoOTA.handle();
}
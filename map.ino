#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const uint8_t image_data_Saraarray[1024] = {
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    // ████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙███∙∙█████∙███████████∙∙█████∙████∙∙∙∙∙∙∙∙∙∙∙████
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙███∙∙██████████∙∙∙∙███∙∙███∙∙∙∙∙∙∙∙∙∙∙∙∙███
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙█∙∙∙∙███████∙████∙████∙∙∙∙∙∙∙∙∙█∙∙█████████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙█∙∙∙∙███████∙████∙∙███∙∙∙∙∙∙∙∙∙█∙∙█████████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█████∙∙∙██∙∙∙∙∙∙∙██∙██∙∙██∙█████∙∙██∙∙∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█████∙∙∙██∙∙∙∙∙∙∙██∙∙█∙∙██∙█████∙∙██∙∙∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█∙∙██████████∙∙████∙∙∙████∙██∙∙█∙∙██∙∙∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙███∙∙∙∙∙∙∙∙████∙∙∙∙∙∙███∙∙██████∙∙██∙∙∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙███∙∙∙∙∙∙∙∙∙███∙∙∙∙∙████∙∙██████∙∙██∙∙∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙███∙∙∙∙█████∙∙∙∙∙██∙████∙∙███∙∙█∙∙█████████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙███∙∙∙██████∙∙∙∙∙██∙████∙∙███∙∙█∙∙█████████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙██∙██∙∙██∙██∙∙██∙██∙∙█∙∙██∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙███
    // ████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙██∙∙█∙∙∙█∙∙██∙∙█∙∙█∙∙∙█∙███∙██∙∙██∙∙∙∙∙∙∙∙∙∙∙████
    // █████████████████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙████∙∙∙∙∙∙∙∙∙∙█████∙∙∙∙█████████████████
    // ████████████████████████████████████████████████████████████████∙██∙∙∙██∙∙∙∙∙███████∙∙██∙∙∙∙∙∙∙█████∙████████∙████∙██∙∙∙████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙██∙∙∙∙∙███████∙∙██∙∙∙∙∙∙∙█████∙∙██████∙∙████∙██∙∙∙∙███
    // █████████████████████████████████████████████████████████████████∙∙∙∙███∙∙███████∙████∙∙███∙∙∙∙∙∙∙∙∙███∙∙███∙∙∙∙██∙∙∙██∙∙███
    // █████████████████████████████████████████████████████████████████∙∙∙∙███∙∙███████∙∙███∙∙███∙∙∙∙∙∙∙∙∙███∙∙███∙∙∙∙█∙∙∙∙██∙∙███
    // █████████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙∙∙∙∙∙∙∙█∙∙██∙∙███∙∙██∙███████∙∙███∙∙∙∙∙∙∙██∙∙∙∙███
    // ████████████████████████████████████████████████████████████████∙∙∙∙██∙∙∙∙█∙∙∙∙∙∙∙∙█████████∙∙∙███████∙∙███∙∙∙∙∙█∙∙███∙∙∙███
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙█∙∙∙∙██∙∙∙∙∙∙███████████∙∙███∙∙█∙∙████∙∙∙∙∙██∙████∙∙███
    // █████████████████████████████████████████████████████████████████∙∙∙∙█████∙∙∙∙∙████∙∙∙██∙∙∙∙∙███∙∙∙∙██∙∙∙∙∙███∙∙∙∙∙██∙∙∙∙███
    // █████████████████████████████████████████████████████████████████∙∙∙∙█████∙∙∙∙∙███∙∙∙∙██∙∙∙∙∙███∙∙∙∙██∙∙∙∙∙███∙∙∙∙∙██∙∙∙████
    // ███████████████████████████████████████████████████████████████∙∙∙∙█████████∙∙∙██∙∙∙██∙∙∙█████████∙∙██∙██∙∙███∙∙██∙∙∙∙∙█████
    // ████████████████████████████████████████████████████████████████∙∙∙█████████∙∙∙██∙∙∙██∙∙∙█████████∙∙██∙██∙∙███∙∙█∙∙∙∙∙∙█████
    // █████████████████████████████████████████████████████████████████████∙████∙∙█∙∙∙∙███∙∙██∙████∙∙∙∙∙∙∙██∙██∙∙█∙∙∙∙∙∙∙█████████
    // ███████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙██████████∙∙∙∙█∙∙███∙∙∙███∙∙∙████∙∙∙∙∙█∙∙∙∙███████
    // ███████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙██████████∙∙∙∙█∙∙████∙████∙∙∙████∙∙∙∙∙██∙∙∙███████
    // █████████████████████████████████████████████████████████████████∙∙∙∙█∙∙██∙∙∙∙∙██∙∙∙∙∙∙∙∙██∙∙██∙██∙∙∙∙∙████∙∙∙██∙∙∙∙∙███████
    // █████████████████████████████████████████████████████████████████∙∙∙∙█∙∙██∙∙∙∙∙██∙∙∙∙∙∙∙∙██∙∙██∙██∙∙∙∙∙████∙∙∙██∙∙∙∙∙███████
    // █████████████████████████████████████████████████████████████████∙∙∙∙∙██████∙████∙∙∙∙∙∙∙∙∙∙███████∙∙██∙∙∙∙∙∙∙∙██∙∙∙██∙∙∙∙███
    // ██████████████████████████████████████████████████████████████████∙∙∙∙█████∙∙████∙∙∙∙∙∙∙∙∙∙███████∙∙█∙∙∙∙∙∙∙∙∙██∙∙∙██∙∙∙████
    // █████████████████████████████████████████████████████████████████████∙████∙∙∙██∙∙███∙∙██∙∙∙████∙████∙∙∙██∙∙∙∙∙∙∙∙∙█∙∙███████
    // ████████████████████████████████████████████████████████████████∙████∙██∙∙█∙∙∙∙█████∙∙∙∙█████∙∙∙██████████∙∙∙███∙∙███∙∙█████
    // ███████████████████████████████████████████████████████████████∙∙████∙██∙∙██∙∙∙█████∙∙∙∙█████∙∙∙███████████∙████∙∙███∙∙█████
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙██∙∙∙∙█████∙██∙∙███∙∙∙∙███∙∙██∙█████████████∙∙∙██∙████
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙██∙∙∙██████∙∙█∙∙███∙∙∙∙███∙∙██∙█████████████∙∙∙██∙∙███
    // ███████████████████████████████████████████████████████████████∙∙∙∙██████████∙∙∙∙█∙∙██∙∙█∙∙∙∙█████∙∙███∙∙███∙∙████∙████∙∙███
    // ████████████████████████████████████████████████████████████████∙∙∙██████████∙∙∙███∙██∙∙█∙∙∙█████∙∙∙███∙∙∙██∙∙████∙███∙∙∙███
    // ███████████████████████████████████████████████████████████████████∙∙█████∙∙█∙∙███████∙∙∙∙∙████∙∙∙∙∙███∙∙∙∙∙∙∙█████∙∙∙∙∙∙███
    // █████████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙███∙∙∙∙∙████∙∙█∙∙████∙██∙∙██∙∙∙∙∙∙∙∙∙∙██∙█████████
    // █████████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙███∙∙∙∙∙∙███∙∙█∙∙████∙██∙∙█∙∙∙∙∙∙∙∙∙∙∙██∙█████████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙█∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙∙█∙∙████∙∙∙∙∙∙∙∙∙∙∙∙∙█████████
    // ████████████████████████████████████████████████████████████████∙██∙███∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙██∙∙████∙∙∙∙∙∙∙∙∙∙∙∙∙█████████
    // █████████████████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙████∙∙∙∙∙███∙∙██∙∙███∙∙█████∙∙∙█████████
    // ████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙██∙∙███∙∙███████∙∙∙∙∙∙∙∙∙████∙∙∙██∙██∙∙██████████
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙████∙███████∙∙∙∙∙∙∙∙∙███∙∙∙∙█∙∙██∙∙██████████
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙█∙∙∙∙█∙∙█████∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙█████∙∙∙∙∙∙∙█████
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙█∙∙∙∙█∙∙█████∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙█████∙∙∙∙∙∙∙█████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙███∙∙█∙∙██∙∙███∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙███████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙███∙███∙██∙∙███∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙███████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█∙∙█████∙∙██∙██∙∙∙∙█∙∙██∙∙∙∙∙█████∙∙██∙████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█∙∙██∙∙∙∙∙∙∙∙∙∙∙∙██∙████∙∙∙██∙███∙∙∙███∙∙███████
    // ███████████████████████████████████████████████████████████████∙∙██∙∙∙∙∙██∙∙█∙∙██∙∙∙∙∙∙∙∙∙∙∙∙██∙████∙∙∙██∙∙█∙∙∙∙███∙∙███████
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙███∙∙∙∙█∙∙███████∙∙∙█████∙∙∙∙∙∙███∙∙∙∙█████∙∙███
    // ███████████████████████████████████████████████████████████████∙∙█████████∙∙███∙∙∙██∙∙███████∙∙∙██████∙∙∙∙∙███∙∙∙∙█████∙████
    // ███████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙███∙∙∙∙███████∙∙∙████∙∙█∙∙██∙████∙∙██████████
    // ████████████████████████████████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙███∙████∙∙████████∙∙∙████∙███∙██∙████∙███████████
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    // ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1c, 0xfb, 0xff, 0x9f, 0x78, 0x00, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x08, 0x73, 0xff, 0x0e, 0x70, 0x00, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xc8, 0x7f, 0x7b, 0xc0, 0x13, 0xfe, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xc8, 0x7f, 0x79, 0xc0, 0x13, 0xfe, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xcf, 0x8c, 0x06, 0xcd, 0xf3, 0x06, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xcf, 0x8c, 0x06, 0x4d, 0xf3, 0x06, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xc9, 0xff, 0x9e, 0x3d, 0x93, 0x06, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xce, 0x01, 0xe0, 0x73, 0xf3, 0x06, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xce, 0x00, 0xe0, 0xf3, 0xf3, 0x06, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xce, 0x1f, 0x06, 0xf3, 0x93, 0xfe, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xce, 0x3f, 0x06, 0xf3, 0x93, 0xfe, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x09, 0xb3, 0x66, 0xc9, 0x90, 0x00, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x19, 0x13, 0x24, 0x5d, 0x98, 0x00, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0x00, 0x3e, 0x1f, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x63, 0x07, 0xf3, 0x01, 0xf7, 0xfb, 0xd8, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x63, 0x07, 0xf3, 0x01, 0xf3, 0xf3, 0xd8, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x3f, 0xbc, 0xe0, 0x0e, 0x70, 0xc6, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x3f, 0x9c, 0xe0, 0x0e, 0x70, 0x86, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x98, 0x00, 0x4c, 0xe6, 0xfe, 0x70, 0x18, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 0x20, 0x1f, 0xf1, 0xfc, 0xe0, 0x9c, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x04, 0x30, 0x3f, 0xf9, 0xc9, 0xe0, 0xde, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xc1, 0xe3, 0x07, 0x0c, 0x1c, 0x18, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xc1, 0xc3, 0x07, 0x0c, 0x1c, 0x18, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf1, 0x8c, 0x7f, 0xcd, 0x9c, 0xc1, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf1, 0x8c, 0x7f, 0xcd, 0x9c, 0x81, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xc8, 0x73, 0x78, 0x0d, 0x90, 0x1f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xf0, 0x9c, 0x71, 0xe0, 0x87, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xf0, 0x9e, 0xf1, 0xe0, 0xc7, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0xc1, 0x80, 0x66, 0xc1, 0xe3, 0x07, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0xc1, 0x80, 0x66, 0xc1, 0xe3, 0x07, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xf7, 0x80, 0x1f, 0xcc, 0x03, 0x18, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xe7, 0x80, 0x1f, 0xc8, 0x03, 0x18, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xc6, 0x73, 0x1e, 0xf1, 0x80, 0x27, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7b, 0x21, 0xf0, 0xf8, 0xff, 0xc7, 0x39, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7b, 0x31, 0xf0, 0xf8, 0xff, 0xef, 0x39, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0x0f, 0xb3, 0x87, 0x37, 0xff, 0xc6, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0x1f, 0x93, 0x87, 0x37, 0xff, 0xc6, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x4c, 0x87, 0xce, 0x73, 0xde, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0xec, 0x8f, 0x8e, 0x33, 0xdc, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xc9, 0xfc, 0x1e, 0x0e, 0x03, 0xe0, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x98, 0x38, 0x3c, 0x9e, 0xcc, 0x00, 0xdf, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x98, 0x38, 0x1c, 0x9e, 0xc8, 0x00, 0xdf, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x64, 0xc0, 0x00, 0xf9, 0x3c, 0x00, 0x1f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6e, 0xc0, 0x00, 0xfb, 0x3c, 0x00, 0x1f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0x07, 0x33, 0x9f, 0x1f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x19, 0xcf, 0xe0, 0x0f, 0x1b, 0x3f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x09, 0xef, 0xe0, 0x0e, 0x13, 0x3f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xc8, 0x4f, 0x80, 0xc0, 0x1f, 0x01, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xc8, 0x4f, 0x80, 0xc0, 0x1f, 0x01, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xce, 0x4c, 0xe0, 0xc0, 0x00, 0x27, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xce, 0xec, 0xe0, 0xc0, 0x00, 0x27, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xc9, 0xf3, 0x61, 0x30, 0x7c, 0xde, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xc9, 0x80, 0x06, 0xf1, 0xb8, 0xe7, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x60, 0xc9, 0x80, 0x06, 0xf1, 0x90, 0xe7, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xce, 0x13, 0xf8, 0xf8, 0x1c, 0x3e, 0x70, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xce, 0x33, 0xf8, 0xfc, 0x1c, 0x3e, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x09, 0xc3, 0xf8, 0xf2, 0x6f, 0x3f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1d, 0xe7, 0xf8, 0xf7, 0x6f, 0x7f, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0
};

// ==== CONFIGURATION ====
const char* ssid = "Onu";
const char* password = "farhan110";
const char* thingSpeakApiKey = "";
const char* thingSpeakURL = "http://api.thingspeak.com/update";

// ==== VARIABLES ====
String latitude = "0.0";
String longitude = "0.0";
float accuracy = 0.0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start web server
  server.begin();

  // Get initial location
  getMozillaLocation();
  sendToThingSpeak();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //delay(2000); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
  display.display();
}

void loop() {
  // Handle web clients
  handleWebServer();

  // Update every 60 seconds
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 60000) {
    getMozillaLocation();
    sendToThingSpeak();
    lastUpdate = millis();
  }
}

void handleWebServer() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'>";
    html += "<style>body {font-family: Arial, sans-serif; margin: 20px;}";
    html += ".data {background: #f5f5f5; padding: 15px; border-radius: 5px;}</style></head>";
    html += "<body><h1>ESP32 Location Tracker</h1>";
    html += "<div class='data'>";
    html += "<p><strong>Latitude:</strong> " + latitude + "</p>";
    html += "<p><strong>Longitude:</strong> " + longitude + "</p>";
    html += "<p><strong>Accuracy:</strong> " + String(accuracy) + " meters</p>";
    html += "<p><strong>Method:</strong> Mozilla Location Service</p>";
    html += "</div></body></html>";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println(html);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void getMozillaLocation() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return;
  }

  // Scan for nearby WiFi networks
  int networkCount = WiFi.scanNetworks();
  if (networkCount == 0) {
    Serial.println("No WiFi networks found!");
    getLocationFromInternet(); // Fallback
    return;
  }

  // Build JSON payload
  String jsonPayload = "{\"wifiAccessPoints\":[";
  for (int i = 0; i < min(networkCount, 10); i++) { // Max 10 networks
    if (i > 0) jsonPayload += ",";
    jsonPayload += "{\"macAddress\":\"" + WiFi.BSSIDstr(i) + "\",";
    jsonPayload += "\"signalStrength\":" + String(WiFi.RSSI(i)) + ",";
    jsonPayload += "\"channel\":" + String(WiFi.channel(i)) + "}";
  }
  jsonPayload += "]}";

  Serial.println("Sending to Mozilla Location Service...");
  
  HTTPClient http;
  http.begin("https://location.services.mozilla.com/v1/geolocate?key=test");
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST(jsonPayload);
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Response:");
    Serial.println(payload);

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    
    latitude = String(doc["location"]["lat"].as<double>(), 6);
    longitude = String(doc["location"]["lng"].as<double>(), 6);
    accuracy = doc["accuracy"].as<float>();
    
    Serial.print("Location: ");
    Serial.print(latitude);
    Serial.print(", ");
    Serial.print(longitude);
    Serial.print(" ± ");
    Serial.print(accuracy);
    Serial.println(" meters");
  } else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
    getLocationFromInternet(); // Fallback to IP geolocation
  }
  
  http.end();
  WiFi.scanDelete();
}

void getLocationFromInternet() {
  HTTPClient http;
  http.begin("http://ip-api.com/json");
  
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DynamicJsonDocument doc(512);
    deserializeJson(doc, payload);
    
    latitude = String(doc["lat"].as<double>(), 6);
    longitude = String(doc["lon"].as<double>(), 6);
    accuracy = 1.0; // IP geolocation typically has ~5km accuracy
    
    Serial.print("IP Location: ");
    Serial.print(latitude);
    Serial.print(", ");
    Serial.println(longitude);
  }
  http.end();
}

void sendToThingSpeak() {
  HTTPClient http;
  String url = String(thingSpeakURL) + "?api_key=" + thingSpeakApiKey + 
               "&field1=" + latitude + "&field2=" + longitude;
  Serial.println("Sending to ThingSpeak: " + url);

  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.print("ThingSpeak Response: ");
    Serial.println(httpCode);
  } else {
    Serial.println("Error sending to ThingSpeak");
  }
  http.end();
}
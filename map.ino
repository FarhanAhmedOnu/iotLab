#include <WiFi.h>
#include <HTTPClient.h>

// ==== CONFIGURATION ====
const char* ssid = "Redmi";
const char* password = "farhan110";

const char* geoAPI = "http://ip-api.com/json"; // Public IP geolocation API
const char* thingSpeakApiKey = "";
const char* thingSpeakURL = "http://api.thingspeak.com/update";

// ==== VARIABLES ====
String latitude = "0.0";
String longitude = "0.0";

// Web server setup
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

  // Get location and send to ThingSpeak
  getLocationFromInternet();
  sendToThingSpeak();
}

void loop() {
  // Handle client for real-time web view
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'/>";
    html += "<title>ESP32 Location</title></head><body>";
    html += "<h1>ESP32 Real-Time Location</h1>";
    html += "<p>Latitude: " + latitude + "</p>";
    html += "<p>Longitude: " + longitude + "</p>";
    html += "</body></html>";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println(html);
    client.stop();
    Serial.println("Client disconnected.");
  }

  // Update ThingSpeak every 60 seconds
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 60000) {
    getLocationFromInternet();
    sendToThingSpeak();
    lastUpdate = millis();
  }
}

void getLocationFromInternet() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(geoAPI);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Location Response:");
      Serial.println(payload);

      // Parse Latitude & Longitude from JSON manually
      int latIndex = payload.indexOf("\"lat\":") + 6;
      int latEnd = payload.indexOf(",", latIndex);
      latitude = payload.substring(latIndex, latEnd);

      int lonIndex = payload.indexOf("\"lon\":") + 6;
      int lonEnd = payload.indexOf(",", lonIndex);
      longitude = payload.substring(lonIndex, lonEnd);

      Serial.println("Parsed Latitude: " + latitude);
      Serial.println("Parsed Longitude: " + longitude);
    } else {
      Serial.println("Failed to get location data.");
    }
    http.end();
  }
}

void sendToThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(thingSpeakURL) + "?api_key=" + thingSpeakApiKey + 
                 "&field1=" + latitude + "&field2=" + longitude;
    Serial.println("Sending to ThingSpeak: " + url);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.print("ThingSpeak Response Code: ");
      Serial.println(httpCode);
    } else {
      Serial.println("Error sending data to ThingSpeak");
    }
    http.end();
  }
}

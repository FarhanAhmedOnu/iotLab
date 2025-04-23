#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ==== CONFIGURATION ====
const char* ssid = "Onu";
const char* password = "";
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

#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  WebServer server(80);
#endif

const char* ssid = "iPhone (Łukasz)";
const char* password = "12345678";

const int ledPin = LED_BUILTIN; 

const int STATE_ON = LOW;  
const int STATE_OFF = HIGH;

void handleRoot();
void handleLedOn();
void handleLedOff();
String getHtmlPage(String status);

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, STATE_OFF);

  Serial.println();
  Serial.print("Laczenie z WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nPolaczono!");
  Serial.print("Adres strony: http://");
  Serial.println(WiFi.localIP()); 

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);

  server.begin();
  Serial.println("Serwer HTTP uruchomiony");
}

void loop() {
  server.handleClient();
}


String getHtmlPage(String status) {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body { font-family: Helvetica, Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f0f0f0;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 20px 40px; ";
  html += "text-align: center; display: inline-block; font-size: 20px; margin: 10px; cursor: pointer; border-radius: 12px;}";
  html += ".button-off { background-color: #f44336; }";
  html += "</style></head>";
  html += "<body>";
  html += "<h1>Sterowanie LED (PlatformIO)</h1>";
  html += "<p>Status: <strong>" + status + "</strong></p>";
  
  if (status == "WYLACZONA") {
    html += "<a href=\"/on\"><button class=\"button\">WLACZ</button></a>";
  } else {
    html += "<a href=\"/off\"><button class=\"button button-off\">WYLACZ</button></a>";
  }
  
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHtmlPage("WYLACZONA"));
}

void handleLedOn() {
  digitalWrite(ledPin, STATE_ON);
  Serial.println("LED WLACZONA");
  server.send(200, "text/html", getHtmlPage("WLACZONA"));
}

void handleLedOff() {
  digitalWrite(ledPin, STATE_OFF);
  Serial.println("LED WYLACZONA");
  server.send(200, "text/html", getHtmlPage("WYLACZONA"));
}
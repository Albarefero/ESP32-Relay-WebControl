#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SSID";    // Replace with your Wi-Fi SSID
const char* password = "PASSWORD";  // Replace with your Wi-Fi password

WebServer server(80);

const int relayPin = 23;

void setup() {
    Serial.begin(115200);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Define routes for the web server
    server.on("/", handleRoot);
    server.on("/relay/on", lightOnListener);
    server.on("/relay/off", lightOffListener);

    server.begin(); // Start the server
}

void loop() {
    server.handleClient(); // Handle incoming client requests
}

void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<title>ESP32 Relay Control</title>";
    html += "<style>";
    html += "body { text-align: center; font-family: Arial, sans-serif; }";
    html += "h1 { color: #333; }";
    html += "button { padding: 15px 30px; font-size: 20px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; }";
    html += ".on { background-color: green; color: white; }";
    html += ".off { background-color: red; color: white; }";
    html += "</style>";
    html += "<script>";
    html += "function sendRequest(action) {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/relay/' + action, true);";
    html += "  xhr.send();";
    html += "}";
    html += "</script>";
    html += "</head>";
    html += "<body>";
    html += "<h1>ESP32 Relay Control</h1>";
    html += "<button class=\"on\" onclick=\"sendRequest('on')\">Turn On</button>";
    html += "<button class=\"off\" onclick=\"sendRequest('off')\">Turn Off</button>";
    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html); // Send HTML response
}

void relayOnListener() {
    digitalWrite(relayPin, HIGH);
    server.send(200, "text/plain", "ON");
}

void relayOffListener() {
    digitalWrite(relayPin, LOW);
    server.send(200, "text/plain", "OFF");
}

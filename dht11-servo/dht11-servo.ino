#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <DHT.h>

// Khai báo thông tin mạng và cảm biến
const char* ssid = "BIBO1-5G";
const char* password = "bibo617487";
const int servoPin = 16;  // Chân control của servo
const int DHTPin = 14;     // Chân kết nối cảm biến DHT
const int DHTType = DHT11; // Loại cảm biến DHT

Servo myServo;
DHT dht(DHTPin, DHTType);
ESP8266WebServer server(80);

void setup() {
  initializeSerial();
  connectToWiFi();
  initializeServo();
  initializeDHT();
  setupServerEndpoints();
  server.begin();
}

void loop() {
  server.handleClient(); // Xử lý các yêu cầu từ client
}

void initializeSerial() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
}

void connectToWiFi() {
  Serial.print("Đang kết nối tới ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi đã kết nối");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());
}

void initializeServo() {
  myServo.attach(servoPin);
  myServo.write(0); // Đặt góc servo về 0 độ ngay lúc khởi động
}

void initializeDHT() {
  dht.begin();
}

void setupServerEndpoints() {
  server.on("/", HTTP_GET, handleRootRequest);
  server.on("/servo90", HTTP_GET, handleServo90);
  server.on("/servo180", HTTP_GET, handleServo180);
  server.on("/data", HTTP_GET, handleDataRequest);
}

void handleRootRequest() {
  server.sendHeader("Content-Type", "text/html; charset=UTF-8");
  server.send(200, "text/html", generateHTML());
}

void handleServo90() {
  executeServoMovement(90, 1000);
}

void handleServo180() {
  executeServoMovement(180, 1000);
}

void executeServoMovement(int angle, int duration) {
  myServo.write(angle);
  delay(duration);
  myServo.write(0);
  delay(duration);
  server.sendHeader("Content-Type", "text/html; charset=UTF-8");
  server.send(200, "text/html", "Servo đã xoay " + String(angle) + " độ");
}

void handleDataRequest() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String jsonData = "{\"humidity\": \"" + String(humidity, 2) + "\", \"temperature\": \"" + String(temperature, 2) + "\"}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Type", "application/json; charset=UTF-8");
  server.send(200, "application/json", jsonData);
}

String generateHTML() {
  return "<!DOCTYPE html>"
         "<html>"
         "<head><meta charset=\"UTF-8\"><title>ESP8266 Weather Station</title></head>"
         "<body>"
         "<h1>ESP8266 DHT11 - Servo</h1>"
         "<p>Độ ẩm: <span id='humidity'>--</span>%</p>"
         "<p>Nhiệt độ: <span id='temperature'>--</span> °C</p>"
         "<form action=\"/servo90\" method=\"get\"><button type=\"submit\">Xoay 90 độ</button></form>"
         "<form action=\"/servo180\" method=\"get\"><button type=\"submit\">Xoay 180 độ</button></form>"
         "<script>"
         "setInterval(function() {"
         "fetch('/data').then(response => response.json()).then(data => {"
         "document.getElementById('humidity').textContent = data.humidity;"
         "document.getElementById('temperature').textContent = data.temperature;"
         "});"
         "}, 1000);"
         "</script>"
         "</body>"
         "</html>";
}

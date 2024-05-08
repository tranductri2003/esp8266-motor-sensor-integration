//Khai báo thư viện
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Khai báo thông tin wifi
const char* ssid = "Undefine";
const char* password = "28092003";

//Khai báo đối tượng webserver, sử dụng port 80
ESP8266WebServer server(80);

//Khai báo chân LED
const int LED_PIN = 5;

//Khai báo biến chứa mã nguồn html dùng const char* R"html
const char* html = R"html(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="utf-8">
    <style>
      button {
        width:200px;
        height:100px;
        font-size:36px;
      }
    </style>
  </head>
  <body>
    <h1>Webserver trên ESP8266</h1>
    <p>Điều khiển LED qua giao diện web</p>
    <p><a href="/led/on"><button>Bật LED</button></a></p>
    <p><a href="/led/off"><button>Tắt LED</button></a></p>
  </body>
</html>
)html";

//Hàm khởi tạo webserver
void setupServer() {
  //Thiết lập chân LED là output
  pinMode(LED_PIN, OUTPUT);
  //Tắt LED
  digitalWrite(LED_PIN, LOW);

  //Kết nối wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Định nghĩa các hàm xử lý yêu cầu từ người dùng
  //Nếu người dùng truy cập địa chỉ IP của ESP8266, trả về giao diện web để điều khiển LED
  server.on("/", handleRoot);

  //Nếu người dùng truy cập địa chỉ IP/led/on, bật LED và trả về thông báo
  server.on("/led/on", handleLedOn);

  //Nếu người dùng truy cập địa chỉ IP/led/off, tắt LED và trả về thông báo
  server.on("/led/off", handleLedOff);

  //Nếu người dùng truy cập địa chỉ IP không hợp lệ, trả về mã lỗi 404
  server.onNotFound(handleNotFound);

  //Bắt đầu webserver
  server.begin();
  Serial.println("Webserver started");
}

//Hàm xử lý yêu cầu gốc
void handleRoot() {
  //Gửi biến chứa mã nguồn html cho người dùng
  server.send(200, "text/html", html);
}

//Hàm xử lý yêu cầu bật LED
void handleLedOn() {
  //Bật LED
  digitalWrite(LED_PIN, HIGH);
  //Gửi thông báo cho người dùng
  server.send(200, "text/plain", "LED da duoc bat");
}

//Hàm xử lý yêu cầu tắt LED
void handleLedOff() {
  //Tắt LED
  digitalWrite(LED_PIN, LOW);
  //Gửi thông báo cho người dùng
  server.send(200, "text/plain", "LED da duoc tat");
}

//Hàm xử lý yêu cầu không hợp lệ
void handleNotFound() {
  //Gửi mã lỗi 404 cho người dùng
  server.send(404, "text/plain", "Khong tim thay trang Web");
}

//Hàm setup của Arduino IDE
void setup() {
  //Khởi tạo cổng nối tiếp
  Serial.begin(115200);
  //Khởi tạo webserver
  setupServer();
}

//Hàm loop của Arduino IDE
void loop() {
  //Xử lý các yêu cầu từ người dùng
  server.handleClient();
}
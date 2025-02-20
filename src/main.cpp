#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT11

const char* ssid = "Tuandat";       // Thay bằng SSID WiFi của bạn
const char* password = "88888888";        // Thay bằng mật khẩu WiFi của bạn
String apiKey = "RGAPEMJG8DRQ6YAE";       // API Key của ThingSpeak
const char* server = "http://api.thingspeak.com/update";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Đang kết nối WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println("\nĐã kết nối WiFi!");
    dht.begin();
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Lỗi đọc cảm biến!");
        return;
    }

    Serial.print("Nhiệt độ: ");
    Serial.print(temperature);
    Serial.print("°C, Độ ẩm: ");
    Serial.print(humidity);
    Serial.println("%");

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = server + String("?api_key=") + apiKey +
                     "&field1=" + String(temperature) +
                     "&field2=" + String(humidity);
        http.begin(url);
        http.GET();
        http.end();
    }

    delay(15000);
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// 設定無線基地台SSID跟密碼
const char* ssid     = "WiFi名稱";     
const char* password = "WiFi密碼";     

const int EmvPin = 4; //電磁閥訊號輸出(D2)
WiFiClient wifiClient;  //wifi設定

void setup() {
  Serial.begin(115200);  // 設定速率 感測器

  // 連接無線基地台
  WiFi.begin(ssid, password);

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting.....");
  }
  pinMode(EmvPin, OUTPUT);  //ESP8266對電磁閥輸出腳位
}

void loop() {
  delay(50);
  if (WiFi.status() == WL_CONNECTED) {
    OpenOrClose();
  }
}
//手動function
void OpenOrClose() {
  HTTPClient httpState;
  httpState.begin(wifiClient, "WebAPI(請自行設計輸出1為開，0為關)");
  int httpStateCode = httpState.GET();

  if (httpStateCode > 0) {
    String Content = httpState.getString();
    Serial.println("是否開啟電磁閥:" + Content);

    if (Content == "1") {
      if (digitalRead(EmvPin) == HIGH) {
        Serial.println("電磁閥開啟\n");
      } else {
        digitalWrite(EmvPin, HIGH);
        EmvFeedBackOpen();
      }
    } else {
      if (digitalRead(EmvPin) == LOW) {
        Serial.println("電磁閥關閉\n");
      } else {
        digitalWrite(EmvPin, LOW);
        EmvFeedBackClose();
      }
    }
  }else
  {
    Serial.println("未接收到回傳值\n");
  }
}

//電磁閥開啟API
void EmvFeedBackOpen() {
  HTTPClient httpEmvOpen;
  httpEmvOpen.begin(wifiClient, "WebAPI(輸出1)");
  int httpEmvOpenCode = httpEmvOpen.GET();
  Serial.println("電磁閥開啟\n");
}
//電磁閥關閉API
void EmvFeedBackClose() {
  HTTPClient httpEmvClose;
  httpEmvClose.begin(wifiClient, "WebAPI(輸出0)");
  int httpEmvCloseCode = httpEmvClose.GET();
  Serial.println("電磁閥關閉\n");
}

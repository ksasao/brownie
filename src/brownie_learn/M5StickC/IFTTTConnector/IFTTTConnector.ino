#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>

static char ssid[64];  // SSID
static char pass[64];  // password
static char ifttt[64]; // IFTTT key
const int MAX_RETRY = 5;

const int MAX_RX_BUFFER_SIZE = 1024;
const int MAX_JSON_BUFFER_SIZE = 1024;
const size_t MAX_PREF_SIZE = 64;

char rx_buffer[MAX_RX_BUFFER_SIZE];
StaticJsonDocument<MAX_JSON_BUFFER_SIZE> doc;
Preferences preferences;

int rx_buffer_pointer = 0;
const String url_pre ="https://maker.ifttt.com/trigger/";
const String url_post = "/with/key/";

HardwareSerial serial_ext(2);

static void initialize_wifi()
{
  // Load preferences from M5StickC flash
  preferences.begin("brownie", false);
  preferences.getString("ssid",ssid,sizeof(ssid));
  preferences.getString("pass",pass,sizeof(pass));
  preferences.getString("ifttt",ifttt,sizeof(ifttt));
  preferences.end();

  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("IFTTT Key:");
  Serial.println(ifttt);
  
  M5.Lcd.setTextSize(1);
  print_lcd(ssid);
  
  WiFi.begin(ssid, pass);
  int count = 0;
  while( WiFi.status() != WL_CONNECTED && count < MAX_RETRY) {
    Serial.print(".");
    delay(500);
    count++; 
  }
  if( WiFi.status() == WL_CONNECTED ){
    print_lcd("Connected.");
    Serial.println("Connected.");
  }else{
    print_lcd("WiFi error.");
    Serial.println("WiFi error.");
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  print_lcd("Connecting...");
  initialize_wifi();
  serial_ext.begin(115200, SERIAL_8N1, 36, 0);
}

void loop() {
  
  M5.update();
  if (serial_ext.available()) {
    char d = serial_ext.read();
    rx_buffer[rx_buffer_pointer] = d;
    if(d == 0){
      // Parse as JSON
      if(rx_buffer[0]=='{'){
        DeserializationError error = deserializeJson(doc, rx_buffer);
        if(!error){
          Serial.println(rx_buffer);
          wifi_update((JsonDocument)doc);
          rx_buffer_pointer = 0;
          return;
        }
      }
      // Send to IFTTT
      print_lcd(rx_buffer);
      if(rx_buffer_pointer > 0){
        String url = url_pre + String(rx_buffer) + url_post + String(ifttt);
        Serial.println(url);
        http_get(url);
      }
      rx_buffer_pointer = 0;
    }else{
      if(rx_buffer_pointer < MAX_RX_BUFFER_SIZE-2){
        rx_buffer_pointer++;
      }
    }
  }
}

void print_lcd(char* str){
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 5, 4);
  M5.Lcd.println(str);
}

void http_get(String url){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
    } 
    http.end();
  }
}

void wifi_update(JsonDocument doc){
  print_lcd("Updating...");
  delay(1000);
  
  bool updated = false;
  if(doc.containsKey("ssid")){
    updated=true;
    memcpy(ssid,(const char*)doc["ssid"],sizeof(ssid));
  }
  
  if(doc.containsKey("pass")){
    updated=true;
    memcpy(pass,(const char*)doc["pass"],sizeof(pass));
  }
  if(doc.containsKey("ifttt")){
    updated=true;
    memcpy(ifttt,(const char*)doc["ifttt"],sizeof(ifttt));
  }
  if(updated){
      preferences.begin("brownie", false);
      preferences.putString("ssid",ssid);
      preferences.putString("pass",pass);
      preferences.putString("ifttt",ifttt);
      preferences.end();
      print_lcd("Updated.");
      delay(2000);
      ESP.restart();
  }else{
      print_lcd("Update error.");
      delay(2000);
  }
}

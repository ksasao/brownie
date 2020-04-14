#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "M5Atom.h"

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

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

void set_led(uint8_t r, uint8_t g, uint8_t b){
  int color = (g << 16) | (r << 8) | b;
  M5.dis.drawpix(0, color);
}
void blink_led(uint8_t r, uint8_t g, uint8_t b, int duration, int count){
  for(int i=0; i< count; i++){
    set_led(r,g,b);
    delay(duration>>1);
    set_led(0,0,0);
    delay(duration>>1);
  }
}
void error_led(){
    blink_led(255,0,0,100,10);
}

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
  
  WiFi.begin(ssid, pass);
  int count = 0;
  while( WiFi.status() != WL_CONNECTED && count < MAX_RETRY) {
    Serial.print(".");
    blink_led(255,128,0,500,1);
    count++; 
  }
  if( WiFi.status() == WL_CONNECTED ){
    Serial.println("Connected.");
    set_led(0,0,255);
  }else{
    Serial.println("WiFi error.");
    error_led();
  }
}

void setup() {
  M5.begin(true,false,true);
  setBuff(0x00, 0x40, 0x00);
  initialize_wifi();
  Serial1.begin(115200, SERIAL_8N1, 26, 32);
}

void loop() {
  M5.update();
  
  if (Serial1.available()) {
    char d = Serial1.read();
    rx_buffer[rx_buffer_pointer] = d;
    if(d == 0){
      // Parse as JSON
      if(rx_buffer[0]=='{'){
        DeserializationError error = deserializeJson(doc, rx_buffer);
        if(!error){
          Serial.println(rx_buffer);
          wifi_update(doc);
          rx_buffer_pointer = 0;
          return;
        }
      }
      // Send to IFTTT
      if(rx_buffer_pointer > 0){
        String url = url_pre + String(rx_buffer) + url_post + String(ifttt);
        Serial.println(url);
        http_get(url);
      }
      set_led(0,0,255);
      rx_buffer_pointer = 0;
    }else{
      if(rx_buffer_pointer < MAX_RX_BUFFER_SIZE-2){
        rx_buffer_pointer++;
      }
    }
  }
}

void http_get(String url){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    blink_led(0,255,255, 200,3);
    set_led(0,255,255);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
    } 
    http.end();
    set_led(0,0,255);
  }else{
    error_led();
  }
}

void wifi_update(StaticJsonDocument<MAX_JSON_BUFFER_SIZE> doc){
  set_led(255,255,0);
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
      M5.dis.drawpix(0, 0xf000f0);
      preferences.begin("brownie", false);
      preferences.putString("ssid",ssid);
      preferences.putString("pass",pass);
      preferences.putString("ifttt",ifttt);
      preferences.end();
      delay(2000);
      ESP.restart();
  }else{
      error_led();
      delay(2000);
  }
}

#include "M5Atom.h"
#include "AtomClient.h"
#include <Preferences.h>
#include <ArduinoJson.h>

static char ssid[64];  // SSID
static char pass[64];  // password
static char mqtt[64]; // MQTT Address

AtomClient ac;

const int MAX_RETRY = 5;

const int MAX_RX_BUFFER_SIZE = 1024;
const int MAX_JSON_BUFFER_SIZE = 1024;
const size_t MAX_PREF_SIZE = 64;

char rx_buffer[MAX_RX_BUFFER_SIZE];
StaticJsonDocument<MAX_JSON_BUFFER_SIZE> doc;
Preferences preferences;

int rx_buffer_pointer = 0;
String oldId = "";
unsigned long lastUpdated = 0;

static void initialize_wifi()
{
  // Load preferences from M5StickC flash
  preferences.begin("brownie", false);
  preferences.getString("ssid",ssid,sizeof(ssid));
  preferences.getString("pass",pass,sizeof(pass));
  preferences.getString("mqtt",mqtt,sizeof(mqtt));
  preferences.end();

  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("MQTT Server:");
  Serial.println(mqtt);

  ac.setup("Brownie", ssid, pass, mqtt);
  Serial.println();
  Serial.print("MQTT Client ID: ");
  Serial.println(ac.getClientId());
}

void setup() {
  M5.begin(true,false,true);
  initialize_wifi();
  Serial1.begin(115200, SERIAL_8N1, 26, 32);
}

void loop() {
  unsigned long now = millis();
  String topic = String("Brownie");
  
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
      // Send to MQTT
      ac.reconnect();
      
      if(rx_buffer_pointer == 0){
        if(oldId.length() > 0){
          String dataOld = "{\"Id\":\"" + oldId + "\",\"Value\":" + String(0) + "}";
          ac.publish(topic,dataOld);
          lastUpdated = now;
          oldId = "";
        }
      }else if(rx_buffer_pointer > 0){ 
        String id = String(rx_buffer);
        if(oldId.length() > 0){
          String dataOld = "{\"Id\":\"" + oldId + "\",\"Value\":" + String(0) + "}";
          ac.publish(topic,dataOld);
          lastUpdated = now;
        }
        oldId = id; 
        String data = "{\"Id\":\"" + id + "\",\"Value\":" + String(1) + "}";
        ac.publish(topic,data);
        lastUpdated = now;
      }
      rx_buffer_pointer = 0;
    }else{
      if(rx_buffer_pointer < MAX_RX_BUFFER_SIZE-2){
        rx_buffer_pointer++;
      }
    }
  }
  if(now - lastUpdated > 10000 && oldId.length() > 0){
      String dataOld = "{\"Id\":\"" + oldId + "\",\"Value\":" + String(1) + "}";
      ac.publish(topic,dataOld);
      lastUpdated = now;
  }
  M5.update();
}

void wifi_update(StaticJsonDocument<MAX_JSON_BUFFER_SIZE> doc){
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
  if(doc.containsKey("mqtt")){
    updated=true;
    memcpy(mqtt,(const char*)doc["mqtt"],sizeof(mqtt));
  }
  if(updated){
      M5.dis.drawpix(0, 0xf000f0);
      preferences.begin("brownie", false);
      preferences.putString("ssid",ssid);
      preferences.putString("pass",pass);
      preferences.putString("mqtt",mqtt);
      preferences.end() ;
      delay(2000);
      ESP.restart();
  }else{
      delay(2000);
  }
}

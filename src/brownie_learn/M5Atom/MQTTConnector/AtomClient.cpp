#include <M5Atom.h>
#include <ArduinoUniqueID.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "AtomClient.h"

const char hex[17]="0123456789ABCDEF";
char id[100];
String name;
char clientIdChar[50];

char msgBuffer[256];
char topicBuffer[256];

char* _ssid;
char* _password;
char* _server;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char mqttUserName[] = "";
const char mqttPass[] = "";

// private methods
void createClientId(String header){
  id[UniqueIDsize*2]='\0';
  for(size_t i = 0; i < UniqueIDsize; i++){
      id[i*2] = hex[UniqueID[i] >> 4];
      id[i*2+1] = hex[UniqueID[i] & 0xF];
  }
  name = header;
  String clientId = name + "-" + String((char*) id);
  clientId.toCharArray(clientIdChar,clientId.length()+1);
}

void set_led(uint8_t r, uint8_t g, uint8_t b){
  int color = (g << 16) | (r << 8) | b;
  M5.dis.drawpix(0, color);
  delay(30);
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

void mqttPublish(String topic, String data) {
  set_led(0,255,0);

  int length = data.length();
  data.toCharArray(msgBuffer,length+1);

  length = topic.length();
  topic.toCharArray(topicBuffer, length+1);
  mqttClient.publish(topicBuffer, msgBuffer);
  Serial.print(topic);
  Serial.print(" ");
  Serial.println(data);
  set_led(0,0,0);
}

void reboot(){
  Serial.println("Rebooting...");
  delay(5 * 1000);
  ESP.restart();
}

void initWiFi(){
  // WiFi connection
  WiFi.begin(_ssid, _password);
  int count = 0;
  while( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blink_led(255,128,0,500,1);
    count++;
    if(count > 100){
      error_led();
      reboot();
    }
  }
  Serial.println("Connected.");
  set_led(0,0,255);
  mqttClient.setServer(_server, 1883); 
}

void reConnect() {
  int resetCounter = 0;
  while (!mqttClient.connected()) {
    set_led(255,255,0);
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(clientIdChar,mqttUserName,mqttPass)) {
      Serial.print("Connected with Client ID:  ");
      Serial.print(clientIdChar);
      Serial.print(", Username: ");
      Serial.print(mqttUserName);
      Serial.print(" , Passwword: ");
      int i=0;
      while(mqttPass[i++]!='\0'){
        Serial.print("*");
      }
      Serial.println();
    }
    else {
      error_led();
      // http://pubsubclient.knolleary.net/api.html#state
      Serial.print("failed, state=");
      Serial.println(mqttClient.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
    if(resetCounter++ >= 10){
      reboot();
    }
  }
  set_led(0,0,0);
}

// public methods
AtomClient::AtomClient()
{
}
void AtomClient::setup(String name, char* ssid, char* password, char* server)
{
  _ssid = ssid;
  _password = password;
  _server = server;
  createClientId(name);
  initWiFi();
}

String AtomClient::getName(void)
{
  return name;
}
char* AtomClient::getClientId(void)
{
  return clientIdChar;
}

void AtomClient::reconnect(void){
  reConnect();
  mqttClient.loop();
}
void AtomClient::publish(String topic, String body){
  mqttPublish(topic,body);
}

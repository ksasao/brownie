#include <M5Atom.h>
#include <ArduinoUniqueID.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "AtomClient.h"

typedef enum
{
  OFF,
  BOOTING,
  CONNECTING,
  CONNECTED,
  DISCONNECTED,
  SENDING,
  RECEIVING,
  ERROR,
  SAVING
} BROWNIE_STATUS;

const char hex[17]="0123456789ABCDEF";
char id[100];
String name;
char clientIdChar[50];
bool wifiConnected = false;

char msgBuffer[256];
char topicBuffer[256];

char* _ssid;
char* _password;
char* _server;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char mqttUserName[] = "";
const char mqttPass[] = "";

void reConnect(bool toReboot);

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

void setStatus(BROWNIE_STATUS status){
  switch(status){
    case OFF:
      set_led(0,0,0);
      break;
    case BOOTING:
      set_led(255,255,255);
      break;
    case CONNECTING:
      blink_led(255,128,0,500,1);
      break;
    case CONNECTED:
      set_led(0,0,255);
      break;
    case DISCONNECTED:
      set_led(255,255,0);
      break;
    case SENDING:
      set_led(0,255,0);
      break;
    case RECEIVING:
      break;
    case ERROR:
      blink_led(255,0,0,100,10);
      break;
    case SAVING:
      break;
    default:
      set_led(0,0,0);
      break;
  }
}

void mqttPublish(String topic, String data) {
  setStatus(SENDING);

  int length = data.length();
  data.toCharArray(msgBuffer,length+1);

  length = topic.length();
  topic.toCharArray(topicBuffer, length+1);
  mqttClient.publish(topicBuffer, msgBuffer);
  Serial.print(topic);
  Serial.print(" ");
  Serial.println(data);

  setStatus(CONNECTED);
}


void initWiFi(){
  // WiFi connection
  WiFi.begin(_ssid, _password);
  int count = 0;
  wifiConnected = true;
  while( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    setStatus(CONNECTING);
    count++;
    if(count > 10){
      setStatus(ERROR);
      wifiConnected = false;
      break;
    }
  }
  if(wifiConnected){
    Serial.println("Connected.");
    mqttClient.setServer(_server, 1883); 
    reConnect(false);
    setStatus(CONNECTED);
  }else{
    Serial.println();
    Serial.println("Connection error. Check WiFi settings.");
    setStatus(DISCONNECTED);
  }
}

void reboot(void){
  Serial.println("Rebooting...");
  setStatus(BOOTING);
  delay(5 * 1000);
  ESP.restart();
}

void reConnect(bool toReboot) {
  int resetCounter = 0;
  while (!mqttClient.connected()) {
    setStatus(CONNECTING);
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
    }
    else {
      setStatus(ERROR);
      // http://pubsubclient.knolleary.net/api.html#state
      Serial.print("failed, state=");
      Serial.println(mqttClient.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
    if(resetCounter++ >= 10){
      if(toReboot){
        reboot();
      }else{
        wifiConnected = false;
        return;
      }
    }
  }
  setStatus(CONNECTED);
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
  if(!wifiConnected){
    return;
  }
  reConnect(true);
  mqttClient.loop();
}
void AtomClient::publish(String topic, String body){
  if(!wifiConnected){
    setStatus(CONNECTING);
    return;
  }
  mqttPublish(topic,body);
}
void AtomClient::reboot(void){
  reboot();
}

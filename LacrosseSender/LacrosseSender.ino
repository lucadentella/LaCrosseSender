#include <ArduinoJson.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>
#include "SX127x.h"
#include "lacrosse.h"
#include "globals.h"

// WiFi
const char* wifiSSID = "xxx";
const char* wifiPassword = "xxx";
WiFiClient wifiClient;

// HomeAssistant mosquitto
PubSubClient pubSubClient(wifiClient);
const char* mqttServer = "192.168.1.1";
const char* mqttUsername = "mqtt";
const char* mqttPassword = "mqtt";
const char* mqttTopic = "zigbee2mqtt/Mysensor";

// Preference to store last sensor values
Preferences preferences;

SX127x SX(LORA_CS, LORA_RST);
const int frequency = 868300;
const int dataRate = 17241;
unsigned long lastSend = 0;

float temperature;
int humidity;


void sendPacket() {

  unsigned long now = millis();
  if (now - lastSend > UPDATE_PERIOD * 1000) {

      bool newBattery = false;
      if(now < NEW_BATTERY_PERIOD * 60 * 1000) newBattery = true; 
      uint8_t frame[4];
      LaCrosse::EncodeFrame(frame, newBattery, temperature, false, humidity);
      uint8_t packet[FRAME_LENGTH];
      for(int i = 0; i < 4; i++) packet[i] = frame[i];
      packet[4] = LaCrosse::CalculateCRC(frame, 4);
      SX.Send(packet);
      lastSend = now;
  }  
}

void mqttCallback(char* topic, byte* message, unsigned int length) {

  // Deserialize JSON message
  StaticJsonDocument<256> doc;
  deserializeJson(doc, message, length);

  // Get values from MQTT message, my sensor sends: 
  //{"battery":100,"humidity":90,"humidity_calibration":0,"linkquality":54,"temperature":14.3,"temperature_calibration":0,"temperature_unit":"celsius"}
  temperature = doc["temperature"];
  humidity = doc["humidity"];

  Serial.print("New values from MQTT (temp, hum): ");
  Serial.print(temperature, 1);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.println("%");

  // Store new values in Preferences
  preferences.putFloat("temperature", temperature);
  preferences.putInt("humidity", humidity);
}

void mqttReconnect() {

  Serial.print("Connecting to MQTT server... ");
  if (pubSubClient.connect("LacrosseSender", mqttUsername, mqttPassword)) {
    Serial.println("done!");
    pubSubClient.subscribe(mqttTopic);
    Serial.println("Topic subscribed");
  } else {
    Serial.print("failed, rc=");
    Serial.println(pubSubClient.state());
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println("LacrosseSender");


  // Load values from Preferences
  preferences.begin("lacsend", false);
  temperature = preferences.getFloat("temperature", 20.0);
  humidity = preferences.getInt("humidity", 80);
  Serial.print("Last values from memory (temp, hum): ");
  Serial.print(temperature, 1);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.println("%");

  // SX1276 init
  if (!SX.init()) {
        Serial.println("SX127x init failed! :(");
        while(true)
            delay(1000);
    }
  SX.SetupForLaCrosse();
  SX.SetFrequency(frequency);
  SX.SetDataRate(dataRate);
  Serial.println("SX127x init completed!");

  // WiFi init
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print(" done! IP: ");
  Serial.println(WiFi.localIP());  

  // PubSub init
  pubSubClient.setServer(mqttServer, 1883);
  pubSubClient.setCallback(mqttCallback);
  mqttReconnect();

  Serial.println("Ready to send...");
}

void loop() {

  sendPacket();

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost WiFi connection, reconnecting... ");
    WiFi.disconnect();
    WiFi.reconnect();
  }

  if (!pubSubClient.connected()) {
    mqttReconnect();
  }

  pubSubClient.loop();
}
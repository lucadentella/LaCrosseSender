# LaCrosseSender
Send data from MQTT to a La Crosse weather station

## Description
I wrote this project when the external sensor for my La Crosse weather station failed and I wasn't able to buy a new one.

The goal of this project is to get sensor data from MQTT and send the temperature and humidity values to a La Crosse station using its proprietary protocol, emulating a "real" sensor.

I primarily developed it to integrate the weather station into my home automation system, based on Home Assistant and Zigbee sensors:
![](https://github.com/lucadentella/LaCrosseSender/raw/main/images/schema.png)

## How to use it
First, you need a [TTGO LoRa v1.3](https://www.lilygo.cc/products/lora-v1-3) board to run the Arduino sketch.

Open the [LacrosseSender.ino](https://github.com/lucadentella/LaCrosseSender/blob/main/LacrosseSender/LacrosseSender.ino) file and enter your WiFi settings:

```
// WiFi
const char* wifiSSID = "xxx";
const char* wifiPassword = "xxx";
```

Then you need to enter the IP address or hostname of your MQTT server, username and password and the topic to subscribe:

```
const char* mqttServer = "192.168.1.1";
const char* mqttUsername = "mqtt";
const char* mqttPassword = "mqtt";
const char* mqttTopic = "zigbee2mqtt/Mysensor";
```

You are ready to program the sketch in your board and run it. If you open the Serial Monitor, you should see some debug messages:


You may need to reset your weather station (remove the batteries for a couple of minutes) before being able to receive the information.

## Links
- [My blog](https://www.lucadentella.it/en/2024/11/03/da-home-assistant-a-stazione-meteo-la-crosse/), where I explain this project in details

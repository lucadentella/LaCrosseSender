# LaCrosseSender
Send data from MQTT to a La Crosse weather station

## Description
I wrote this project when the external sensor for my La Crosse weather station failed and I wasn't able to buy a new one.

The goal of this project is to get sensor data from MQTT and send the temperature and humidity values to a La Crosse station using its proprietary protocol, emulating a "real" sensor.

I primarily developed it to integrate the weather station into my home automation system, based on Home Assistant and Zigbee sensors:
![](https://github.com/lucadentella/LaCrosseSender/raw/main/images/schema.png)

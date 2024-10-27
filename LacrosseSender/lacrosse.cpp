#include "lacrosse.h"

/*
* Message Format:
*
* .- [0] -. .- [1] -. .- [2] -. .- [3] -. .- [4] -.
* SSSS.DDDD DDN_.TTTT TTTT.TTTT WHHH.HHHH CCCC.CCCC
* |  | |     ||  |  | |  | |  | ||      | |       |
* |  | |     ||  |  | |  | |  | ||      | `--------- CRC
* |  | |     ||  |  | |  | |  | |`-------- humidity%
* |  | |     ||  |  | |  | |  | `---- weak battery
* |  | |     ||  `--------------- Temperature BCD, T = X/10-40
* |  | |     | `--- new battery
* |  | `-------- sensor ID
* `---- start byte
*
* more details:
* https://github.com/merbanan/rtl_433/blob/master/src/devices/lacrosse_tx35.c
*/

// ------------------------------------ PACKET ENCODING ------------------------------------

void LaCrosse::EncodeFrame(uint8_t frame[], bool newBattery, float temperature, bool weakBattery, int humidity) {

  // convert temperature in BCD format (temperature * 10 + 400)
  byte tempBCD[3];
  int temperatureValue = temperature * 10 + 400;
  tempBCD[0] = temperatureValue / 100;
  tempBCD[1] = (temperatureValue % 100) / 10;
  tempBCD[2] = temperatureValue % 10;

  // first byte = length + 4 bits of Sensor ID
  frame[0] = 0x90 | (SENSOR_ID >> 2);

  // second byte, 2 bits of sensor ID, newBattery indicator, unused bit and temperature
  frame[1] = ((SENSOR_ID & 0x03) << 6) | (newBattery << 5) | (0 << 4) | (tempBCD[0] & 0xF);

  // third byte, temperature
  frame[2] = ((tempBCD[1] & 0xF) << 4) | (tempBCD[2] & 0xF);

  // fourth byte, weakBattery and humidity
  frame[3] = (weakBattery << 7) | (humidity & 0x7F);
}


// ------------------------------------ CRC FUNCTIONS ------------------------------------

byte LaCrosse::UpdateCRC(byte res, uint8_t val) {
    for (int i = 0; i < 8; i++) {
        uint8_t tmp = (uint8_t)((res ^ val) & 0x80);
        res <<= 1;
        if (0 != tmp) {
            res ^= 0x31;
        }
        val <<= 1;
    }
    return res;
}
byte LaCrosse::CalculateCRC(byte *data, byte len) {
    byte res = 0;
    for (int j = 0; j < len; j++) {
        uint8_t val = data[j];
        res = UpdateCRC(res, val);
    }
    return res;
}
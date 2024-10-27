#ifndef _LACROSSE_h
#define _LACROSSE_h

#include "Arduino.h"
#include "globals.h"

class LaCrosse {
public:
    struct Frame {
        uint8_t ID;         /* byte 1 */
        int8_t  humi;       /* byte 2 */
        int8_t  rssi;       /* byte 3 */
        uint8_t init:1;     /* byte 4 */
        uint8_t batlo:1;    /* ordering... */
        uint8_t valid:1;    /* ..is important.. */
        uint8_t pad:5;      /* ...for alignment */
        float   temp;       /* byte 5-8 */
        int     rate;       /* byte 9-12 */
    };
    static void EncodeFrame(uint8_t frame[], bool newBattery, float temperature, bool weakBattery, int humidity);
    static uint8_t UpdateCRC(byte res, uint8_t val);
    static uint8_t CalculateCRC(byte *data, uint8_t len);
};

#endif


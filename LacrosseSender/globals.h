#ifndef _GLOBALS_H
#define _GLOBALS_H

// SPI LoRa Radio
#define LORA_SCK  5   // GPIO5 - SX1276 SCK
#define LORA_MISO 19  // GPIO19 - SX1276 MISO
#define LORA_MOSI 27  // GPIO27 - SX1276 MOSI
#define LORA_CS   18  // GPIO18 - SX1276 CS
#define LORA_RST  14  // GPIO14 - SX1276 RST
#define LORA_IRQ  26  // GPIO26 - SX1276 IRQ (interrupt request)

#define FRAME_LENGTH        5
#define SENSOR_ID           8
#define UPDATE_PERIOD       10
#define NEW_BATTERY_PERIOD  10
#define DISPLAY_ON_PERIOD   30


#endif
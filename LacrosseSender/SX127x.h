#ifndef __SX127x_h
#define __SX127x_h

#include <Arduino.h>
#include "globals.h"
#define PAYLOADSIZE 64

class SX127x {
public:
  enum radio_type {
    Unknown,
    SX127X = 1
  };

  SX127x(byte ss=SS, byte reset=-1);
  bool init();
  void SetupForLaCrosse();
  void SetDataRate(int dataRate);
  void SetFrequency(unsigned long kHz);
  static byte CalculateCRC(byte data[], int len);
  bool ready();
  bool Send(byte data[]);

private:
  byte m_ss, m_reset;
  int m_datarate;
  unsigned long m_frequency;
  bool m_payloadready;
  byte m_payload[PAYLOADSIZE];
  byte m_rssi;

  byte ReadReg(byte addr);
  void WriteReg(byte addr, byte value);
  byte GetByteFromFifo();
  void ClearFifo();

};

/* use header from semtech SDK */
#include "sx1276Regs-Fsk.h"
#endif

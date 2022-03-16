#ifndef INC_COMMS_H_A
#define INC_COMMS_H_A

#include "main.h"
#include <SETUP.h>

// see comms.c for function explanations

void SendPollResponse();
void SendOrigin();
void SendInputs(); // sends
void InitDataLine();
void ReceiveCommand();

void SendByte(uint8_t dataByte);
uint8_t ReceiveByte();
uint8_t ReceiveRumbleByte();
void SendPair(uint8_t sent);
uint8_t ReceivePair();
uint8_t Byte2GC(uint8_t dataByte);
uint8_t GC2Byte(uint8_t dataByte);
void SendStopBit();
void FlushReceiveBuffer();
void SetBaudSend();
void SetBaudReceive();
void SetRumble(uint8_t command);

// extern volatile uint8_t  StYXBA;
// extern volatile uint8_t  LRZDpad; not necessary

extern volatile uint8_t ControlX; // below from main.c
extern volatile uint8_t ControlY;

extern volatile uint8_t CstickX;
extern volatile uint8_t CstickY;

extern volatile uint8_t AnalogL;
extern volatile uint8_t AnalogR;

extern volatile uint8_t DPad;

#endif /* INC_COMMS_H_ */

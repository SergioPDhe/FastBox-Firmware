#ifndef GC_DAC
#define GC_DAC

#include <SETUP.h>
#include "main.h"

extern volatile uint8_t StYXBA;
extern volatile uint8_t LRZDpad;
extern volatile uint8_t ControlX;
extern volatile uint8_t ControlY;
extern volatile uint8_t CstickX;
extern volatile uint8_t CstickY;
extern volatile uint8_t AnalogL;
extern volatile uint8_t AnalogR;

extern volatile uint8_t DPad;

extern bool dPad_on;

volatile uint8_t DAC_ControlX; // variables hold Control stick values while they're not ready to be sent
volatile uint8_t DAC_ControlY;

volatile uint8_t DAC_AnalogR;
// volatile uint8_t  DAC_AnalogL;

void Digital_Analog_Conversion();

uint8_t Cstick2Dpad(uint8_t CStick);
void ControlDAC();
void CStickDAC();
void TriggersDAC();
short int SOCD_TwoIPNoR(uint8_t input, uint8_t *SOCD);

void UpdateAnalog();

short int posX = 0;
short int posY = 0;

short int posCX = 0;
short int posCY = 0;

uint8_t Ctrl_SOCD_X = 0;
uint8_t Ctrl_SOCD_Y = 0;
uint8_t CStk_SOCD_X = 0;
uint8_t CStk_SOCD_Y = 0;

#endif

#ifndef GC_DAC
#define GC_DAC


#include "main.h"
#include "GC_SETUP.h"
//#include "GC_COMM.h"


extern volatile uint8_t  StYXBA;
extern volatile uint8_t  LRZDpad;
extern volatile uint8_t  ControlX;
extern volatile uint8_t  ControlY;
extern volatile uint8_t  CstickX;
extern volatile uint8_t  CstickY;
extern volatile uint8_t  AnalogL;
extern volatile uint8_t  AnalogR;
extern volatile uint8_t  DPad;

extern bool dPad_on;

volatile uint8_t  DAC_ControlX;
volatile uint8_t  DAC_ControlY;

//GCDAC();
void Digital_Analog_Conversion();

uint8_t Cstick2Dpad(uint8_t CStick);
void ControlDAC();
void CStickDAC();
void TriggersDAC();
short int SOCD_TwoIPNoR(uint8_t input, uint8_t *SOCD);

void UpdateAnalog();

uint8_t Ctrl_SOCD_X = 0;
uint8_t Ctrl_SOCD_Y = 0;
uint8_t CStk_SOCD_X = 0;
uint8_t CStk_SOCD_Y = 0;

/*
class GCDAC
{
  public:
    GCDAC();
    void DAC();



    volatile uint8_t  ControlX =  NEUTRAL;
    volatile uint8_t  ControlY =  NEUTRAL;
    volatile uint8_t  CstickX =   NEUTRAL;
    volatile uint8_t  CstickY =   NEUTRAL;
    volatile uint8_t  AnalogL =   0;
    volatile uint8_t  AnalogR =   0;

    
    bool dPad = false;


    uint8_t Cstick2Dpad(uint8_t CStick); 

    //char modkey = 'a';
    //uint8_t out = 0;

  private:
    inline void ControlDAC();
    inline void CStickDAC();
    inline void TriggersDAC();
    inline short int SOCD_TwoIPNoR(uint8_t input, uint8_t *SOCD);

    
    
    uint8_t Ctrl_SOCD_X = 0;
    uint8_t Ctrl_SOCD_Y = 0;
    uint8_t CStk_SOCD_X = 0;
    uint8_t CStk_SOCD_Y = 0;

    
};


*/

#endif

#include <DAC.h>

//uint32_t PB; // debug only
//uint32_t PA;

void Digital_Analog_Conversion() // main DAC function, takes care of updating analog inputs and Dpad
{
//  PB = GPIOB->IDR; // debug
//  PA = GPIOA->IDR;

  ControlDAC(); // DAC for analog sticks + dpad
  TriggersDAC(); // DAC for analog trigger presses

  UpdateAnalog(); // update control stick + dpad value
}

void ControlDAC() // DAC for analog sticks + dpad - mostly based on Crane's DAC software in his fantastic firmware - https://github.com/Crane1195/CL-FW
{		  // some code here was orignially written for slower AVR chips, so its optimized for speed rather than human readability

  posX = SOCD_TwoIPNoR((CTRL_PIN & CTRL_HORIZ), &Ctrl_SOCD_X);			// figure out SOCD for analog sticks
  posY = SOCD_TwoIPNoR((CTRL_PIN & CTRL_VERT) >> CTRL_UP, &Ctrl_SOCD_Y);	// SOCD used is TwoIPNoR

  posCX = SOCD_TwoIPNoR((CSTK_PIN & CSTK_HORIZ) >> CSTK_RIGHT, &CStk_SOCD_X);
  posCY = SOCD_TwoIPNoR((CSTK_PIN & CSTK_VERT) >> CSTK_UP, &CStk_SOCD_Y);


  if (!dPad_on)
  {
    CstickX = NEUTRAL + (MAXOFFSET * posCX); // update C-stick values directly if Dpad mode is off
    CstickY = NEUTRAL + (MAXOFFSET * posCY);
  }
  dPad_on = false;

  uint8_t mod = (CTRL_PIN & CTRL_MOD) >> CTRL_MX; // bit 0 is MODX and bit 1 is MODY

  // check if input is horizontal (0b0001), vertical (0b0010) or diagonal (0b0100)
  uint8_t Direction = 0;

  DAC_ControlX = NEUTRAL;
  DAC_ControlY = NEUTRAL;

  if (posX)
    Direction |= 0b00000001;
  if (posY)
    Direction |= 0b00000010;
  if (Direction == 0b00000011)
    Direction = 0b00000100;


  switch (mod) // check combination of ModX and ModY
  {
  case 0b00000011: // neither pressed ///////////////////////////////////////////////////////////////////////

    switch (Direction) // check direction
    {
    case (0b00000001): // horizontal
      DAC_ControlX += (MAXOFFSET * posX);
      return;

    case (0b00000010): // vertical
      DAC_ControlY += (MAXOFFSET * posY);
      return;

    case (0b00000100): // diagonal
      if ((~BTN_PIN & BTN_LR) || (~SHIELD_PIN & SHIELD_MOD))
      {
        DAC_ControlX += (DIAG_SHIELD_X * posX);
        DAC_ControlY += (DIAG_SHIELD_Y * posY);
        return;
      }
      // ELSE
      DAC_ControlX += (DIAGONAL_X * posX);
      DAC_ControlY += (DIAGONAL_Y * posY);
      return;
    }
    return;
  case 0b00000010: // modx pressed //////////////////////////////////////////////////////////////////////////////////
    switch (Direction) // check direction
    {
    case (0b00000001):             // horizontal /////////////////////////////////
      if (~BTN_PIN & (1 << BTN_B)) // if B is pressed
      {
        DAC_ControlX += (MODX_H_B_X * posX); // side-b nerf
        break;
      }
      // ELSE
      DAC_ControlX += (MODX_H_X * posX);
      break;

    case (0b00000010):             // vertical /////////////////////////////////
      if (~BTN_PIN & (1 << BTN_B)) // if B is pressed
      {
        DAC_ControlY += (MODX_V_B_Y * posY); // side-b nerf
        break;
      }
      // ELSE
      DAC_ControlY += (MODX_V_Y * posY);
      break;

    case (0b00000100):       // diagonal //////////////////////////////////
      if (~BTN_PIN & BTN_LR) // if L/R are pressed
      {
        DAC_ControlX += (MODX_D_SHIELD_X * posX); // MODX wavedash
        DAC_ControlY += (MODX_D_SHIELD_Y * posY);
        break;
      }

      // ELSE
      if (!(~CSTK_PIN & CSTK_CSTK)) // if Cstick buttons are NOT pressed
      {
        DAC_ControlX += (MODX_DIAG_X * posX); // MODX tilt
        DAC_ControlY += (MODX_DIAG_Y * posY);
        break;
      }

      // ELSE
      if (~BTN_PIN & (1 << BTN_B)) // if B is held for extended angles
      {
        if (~CSTK_PIN & (1 << CSTK_DOWN))
        {
          DAC_ControlX += (MODX_D_CD_B_X * posX);
          DAC_ControlY += (MODX_D_CD_B_Y * posY);
          break;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_LEFT))
        {
          DAC_ControlX += (MODX_D_CL_B_X * posX);
          DAC_ControlY += (MODX_D_CL_B_Y * posY);
          break;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_UP))
        {
          DAC_ControlX += (MODX_D_CU_B_X * posX);
          DAC_ControlY += (MODX_D_CU_B_Y * posY);
          break;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_RIGHT))
        {
          DAC_ControlX += (MODX_D_CR_B_X * posX);
          DAC_ControlY += (MODX_D_CR_B_Y * posY);
          break;
        }
      }

      if (~CSTK_PIN & (1 << CSTK_DOWN))
      {
        DAC_ControlX += (MODX_D_CD_X * posX);
        DAC_ControlY += (MODX_D_CD_Y * posY);
        break;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_LEFT))
      {
        DAC_ControlX += (MODX_D_CL_X * posX);
        DAC_ControlY += (MODX_D_CL_Y * posY);
        break;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_UP))
      {
        DAC_ControlX += (MODX_D_CU_X * posX);
        DAC_ControlY += (MODX_D_CU_Y * posY);
        break;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_RIGHT))
      {
        DAC_ControlX += (MODX_D_CR_X * posX);
        DAC_ControlY += (MODX_D_CR_Y * posY);
        break;
      }
    }
    if (posCX)
    {
      CstickX = NEUTRAL + (posCX * MODX_FSMASH_X);
      CstickY = NEUTRAL + (posY * MODX_FSMASH_Y);
    }
    return;

  case 0b00000001: // mody pressed
    switch (Direction) // check direction
    {
    case (0b00000001):           // horizontal /////////////////////////////////
      if (~BTN_PIN & 0b00000010) // if B is pressed
      {
        DAC_ControlX += (MODX_H_B_X * posX); // side-b nerf
        return;
      }
      // ELSE
      DAC_ControlX += (MODX_H_X * posX);
      return;

    case (0b00000010):             // vertical /////////////////////////////////
      if (~BTN_PIN & (1 << BTN_B)) // if B is pressed
      {
        DAC_ControlY += (MODX_V_B_Y * posY); // side-b nerf
        return;
      }
      // ELSE
      DAC_ControlY += (MODX_V_Y * posY);
      return;
    case (0b00000100):       // diagonal //////////////////////////////////
      if (~BTN_PIN & BTN_LR) // if L/R are pressed
      {
        DAC_ControlX += (MODY_D_SHIELD_X * posX); // MODY wavedash
        DAC_ControlY += (MODY_D_SHIELD_Y * posY);
        return;
      }

      // ELSE
      if (!(~CSTK_PIN & CSTK_CSTK)) // if Cstick buttons are NOT pressed
      {
        DAC_ControlX += (MODY_DIAG_X * posX); // MODY tilt
        DAC_ControlY += (MODY_DIAG_Y * posY);
        return;
      }

      // ELSE
      if (~BTN_PIN & (1 << BTN_B)) // if B is held for extended angles
      {
        if (~CSTK_PIN & (1 << CSTK_DOWN))
        {
          DAC_ControlX += (MODY_D_CD_B_X * posX);
          DAC_ControlY += (MODY_D_CD_B_Y * posY);
          return;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_LEFT))
        {
          DAC_ControlX += (MODY_D_CL_B_X * posX);
          DAC_ControlY += (MODY_D_CL_B_Y * posY);
          return;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_UP))
        {
          DAC_ControlX += (MODY_D_CU_B_X * posX);
          DAC_ControlY += (MODY_D_CU_B_Y * posY);
          return;
        }
        // ELSE
        if (~CSTK_PIN & (1 << CSTK_RIGHT))
        {
          DAC_ControlX += (MODY_D_CR_B_X * posX);
          DAC_ControlY += (MODY_D_CR_B_Y * posY);
          return;
        }
      }

      if (~CSTK_PIN & (1 << CSTK_DOWN))
      {
        DAC_ControlX += (MODY_D_CD_X * posX);
        DAC_ControlY += (MODY_D_CD_Y * posY);
        return;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_LEFT))
      {
        DAC_ControlX += (MODY_D_CL_X * posX);
        DAC_ControlY += (MODY_D_CL_Y * posY);
        return;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_UP))
      {
        DAC_ControlX += (MODY_D_CU_X * posX);
        DAC_ControlY += (MODY_D_CU_Y * posY);
        return;
      }
      // ELSE
      if (~CSTK_PIN & (1 << CSTK_RIGHT))
      {
        DAC_ControlX += (MODY_D_CR_X * posX);
        DAC_ControlY += (MODY_D_CR_Y * posY);
        return;
      }
    }
    return;

  case 0b00000000: // both pressed
    dPad_on = true;
  }

  DAC_ControlX = NEUTRAL + (MAXOFFSET * posX);
  DAC_ControlY = NEUTRAL + (MAXOFFSET * posY);
}

void CStickDAC()
{
  CstickX = NEUTRAL + (MAXOFFSET * SOCD_TwoIPNoR((CSTK_PIN & CSTK_HORIZ), &CStk_SOCD_X));
  CstickY = NEUTRAL + (MAXOFFSET * SOCD_TwoIPNoR((CSTK_PIN & CSTK_VERT) >> 2, &CStk_SOCD_Y));
}

void TriggersDAC()
{
  if ((SHIELD_PIN & SHIELD_MOD) == 768)
  {
    AnalogR = 0;
    return;
  }

  if (~SHIELD_PIN & (1 << SHIELD_MS))
  {
    AnalogR = SHIELD_MID;
    return;
  }

  if (~SHIELD_PIN & (1 << SHIELD_LS))
  {
    AnalogR = SHIELD_LIGHT;
    return;
  }
}

uint8_t Cstick2Dpad(uint8_t Stick)
{
  Stick = ~Stick;
  uint8_t Pad = (Stick & 0b00000101) << 1;
  Stick = (Stick & 0b00001010) >> 1;
  return (Pad | Stick);
}

short int SOCD_TwoIPNoR(uint8_t input, uint8_t *SOCD) // input uses bits 0 and 1. 0 for Right/Up and 1 for Left/Down. // Activated inputs are low (e.g. 10 could be an 'up' input and 00 means both opposite inputs are pressed.)
{                                                     // SOCD uses bits 0, 1, 2 and 3. 0 and 1 to check what inputs were pressed; 3 and 4 to check if inputs are locked.
                                                      // this code is a bit difficult to understand because of the binary, but it is very similar to SOCD code found in other open-source box firmwares such as the Crane Firmware: https://github.com/Crane1195/CL-FW

  if (input == 0) // both opposite inputs are pressed
  {
    if (*SOCD & 0b00000001) // if R/U input was previously pressed
    {
      *SOCD |= 0b00000100; // lock R/U input
      return -1;           // return L/D input
    }
    if (*SOCD & 0b00000010) // if L/D input was previously pressed
    {
      *SOCD |= 0b00001000; // lock L/D
      return 1;            // return R/U
    }
  }

  if (input == 0b00000010 && !(*SOCD & 0b00000100)) // R/U input is pressed and not locked
  {
    *SOCD = 0b00000001; // R/U was pressed, all is unlocked
    return 1;           // return R/U
  }

  if (input == 0b00000001 && !(*SOCD & 0b00001000))
  {
    *SOCD = 0b00000010; // L/D was pressed, all is unlocked
    return -1;          // return L/D
  }

  if (input == 0b00000011)
  {
    *SOCD = 0; // nothing is pressed, all is unlocked
    return 0;  // return neutral
  }

  return 0; // when in doubt, return neutral
}

void UpdateAnalog() // updates the Control stick values after DAC is done
{

  ControlX = DAC_ControlX;
  ControlY = DAC_ControlY;

  if (dPad_on)
  {
    DPad = Cstick2Dpad((CSTK_PIN & CSTK_CSTK) >> CSTK_RIGHT);
    CstickX = NEUTRAL;
    CstickY = NEUTRAL;
  }
}

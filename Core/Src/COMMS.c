/*
 * COMMS.c
 *
 *  Created on: Feb 25, 2022
 *      Author: Sergio
 */

#include "COMMS.h"

void ReceiveCommand()
{
  uint8_t command = ReceiveByte();

  switch (command)
  {
    case 0x40:
      ReceiveByte();                      // get second command out of the way
      ReceiveRumbleByte(); // get last 2 bits of 3rd command to check for rumble
      SendInputs();
      break;

    case 0x00:
      FlushReceiveBuffer();
      SendPollResponse();
      break;

    case 0x41:
      FlushReceiveBuffer();
      SendOrigin();
      break;

  }
  FlushReceiveBuffer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendPollResponse() // responds to console poll to check if a controller is plugged in (See GCN communication protocol guides)
{
  SetBaudSend(); // sets UART frame size to 6 (see function for details)

  SendByte(0x09);   // send expected poll response bytes
  SendByte(0x00);
  SendByte(0x03);
  SendStopBit();    // sends stop bit (duh)

  SetBaudReceive(); // flushes buffer and sets UART frame back to 8 (see function for details)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendOrigin() // responds to console poll for origin (See GCN communication protocol guides)
{
  SetBaudSend();  // sets UART frame size to 6 (see function for details)

  SendByte((uint8_t)0x00);     // Sends Start, Y, X, B, A and some overhead stuff, format is usually [0,0,0,St,Y,X,B,A]
  SendByte((uint8_t)0x80);     // Sends L, R, Z and Dpad inputs, format is usually [1,L,R,Z,Dup,Ddown,Dright,Dleft]

  SendByte((uint8_t)128);      // control stick inputs
  SendByte((uint8_t)128);

  SendByte((uint8_t)128);      // c-stick inputs
  SendByte((uint8_t)128);

  SendByte((uint8_t)0x00);     // analog L and R inputs
  SendByte((uint8_t)0x00);

  SendByte((uint8_t)0x00);     // null bytes (expected by console)
  SendByte((uint8_t)0x00);

  SendStopBit();               // stop bit


  SetBaudReceive(); // sets UART frame back to 8 (see function for details)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendInputs() // sends inputs to console
{
  SetBaudSend();

  SendByte(~(GPIOA->IDR) & 0b00011111);   // Sends Start, Y, X, B, A and some overhead stuff, format is usually [0,0,0,St,Y,X,B,A]
  SendByte(((~(GPIOA->IDR) & 0b11100000)>>1)| 0b10000000 | DPad);  // Sends L, R, Z and Dpad inputs, format is usually [1,L,R,Z,Dup,Ddown,Dright,Dleft]

  SendByte(ControlX); // control stick inputs
  SendByte(ControlY);

  SendByte(CstickX);  // c-stick inputs
  SendByte(CstickY);

  SendByte(AnalogL);  // analog L and R inputs
  SendByte(AnalogR);

  SendStopBit();      // stop bit

  FlushReceiveBuffer(); // empties RX buffer

  SetBaudReceive(); // sets UART frame back to 8 (see function for details)
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendByte(uint8_t dataByte) // Sends a full GC byte to the console in pairs of GC bits using 4 UART bytes (REQUIRES 6-BIT FRAME)
{
  SendPair(Byte2GC((dataByte & 0b11000000) >> 6));      //splits byte into pairs, converts them to UART bytes and sends them
  SendPair(Byte2GC((dataByte & 0b00110000) >> 4));
  SendPair(Byte2GC((dataByte & 0b00001100) >> 2));
  SendPair(Byte2GC(dataByte & 0b00000011));

  USART1->SR |= USART_SR_TC; // clear TX_COMPLETE bit by writing a 1 to it (see TXCn in AVR datasheet)
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceiveByte() // Receives 1 full GC byte as 4 UART bytes (REQUIRES 8-BIT FRAME)
{
  uint8_t first, second, third, last;

  first = ReceivePair();    //receive each pair of bytes
  second = ReceivePair();
  third = ReceivePair();
  last = ReceivePair();

  first = GC2Byte(first);   //convert UART bytes into GC bits
  second = GC2Byte(second);
  third = GC2Byte(third);
  last = GC2Byte(last);

  return (first << 6) | (second << 4) | (third << 2) | (last); // return GC byte
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceiveRumbleByte() // Receives 1 full GC byte as 4 UART bytes (REQUIRES 8-BIT FRAME)
{
  //uint8_t first, second, third, last;

  ReceivePair();    //receive each pair of bytes
  ReceivePair();
  ReceivePair();
  SetRumble(GC2Byte(ReceivePair()));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendPair(uint8_t sent)  // sends UART Byte via TX (SHOULD USE 6-BIT FRAME)
{
  //PORTB = (1<<3);
  while (!(USART1->SR & USART_SR_TXE)); //wait for TX buffer to be ready to send data
  USART1->DR = sent;  // send data
  //PORTB = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceivePair()  // receives UART byte via RX (SHOULD USE 8-BIT FRAME)
{
  //PORTB = (1<<2);
  while (!(USART1->SR & USART_SR_RXNE));  // wait for data to be received
  //PORTB = 0;
  return USART1->DR;  // read data
}

uint8_t Byte2GC(uint8_t dataByte) // translates 2 GC bits into one UART byte (SHOULD USE 6-BIT FRAME)
{
  switch (dataByte) // UART sends bits LSB first (unlike GC which sends them MSB first) so these have to be flipped
  {
                          // start & stop bits in []
                          // GC bits      desired output            sent UART       UART input
    case 0: return 0x08;  // 0b00   ->    [0]00010000[1] -> (flip) -> [0]00001000[1]  ->  0x08
    case 1: return 0xE8;  // 0b01   ->    [0]00010111[1] -> (flip) -> [0]11101000[1]  ->  0xE8
    case 2: return 0x0F;  // 0b10   ->    [0]11110000[1] -> (flip) -> [0]00001111[1]  ->  0x0F
    case 3: return 0xEF;  // 0b11   ->    [0]11110111[1] -> (flip) -> [0]11101111[1]  ->  0xEF
  }
}

uint8_t GC2Byte(uint8_t dataByte)  // translates 1 UART byte (8-bit frame) to 2 GC bits
{
  dataByte &= 0b01010010; // we only care about checking the 2nd and 2nd-last in the UART since they correspond with the GC bits (possible values )
                          // the middle bit is checked to see if its a stop bit (1 if stop bit)

  switch (dataByte)
  {
    case 0x00: return 0;
    case 0x40: return 1;
    case 0x02: return 2;
    case 0x42: return 3;
    case 0x52: return 255;  // STOP bit if input is 0b01010010
    default:   return 0;    // default to 00 if something goes wrong
  }
}

void SendStopBit()
{
  SendPair(0xFF); // send (01111111) stop bit to tell console that your message is done
}

void FlushReceiveBuffer() // clear receiver buffer before changing frame size or awaiting new messages from the console
{
  //PORTB = (1<<4);
  while (!(USART1->SR & USART_SR_TC));
  uint32_t dummy;
  while(USART1->SR & USART_SR_RXNE) dummy = USART1->DR; // read data until no more data is in the buffer
  //PORTB = 0;
}

void SetBaudSend() // changes UART frame size to 6 (after receiving a console command and before sending a controller message)
{
   FlushReceiveBuffer();
   USART1->BRR = 0x095;
}

void SetBaudReceive() // changes UART frame size to 8 (after receiving sending a controller message - this is the default frame size for receiving data from the console)
{
   FlushReceiveBuffer();
   while (!(USART1->SR & USART_SR_TC)); // wait for transmit buffer to be clear (i.e. wait for the controller message to be sent)
   USART1->BRR = 0x0C0;
}

void SetRumble(uint8_t command)
{
  //RMBL_PORT = command & 1;
}




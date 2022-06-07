/*
GCN protocol guides:
  https://simplecontrollers.bigcartel.com/gamecube-protocol
  http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
*/

#include "COMMS.h"

void ReceiveCommand() // main communication function - reads incoming console message and replies accordingly
{
  uint8_t command = ReceiveByte(); // read the first byte sent by the console

  switch (command)
  {
  case 0x40:	// Console asks for controller inputs
    ReceiveByte();       // get second command out of the way (useless byte)
    ReceiveRumbleByte(); // get last 2 bits of 3rd command to check for rumble

    SendInputs();	// reply with controller inputs
    break;

  case 0x00:	// console asks if a controller is plugged in
    modeUSB = 0;
    FlushReceiveBuffer(); // flush UART buffer
    SendPollResponse(); // reply to tell console that the controller is now plugged in
    break;

  case 0x41:	// console polls for controller origins
    FlushReceiveBuffer(); // flush UART buffer
    SendOrigin(); // send origins
    break;
  }
  FlushReceiveBuffer(); // flush UART buffer for good measure
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendPollResponse() // responds to console poll to check if a controller is plugged in (See GCN communication protocol guides)
{
  SetBaudSend(); // sets UART frame size to 6 (see function for details)

  SendByte(0x09); // send expected poll response bytes
  SendByte(0x00);
  SendByte(0x03);
  SendStopBit(); // sends stop bit (duh)

  SetBaudReceive(); // flushes buffer sets changes baud to 'receive mode' @1M baud (see function for details)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendOrigin() // responds to console poll for origin (See GCN communication protocol guides)
{
  SetBaudSend(); // sets UART baud to 'send mode' @1.25M baud (see function for details)

  SendByte((uint8_t)0x00); // Sends Start, Y, X, B, A and some overhead stuff, format is usually [0,0,0,St,Y,X,B,A]
  SendByte((uint8_t)0x80); // Sends L, R, Z and Dpad inputs, format is usually [1,L,R,Z,Dup,Ddown,Dright,Dleft]

  SendByte((uint8_t)128); // control stick inputs
  SendByte((uint8_t)128);

  SendByte((uint8_t)128); // c-stick inputs
  SendByte((uint8_t)128);

  SendByte((uint8_t)0x00); // analog L and R inputs
  SendByte((uint8_t)0x00);

  SendByte((uint8_t)0x00); // null bytes (expected by console)
  SendByte((uint8_t)0x00);

  SendStopBit(); // stop bit

  SetBaudReceive(); // flushes buffer sets changes baud to 'receive mode' @1M baud (see function for details)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendInputs() // sends inputs to console
{
  SetBaudSend(); // sets UART baud to 'send mode' @1.25M baud (see function for details)

  SendByte(~(BTN_PIN) & BTN_StYXBA);                              // Sends Start, Y, X, B, A and some overhead stuff, format is usually [0,0,0,St,Y,X,B,A]
  SendByte(((~(BTN_PIN) & BTN_LRZ) >> 1) | 0b10000000 | DPad); // Sends L, R, Z and Dpad inputs, format is usually [1,L,R,Z,Dup,Ddown,Dright,Dleft]

  SendByte(ControlX); // control stick inputs
  SendByte(ControlY);

  SendByte(CstickX); // c-stick inputs
  SendByte(CstickY);

  SendByte(AnalogL); // analog L and R inputs
  SendByte(AnalogR);

  SendStopBit(); // stop bit

  FlushReceiveBuffer();

  SetBaudReceive(); // flushes buffer sets changes baud to 'receive mode' @1M baud (see function for details)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendByte(uint8_t dataByte) // Sends a full GC byte to the console in pairs of GC bits using 4 UART bytes (1.25M baud needed for sending)
{
  SendPair(Byte2GC((dataByte & 0b11000000) >> 6)); // splits byte into pairs, converts them to UART bytes and sends them
  SendPair(Byte2GC((dataByte & 0b00110000) >> 4));
  SendPair(Byte2GC((dataByte & 0b00001100) >> 2));
  SendPair(Byte2GC(dataByte & 0b00000011));

  USART1->SR |= USART_SR_TC; // clear TRANSMIT_COMPLETE bit by writing a 1 to it (see STM32F411 datasheet)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceiveByte() // Receives 1 full GC byte as 4 UART bytes (requires 1M baud for receiving)
{
  uint8_t first, second, third, last;

  first = ReceivePair(); // receive each pair of bytes
  second = ReceivePair();
  third = ReceivePair();
  last = ReceivePair();

  first = GC2Byte(first); // convert UART bytes into GC bits
  second = GC2Byte(second);
  third = GC2Byte(third);
  last = GC2Byte(last);

  return (first << 6) | (second << 4) | (third << 2) | (last); // return GC byte
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceiveRumbleByte() // receives byte and checks last bit for rumble state
{
  // uint8_t first, second, third, last;

  ReceivePair(); // skip first few bytes
  ReceivePair();
  ReceivePair();

  SetRumble(	     // reads UART and sets rumble based on the final console bit
      GC2Byte(		// converts it to UART
	  ReceivePair())); // receives the last pair of bits
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendPair(uint8_t sent) // sends UART Byte via TX (needs 1.25M baud for sending)
{
  while (!(USART1->SR & USART_SR_TXE))
    ;                // wait for TX buffer to be ready to send data

  USART1->DR = sent; // send data
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceivePair() // receives UART byte via RX (SHOULD USE 8-BIT FRAME)
{
  while (!(USART1->SR & USART_SR_RXNE))
    ; // wait for data to be received

  return USART1->DR; // read data
}

uint8_t Byte2GC(uint8_t dataByte) // translates 2 GC bits into one UART byte - NAMED THIS WAY BECAUSE IT IS USED TO SEND DATA TO CONSOLES
{
  switch (dataByte) // UART sends bits LSB first (unlike GC which sends them MSB first) so these have to be flipped
  {
    // start & stop bits in []
		  // GC bits      desired output            sent UART       	UART input
  case 0b00:
    return 0x08; // 0b00   ->    [0]00010000[1] -> (flip) -> [0]00001000[1]  ->  0x08
  case 0b01:
    return 0xE8; // 0b01   ->    [0]00010111[1] -> (flip) -> [0]11101000[1]  ->  0xE8
  case 0b10:
    return 0x0F; // 0b10   ->    [0]11110000[1] -> (flip) -> [0]00001111[1]  ->  0x0F
  case 0b11:
    return 0xEF; // 0b11   ->    [0]11110111[1] -> (flip) -> [0]11101111[1]  ->  0xEF
  }
}

uint8_t GC2Byte(uint8_t dataByte) // translates 1 UART byte to 2 GC bits - NAMED THIS WAY BECAUSE IT IS USED TO RECEIVE DATA FROM CONSOLES
{
  dataByte &= 0b01010010; // we only care about checking the 2nd and 2nd-last in the UART since they correspond with the GC bits (possible values )
                          // the middle bit is checked to see if its a stop bit (1 if stop bit)

  switch (dataByte)
  {
  case 0x00: 	// 0b 0000 0000
    return 0;
  case 0x40:	// 0b 0100 0000
    return 1;
  case 0x02:	// 0b 0000 0010
    return 2;
  case 0x42:	// 0b 0100 0010
    return 3;
  case 0x52:	// 0b 0101 0010
    return 255; // STOP bit if input is 0b 0101 0010
  default:
    return 0; // default to 00 if something goes wrong
  }
}

void SendStopBit()
{
  SendPair(0xFF); // send (01111111) stop bit to tell console that your message is done
}

void FlushReceiveBuffer() // clear receiver buffer before changing frame size or awaiting new messages from the console
{
  while (!(USART1->SR & USART_SR_TC)) // wait for transmitting to be done
    ;
  uint32_t dummy;			// dummy variable

  while (USART1->SR & USART_SR_RXNE)
    dummy = USART1->DR; // read data until no more data is in the buffer
}

//SETTING BAUD RATE - console communication use 5ms bits for console->controller messages and 4ms bits for controller->console messages
//			therefore the baud needs to be adjusted for each case between sending/receiving messages

void SetBaudSend() // changes UART baud rate to 1.25M (after receiving a console command and before sending a controller message - this baud is used for sending)
{
  FlushReceiveBuffer();
  USART1->BRR = 0x095; // set baud to 1.25M
}

void SetBaudReceive() // changes UART Baud rate to 1M (after receiving sending a controller message - this is the default baud for receiving data from the console)
{
  FlushReceiveBuffer();
  while (!(USART1->SR & USART_SR_TC))
    ; // wait for transmit buffer to be clear (i.e. wait for the controller message to be sent)
  USART1->BRR = 0x0C0; // set baud to 1M
}


void SetRumble(uint8_t command) // takes pair of console bits in the form of UART
{
  RMBL_PIN &= ~RMBL_MASK;
  RMBL_PIN |= (RMBL_MASK) * (command & 1);
}

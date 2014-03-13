/*
 * LCD2Line.c
 *
 *  Created on: Jul 2, 2013
 *      Author: Kevin
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "LCD2Line.h"

//#define LCD_LATCH PB2
#define LCD_LATCH PB1
//#define MOSI PB3
#define MOSI PB0
//#define SCK PB5
#define SCK PB2

void LCDinitSPI()
{
	// Outputs MOSI, SCK, SS
	DDRB = (1<<LCD_LATCH)|(1<<MOSI)|(1<<SCK);
	PORTB &= ~(1<<LCD_LATCH);
	// SPI Enable, Master, FCLK/16

#if !defined(__AVR_ATtiny85__)
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
#endif

}

uint8_t LCDwriteSPI(uint8_t data)
{
#if !defined(__AVR_ATtiny85__)
	SPDR=data;
	while(!(SPSR & (1<<SPIF)));

#else
	PORTB &= ~(1<<SCK);

	for (int n=7; n>=0; n--)
	{
		PORTB &= ~(1<<MOSI);
		if(data & (1<<n))
			PORTB |=(1<<MOSI);

		PORTB |= (1<<SCK);
		PORTB &= ~(1<<SCK);
	}

#endif

	PORTB |= (1<<LCD_LATCH);

	PORTB &= ~(1<<LCD_LATCH);

	return data;


}

void LCDSendByte(uint8_t byte, uint8_t rs, uint16_t delay)
{
	uint8_t LCDPins =0;
	if(rs)
		SET_RS(LCDPins);

	LCDPins|=(byte & 0xf0);
	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	LCDPins &=0x0f;
	LCDPins |= (byte <<4) & 0xf0;
	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	_delay_us(delay);
}

void LCDInit()
{
	PORTB &= ~(1<<PB2);

	_delay_ms(100);

	uint8_t LCDPins;

	LCDPins=0x30;
	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	_delay_ms(10);

	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	_delay_ms(10);

	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	_delay_ms(1);

	LCDPins=0x20;
	SET_E(LCDPins);
	LCDwriteSPI(LCDPins);
	CLR_E(LCDPins);
	LCDwriteSPI(LCDPins);
	_delay_ms(1);

	LCDFunctionSet(0,1,0);

    LCDClearDisplay();

	LCDSetDisplay(1,1,1);

	LCDEms(1,0);

}

void LCDClearDisplay()
{
	LCDSendByte(0x01,0,1640);
}

void LCDReturnHome()
{
	LCDSendByte(0x02,0,1640);
}

/* Configures Display and cursor
 * disp=1, display on, =0 display off
 * cursor=1 cursor on, =0 cursor off
 * blink=1, blink on, =0 blink off
 */
void LCDSetDisplay(uint8_t disp, uint8_t cursor, uint8_t blink)
{
	LCDSendByte(0x08 | (disp << 2 ) | (cursor << 1) | blink,0,40);
}

/* Configures Entry mode
 * inc = 1, increment address after write, =0 decrement address after write
 * shift=1 display shift on, =0 display shift off
 */
void LCDEms(uint8_t inc, uint8_t shift)
{
	LCDSendByte(0x04|(inc << 1) | shift, 0 , 40);
}

/* Configures display
 * dl = 1, 8 bit data length, =0 4 bit data length
 * numLines = 1 2 lines, =0 1 line
 * font=1 5x10 dot font, =0 5x8 dot font
 */
void LCDFunctionSet(uint8_t dl, uint8_t numLines, uint8_t font)
{
	LCDSendByte(0x20 | (dl << 4) | (numLines << 3) | (font << 2), 0, 40);
}

void LCDWriteChar(char toSend)
{
	LCDSendByte(toSend,1,45);
}

/* Set Character Gen Address
 * addr = 6 bit address
 */
void LCDSetCGramAddr(uint8_t addr)
{
	LCDSendByte(0x40 | addr,0,40);
}

/* Set Display address
 * addr = 7 bit address
 * 0x40 is the address of the second line
 */
void LCDSetDDramAddr(uint8_t addr)
{
	LCDSendByte(0x80 | addr, 0 , 40);
}

void LCDSendString(char *strPtr)
{
	while(*strPtr)
	{
		LCDWriteChar(*strPtr);
		strPtr++;
	}
}


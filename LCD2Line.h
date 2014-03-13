/*
 * LCD2Line.h
 *
 *  Created on: Jul 23, 2013
 *      Author: Kevin
 */

#ifndef LCD2LINE_H_
#define LCD2LINE_H_

#define RS  0x04    // RS pin
#define E   0x08    // E pin

#define SET_RS(A)  A |= RS
#define CLR_RS(A)  A &= ~RS
#define SET_E(A)   A |= E
#define CLR_E(A)   A &= ~E

void LCDinitSPI();
uint8_t LCDwriteSPI(uint8_t);
void LCDSendByte(uint8_t, uint8_t , uint16_t);
void LCDInit();
void LCDSendString(char *);
void LCDClearDisplay();
void LCDReturnHome();
void LCDSetDisplay(uint8_t, uint8_t, uint8_t);
void LCDEms(uint8_t, uint8_t);
void LCDFunctionSet(uint8_t, uint8_t, uint8_t);
void LCDWriteChar(char);
void LCDSetCGramAddr(uint8_t);
void LCDSetDDramAddr(uint8_t);

#endif /* LCD2LINE_H_ */

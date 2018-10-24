/*
 * LCDDisplay.h
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#ifndef LCDDISPLAY_H_
#define LCDDISPLAY_H_

#define DB0_PORT P4
#define DB0_PIN  BIT0
#define DB1_PORT P4
#define DB1_PIN  BIT1
#define DB2_PORT P4
#define DB2_PIN  BIT2
#define DB3_PORT P4
#define DB3_PIN  BIT3
#define DB4_PORT P4
#define DB4_PIN  BIT4
#define DB5_PORT P4
#define DB5_PIN  BIT5
#define DB6_PORT P4
#define DB6_PIN  BIT6
#define DB7_PORT P4
#define DB7_PIN  BIT7
#define RS_PORT P6
#define RS_PIN  BIT0
#define E_PORT P3
#define E_PIN  BIT2


void SetupPort4();
void SetupSysTick(void);
void SetupLCD();
void ComWrite(uint8_t command);
void DataWrite (uint8_t data);
void pushByte(uint8_t byte);
void pushNibble(uint8_t nibble);
void PulseE();
void delay_micro(uint32_t us);
void delay_milli(uint32_t ms);
void PrintString(char *STRING);
void PrintStringWithLength(char *STRING, int size);


#endif /* LCDDISPLAY_H_ */

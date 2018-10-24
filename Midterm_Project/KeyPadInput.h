/*
 * KeyPadInput.h
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#ifndef KEYPADINPUT_H_
#define KEYPADINPUT_H_

#define ROW0_PORT P3
#define ROW0_PIN  BIT6
#define ROW1_PORT P5
#define ROW1_PIN  BIT2
#define ROW2_PORT P5
#define ROW2_PIN  BIT0
#define ROW3_PORT P1
#define ROW3_PIN  BIT7

#define COL0_PORT P3
#define COL0_PIN  BIT7
#define COL1_PORT P3
#define COL1_PIN  BIT5
#define COL2_PORT P5
#define COL2_PIN  BIT1

int Read_Keypad();
void reset_function(void);
void SetupKeypadPort(void);

#endif /* KEYPADINPUT_H_ */

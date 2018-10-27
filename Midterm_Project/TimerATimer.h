/*
 * TimerATimer.h
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#ifndef TIMERATIMER_H_
#define TIMERATIMER_H_

//Period = (1/Freq) / (ID * (1/3MHz))
#define PERIOD 18750.0

//Period = .02 / (1/3mhz)
#define PERIOD_DOOR 30000.0


void TimerAInit(void);

void GetTimeOnForDoor(float DutyCycle);
void Timer_A0_Door(float timeOn);

void GetTimeOnForLights(float Brightness, int LEDColor);
void Timer_A1_Lights(float timeOn, int LED_);

void GetTimeOnForMotor(float DutyCycle);
void Timer_A1_Motor(float timeOn);


#endif /* TIMERATIMER_H_ */

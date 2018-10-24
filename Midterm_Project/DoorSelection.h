/*
 * DoorSelection.h
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#ifndef DOORSELECTION_H_
#define DOORSELECTION_H_

#define SERVO_PORT P2
#define SERVO_PIN  BIT4


void initDoorLEDs();
int chooseDoor();
void changeDoor(int Input);


#endif /* DOORSELECTION_H_ */

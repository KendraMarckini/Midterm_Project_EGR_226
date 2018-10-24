/*
 * DoorSelection.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "DoorSelection.h"
#include "KeyPadInput.h"
#include "LCDDisplay.h"
#include "TimerATimer.h"
#include "math.h"

int chooseDoor()
{
    int Input = 0;
    char  Open[] = "1   Open Door   ";
    char Close[] = "2   Close Door  ";

    while (!((Input == 1) | (Input == 2)))
    {
        SetupLCD();
        delay_micro(100);

        //prints string on first line on LCD
        PrintString(Open);
        delay_micro(100);

        //Moves cursor to the second line on LCD
        ComWrite(0xC0);
        delay_micro(100);

        //Prints string on second line on LCD
        PrintString(Close);
        delay_micro(100);

        //ask user if they want to open or close door state
        Input = Read_Keypad();
    }

    return Input;
}

void changeDoor(int Input)
{
    //user want to open door
    if(Input == 1)
    {
        //turn on GREEN light
        P2->OUT |=  BIT6;
        //turn off RED light
        P2->OUT &= ~BIT5;

        //open door = 90 degree change
        //1.5/20
        GetTimeOnForDoor(0.09);
    }

    else if(Input == 2)
    {
        //turn on RED light
        P2->OUT |=  BIT5;
        //turn off GREEN light
        P2->OUT &= ~BIT6;

        GetTimeOnForDoor(0.05);
    }
}

//init LEDs in this program
void initDoorLEDs()
{
    P2->SEL0 &= ~(BIT5|BIT6);
    P2->SEL1 &= ~(BIT5|BIT6);
    P2->DIR  |=  (BIT5|BIT6);
    P2->OUT  &= ~(BIT5|BIT6);
}

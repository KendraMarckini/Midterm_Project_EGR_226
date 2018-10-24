//Need to use TimerA for Motor!!

/*
 * MotorSelection.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "stdio.h"
#include "stdlib.h"
#include "MotorSelection.h"
#include "KeyPadInput.h"
#include "LCDDisplay.h"
#include "TimerATimer.h"
#include "string.h"
#include "math.h"

void MotorFunction()
{
    int j=0, KeyPressed = 0, PIN[3]={0};
    float Duty_Cycle = 0;

    char Current[] = "";
    char EnterDutyCycle[] = "Enter Duty Cycle";
    char EndInput[]       = "End Input with *";

    SetupLCD();
    delay_micro(100);

    //print string to first line on LCD
    PrintString(EnterDutyCycle);
    delay_micro(100);

    //Moves cursor to the second line on LCD
    ComWrite(0xC0);
    delay_micro(100);

    //print string to second line on LCD
    PrintString(EndInput);
    delay_milli(1000);

     while(!(KeyPressed == 12))
    {
        //key press detected
        KeyPressed = Read_Keypad();

        //if function will be true if the key pressed is between 0-9
        if(KeyPressed < 10)
        {
            //resets duty cycle to zero
            Duty_Cycle = 0;

            //these lines of code shift the value stored in the array over by 1
            PIN[2]=PIN[1];
            PIN[1]=PIN[0];
            PIN[0]= KeyPressed;


            if(j == 0)
            {
                SetupLCD();
                delay_micro(100);
            }

            sprintf(Current, "%d", KeyPressed);

            PrintStringWithLength(Current, 1);
            delay_micro(100);

            j++;
        }
    }

    //puts the array values as one number and sets it equal to the duty cycle
    Duty_Cycle = PIN[0] + (PIN[1]*10) + (PIN[2]*100);

    //if statement will run if the user presses "#", the duty cycle was between 0 and 100, and at least 1 digit was entered
    if((KeyPressed == 12) && (Duty_Cycle <=100) && (j>0))
    {
        char InputDutyCycle[] ="";
        int length = 0;

        sprintf(InputDutyCycle, " Duty Cycle: %0.0f", Duty_Cycle);
        length = strlen(InputDutyCycle);

        SetupLCD();
        delay_micro(100);

        PrintStringWithLength(InputDutyCycle, length);
        delay_milli(1500);

        //call to MotorTimer.h
        GetTimeOnForMotor(Duty_Cycle);

        //these lines of code will reset the array in this function, along with counter j
        PIN[0]=0;
        PIN[1]=0;
        PIN[2]=0;
        j = 0;
    }

    //this statement will be true if "#" was pressed and either no digits were entered or the duty cycle was gter than 100
    else if((KeyPressed == 12) && ( (j==0) | (!(Duty_Cycle<=100))))
    {
        if(j==0)
        {
            char NotEnter1[] = "A Duty Cycle was";
            char NotEnter2[] = "  not entered.  ";

            SetupLCD();
            delay_micro(100);

            //prints string on first line on LCD
            PrintString(NotEnter1);
            delay_micro(100);

            //Moves cursor to the second line on LCD
            ComWrite(0xC0);
            delay_micro(100);

            //Prints string on second line on LCD
            PrintString(NotEnter2);
            delay_milli(5000);
        }

        if(!(Duty_Cycle<=100))
        {
            char TooLarge1[] = "Duty Cycle is   ";
            char TooLarge2[] = "too big. Must be";
            char TooLarge3[] = "between 0 & 100 ";

            SetupLCD();
            delay_micro(100);

            //prints string on first line on LCD
            PrintString(TooLarge1);
            delay_micro(100);

            //Prints string on third line on LCD
            PrintString(TooLarge3);
            delay_micro(100);

            //Moves cursor to the second line on LCD
            ComWrite(0xC0);
            delay_micro(100);

            //Prints string on second line on LCD
            PrintString(TooLarge2);
            delay_milli(5000);
        }

        //these lines of code will reset the array in this function, along with counter j
        PIN[0]=0;
        PIN[1]=0;
        PIN[2]=0;
        j = 0;

        delay_milli(5000);
    }
}

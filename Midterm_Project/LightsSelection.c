/*
 * LightsSelection.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "stdio.h"
#include "stdlib.h"
#include "LightsSelection.h"
#include "KeyPadInput.h"
#include "LCDDisplay.h"
#include "TimerATimer.h"
#include "string.h"
#include "math.h"


void ChooseLED()
{
    SetupLCD();
    delay_micro(100);

    char     sentence[]=  "  Choose an LED ";
    char          Red[]=  "1      Red      ";
    char        Green[]=  "2     Green     ";
    char         Blue[]=  "3      Blue     ";

    int SelectedLED = 0;

    while((SelectedLED == 0) | (SelectedLED > 3))
    {
        SetupLCD();
        delay_micro(100);

        //prints string on first line on LCD
        PrintString(sentence);
        delay_micro(100);

        //Prints string on third line on LCD
        PrintString(Green);

        //Moves cursor to the second line on LCD
        ComWrite(0xC0);
        delay_micro(100);

        //Prints string on second line on LCD
        PrintString(Red);
        delay_micro(100);

        //Prints string on fourth line on LCD
        PrintString(Blue);
        delay_milli(100);

        SelectedLED = Read_Keypad();

        //if red
        if(SelectedLED == 1)
        {
            ControlBrightness(1);
        }

        //if green
        else if(SelectedLED == 2)
        {
            ControlBrightness(2);
        }

        //if blue
        else if(SelectedLED == 3)
        {
            ControlBrightness(3);
        }
    }
}

void ControlBrightness(int LED_Color)
{
    int j=0, KeyPressed = 0, PIN[3]={0};
    float Brightness = 0;

    char Current[] = "";
    char Brightness1[] = "Enter brightness";
    char Brightness2[] = "between 0 & 100 ";
    char Brightness3[] = "End Input with #";

    SetupLCD();
    delay_micro(100);

    //prints first line on LCD
    PrintString(Brightness1);
    delay_micro(100);

    //Prints third line on LCD
    PrintString(Brightness3);
    delay_micro(100);

    //Moves cursor to the second line on LCD
    ComWrite(0xC0);
    delay_micro(100);

    //Prints second line on LCD
    PrintString(Brightness2);
    delay_milli(100);

    while(!(KeyPressed == 12))
        {
            //key press detected
            KeyPressed = Read_Keypad();

            //if function will be true if the key pressed is between 0-9
            if(KeyPressed < 10)
            {
                //resets duty cycle to zero
                Brightness = 0;

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
    Brightness = PIN[0] + (PIN[1]*10) + (PIN[2]*100);

    //if statement will run if the user presses "#", the duty cycle was between 0 and 100, and at least 1 digit was entered
    if((KeyPressed == 12) && (Brightness <=100) && (j>0))
    {
        char InputBrightness[] ="";
        int length = 0;

        sprintf(InputBrightness, "Brightness: %0.0f", Brightness);
        length = strlen(InputBrightness);

        SetupLCD();
        delay_micro(100);

        PrintStringWithLength(InputBrightness, length);
        delay_milli(1500);

        //call to MotorTimer.h
        GetTimeOnForLights(Brightness, LED_Color);

        //these lines of code will reset the array in this function, along with counter j
        PIN[0]=0;
        PIN[1]=0;
        PIN[2]=0;
        j = 0;
    }

    //this statement will be true if "#" was pressed and either no digits were entered or the duty cycle was gter than 100
    else if((KeyPressed == 12) && ( (j==0) | (!(Brightness<=100))))
    {
        if(j==0)
        {
            char ReEnter1[] = "A Brightness was";
            char ReEnter2[] = "  not entered.  ";

            SetupLCD();
            delay_micro(100);

            //prints string on first line on LCD
            PrintString(ReEnter1);
            delay_micro(100);

            //Moves cursor to the second line on LCD
            ComWrite(0xC0);
            delay_micro(100);

            //Prints string on second line on LCD
            PrintString(ReEnter2);
            delay_milli(1000);
        }

        if(!(Brightness<=100))
        {
            char TooLarge1[] = " Brightness is  ";
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
            delay_milli(1000);
        }

        //these lines of code will reset the array in this function, along with counter j
        PIN[0]=0;
        PIN[1]=0;
        PIN[2]=0;
        j = 0;
    }
}

/*
 * DisplayMenu.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */
#include "msp.h"
#include "DisplayMenu.h"
#include "LCDDisplay.h"
#include "KeyPadInput.h"

int MainMenuDisplay1()
{
    int KeyPRESSED = 0;

    char first[17]=  {'_','_','_','_','_','_','M','e','n','u','1','_','_','_','_','_','\0'};
    char second[17]= {'1',' ',' ',' ',' ',' ',' ','D','o','o','r',' ',' ',' ',' ',' ','\0'};
    char third[17]=  {'2',' ',' ',' ',' ',' ',' ','M','o','t','o','r',' ',' ',' ',' ','\0'};
    char fourth[17]= {'P','u','s','h',' ','*',' ','t','o',' ','s','c','r','o','l','l','\0'};

    //reconfigure LCD to original settings
    SetupLCD();

    //runs forever
    while(1)
    {
        //prints first line on LCD
        PrintString(first);
        //delay_micro(100);

        //Prints third line on LCD
        PrintString(third);
        //delay_micro(100);

        //Moves cursor to the second line on LCD
        ComWrite(0xC0);
        //delay_micro(5);

        //Prints second line on LCD
        PrintString(second);
        //delay_micro(100);

        //Prints fourth line on LCD
        PrintString(fourth);
        delay_micro(1000);

        KeyPRESSED = Read_Keypad();
        return KeyPRESSED;
    }
}

int MainMenuDisplay2()
{
    int KeyPRESSED = 0;

    char fifth[17]=  {'_','_','_','_','_','_','M','e','n','u','2','_','_','_','_','_','\0'};
    char sixth[17]=  {'3',' ',' ',' ',' ',' ','L','i','g','h','t','s',' ',' ',' ',' ','\0'};
    char seventh[17]={'4',' ',' ',' ',' ',' ',' ','B','e','l','l',' ',' ',' ',' ',' ','\0'};
    char eighth[17]= {'P','u','s','h',' ','#',' ','t','o',' ','s','c','r','o','l','l','\0'};

    //reconfigure LCD to original settings
    SetupLCD();

   while(1)
    {
        //prints first line on LCD
        PrintString(fifth);
        //delay_micro(100);

        //Prints third line on LCD
        PrintString(seventh);
        //delay_micro(100);

        //Moves cursor to the second line on LCD
        ComWrite(0xC0);
        //delay_micro(5);

        //Prints second line on LCD
        PrintString(sixth);
        //delay_micro(100);

        //Prints fourth line on LCD
        PrintString(eighth);
        delay_milli(1000);

        KeyPRESSED = Read_Keypad();
        return KeyPRESSED;
    }
}



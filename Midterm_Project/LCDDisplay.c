/*
 * LCDDisplay.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */


#include "msp.h"
#include "LCDDisplay.h"

void PrintStringWithLength(char *STRING, int size)
{
    int i = 0;

    for(i=0; i<size; i++)
    {
        //call function to print the string one character at a time
        DataWrite(STRING[i]);
        //increment to continue while loop
    }
}

//This function prints the string
void PrintString(char *STRING)
{
    int i=0;

    //while loop will loop until it reaches the null value in the string
    while (STRING[i] != '\0')
    {
        //call function to print the string one character at a time
        DataWrite(STRING[i]);
        //increment to continue while loop
        i++;
    }
}

//This function write one byte of commands
void ComWrite(uint8_t command)
{
    //RS to 0 for commands
    RS_PORT->OUT &= ~RS_PIN;

    //call function to pushByte
    pushByte(command);
}

//This function write one byte of data
void DataWrite (uint8_t data)
{
    //RS to 1 for data
    RS_PORT->OUT |= RS_PIN;

    //call function to pushByte
    pushByte(data);
}

//This function splits up the parameter byte into the most significant 4
//bits and 4 least significant bits and sends them one at a time to pushNibble.
void pushByte(uint8_t byte)
{
    //define variables
    uint8_t Nibble;
    //masks 4 least significant bits
    Nibble = (byte & 0xF0) >> 4;
    //call function that sends the most significant 4 bits (higher nibble)
    pushNibble(Nibble);

    //masks 4 most significant bits
    Nibble = byte & 0x0F;
    //call function that sends the least significant 4 bits (lower nibble)
    pushNibble(Nibble);
    //short delay so that there is no overlapping in the program
    delay_micro(100);
}

// This function pushes 1 nibble onto the data pins and pulses the Enable pin
void pushNibble(uint8_t nibble)
{
    //makes pins low
    P4->OUT &= ~(0xF0);
    //write into those pins (P4.3-P4.6 wired to D4-D7)
    P4->OUT |= ((nibble & 0x0F) << 4);

    //function call to Pulse
    PulseE();
}

//this function pulses E
void PulseE()
{
    //make pin low
    E_PORT->OUT &= ~E_PIN;
    delay_micro(10);
    //take pin high
    E_PORT->OUT |= E_PIN;
    delay_micro(10);
    //take pin low
    E_PORT->OUT &= ~E_PIN;
    delay_micro(10);
}

//This function is the SysTick Delay in microseconds
void delay_micro(uint32_t us)
{
    //define variables
    int t;

    //for-loop: this loop will run until the t variable is equal to the value of the variable us
    for(t=0;t<us;t++)
    {
        //1 ms count down to zero - multiplies the delay time by 3 to "convert it to microseconds"
        SysTick -> LOAD = (us*3 - 1);
    }
    //any write to CVR clears it and COUNTFlAG in CSR
    SysTick -> VAL = 0;
    //wait for flag to be Set (Timeout happened)
    while((SysTick -> CTRL & 0x00010000) == 0);
}

//This function is the SysTick Delay in milliseconds
void delay_milli(uint32_t ms)
{
    //1 ms count down to zero - multiplies the delay time by 3000 to "convert it to milliseconds"
    SysTick -> LOAD = ((ms*3000) - 1);
    //any write to CVR clears it and COUNTFlAG in CSR
    SysTick -> VAL = 0;
    //wait for flag to be Set (Timeout happened)
    while((SysTick -> CTRL & 0x00010000) == 0);
}

//This function initializes the LCD
void SetupLCD()
{
    //RS=0
    RS_PORT->OUT &= ~RS_PIN;

    //reset the sequence
    ComWrite(0x03);
    delay_micro(100);
    ComWrite(0x03);
    delay_micro(100);
    ComWrite(0x03);
    delay_micro(100);

    //4 bit mode
    ComWrite(0x02);
    delay_micro(100);
    ComWrite(0x02);

    //blank display
    ComWrite(0x08);
    delay_micro(100);

    //display and blinking cursor on
    ComWrite(0x0F);
    delay_micro(100);

    //clear display and home cursor
    ComWrite(0x01);
    delay_micro(100);

    //increment cursor
    ComWrite(0x06);
    delay_micro(100);
}

//This function sets up the ports for keypad
void SetupPort4()
{
    DB0_PORT->SEL0 &=~ DB0_PIN;
    DB0_PORT->SEL1 &=~ DB0_PIN;
    DB0_PORT->DIR  |=  DB0_PIN;

    DB1_PORT->SEL0 &=~ DB1_PIN;
    DB1_PORT->SEL1 &=~ DB1_PIN;
    DB1_PORT->DIR  |=  DB1_PIN;

    DB2_PORT->SEL0 &=~ DB2_PIN;
    DB2_PORT->SEL1 &=~ DB2_PIN;
    DB2_PORT->DIR  |=  DB2_PIN;

    DB3_PORT->SEL0 &=~ DB3_PIN;
    DB3_PORT->SEL1 &=~ DB3_PIN;
    DB3_PORT->DIR  |=  DB3_PIN;

    DB4_PORT->SEL0 &=~ DB4_PIN;
    DB4_PORT->SEL1 &=~ DB4_PIN;
    DB4_PORT->DIR  |=  DB4_PIN;

    DB5_PORT->SEL0 &=~ DB5_PIN;
    DB5_PORT->SEL1 &=~ DB5_PIN;
    DB5_PORT->DIR  |=  DB5_PIN;

    DB6_PORT->SEL0 &=~ DB6_PIN;
    DB6_PORT->SEL1 &=~ DB6_PIN;
    DB6_PORT->DIR  |=  DB6_PIN;

    DB7_PORT->SEL0 &=~ DB7_PIN;
    DB7_PORT->SEL1 &=~ DB7_PIN;
    DB7_PORT->DIR  |=  DB7_PIN;

    RS_PORT->SEL0 &=~ RS_PIN;
    RS_PORT->SEL1 &=~ RS_PIN;
    RS_PORT->DIR  |=  RS_PIN;

    E_PORT->SEL0 &=~ E_PIN;
    E_PORT->SEL1 &=~ E_PIN;
    E_PORT->DIR  |=  E_PIN;
}

//This function sets up the SysTick Timer used in the program
void SetupSysTick(void)
{
    //disable SysTick during setup
    SysTick ->CTRL = 0;
    //max reload value
    SysTick ->LOAD = 0xFFFFFF;
    //any write to current value clears it
    SysTick ->VAL = 0;
    //enable SysTick, CPU click, no interrupts
    SysTick ->CTRL = 0x5;
}

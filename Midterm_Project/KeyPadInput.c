/*
 * KeyPadInput.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "KeyPadInput.h"

int Read_Keypad()
{
    //define variables
    int button = 15;

    //Column 0 is set to low
    COL0_PORT-> OUT &=~ COL0_PIN;
    COL0_PORT-> DIR |=  COL0_PIN;

        //if row 0 is low
        if(!(ROW0_PORT->IN & ROW0_PIN))
        {
            //button pressed was *
            button = 11;
            //wait for button to be released
            while (!(ROW0_PORT->IN & ROW0_PIN)) {}
        }

        //if row 1 is low
        if(!(ROW1_PORT->IN & ROW1_PIN))
        {
            //button pressed was 7
            button = 7;
            //wait for button to be released
            while (!(ROW1_PORT->IN & ROW1_PIN)) {}
        }

        //if row 2 is low
        if(!(ROW2_PORT->IN & ROW2_PIN))
        {
            //button pressed was 4
            button = 4;
            //wait for button to be released
            while (!(ROW2_PORT->IN & ROW2_PIN)) {}
        }

        //if row 3 is low
        if(!(ROW3_PORT->IN & ROW3_PIN))
        {
            //button pressed was 1
            button = 1;
            //wait for button to be released
            while (!(ROW3_PORT->IN & ROW3_PIN)) {}
        }

    //function resets all pins back to low
    reset_function();

    //set col 1 to low
    COL1_PORT-> OUT &=~ COL1_PIN;
    COL1_PORT-> DIR |=  COL1_PIN;

        //if row 0 is low
        if(!(ROW0_PORT->IN & ROW0_PIN))
        {
            //button pressed was 0
            button = 0;
            //wait for button to be released
            while (!(ROW0_PORT->IN & ROW0_PIN)) {}
        }

        //if row 1 is low
        if(!(ROW1_PORT->IN & ROW1_PIN))
        {
            //button pressed was 8
            button = 8;
            //wait for button to be released
            while (!(ROW1_PORT->IN & ROW1_PIN)) {}
        }

        //if row 2 is low
        if(!(ROW2_PORT->IN & ROW2_PIN))
        {
            //button pressed was 5
            button = 5;
            //wait for button to be released
            while (!(ROW2_PORT->IN & ROW2_PIN)) {}
        }

        //if row 3 is low
        if(!(ROW3_PORT->IN & ROW3_PIN))
        {
            //button pressed was 2
            button = 2;
            //wait for button to be released
            while (!(ROW3_PORT->IN & ROW3_PIN)) {}
        }

    //function resets all pins back to low
    reset_function();

    //set col 2 to low
    COL2_PORT-> OUT &=~ COL2_PIN;
    COL2_PORT-> DIR |=  COL2_PIN;

        //if row 0 is low
        if(!(ROW0_PORT->IN & ROW0_PIN))
        {
            //button pressed was #
            button = 12;
            //wait for button to be released
            while (!(ROW0_PORT->IN & ROW0_PIN)) {}
        }

        //if row 1 is low
        if(!(ROW1_PORT->IN & ROW1_PIN))
        {
            //button pressed was 9
            button = 9;
            //wait for button to be released
            while (!(ROW1_PORT->IN & ROW1_PIN)) {}
        }

        //if row 2 is low
        if(!(ROW2_PORT->IN & ROW2_PIN))
        {
            //button pressed was 6
            button = 6;
            //wait for button to be released
            while (!(ROW2_PORT->IN & ROW2_PIN)) {}
        }

        //if row 3 is low
        if(!(ROW3_PORT->IN & ROW3_PIN))
        {
            //button pressed was 3
            button = 3;
            //wait for button to be released
            while (!(ROW3_PORT->IN & ROW3_PIN)) {}
        }
    //function resets all pins back to low
    reset_function();

    //function returns whihc button was pressed
    return button;
}

//Reset function
void reset_function(void)
{
    ROW0_PORT->DIR &=~ ROW0_PIN;
    ROW0_PORT->OUT |=  ROW0_PIN;
    ROW1_PORT->DIR &=~ ROW1_PIN;
    ROW1_PORT->OUT |=  ROW1_PIN;
    ROW2_PORT->DIR &=~ ROW2_PIN;
    ROW2_PORT->OUT |=  ROW2_PIN;
    ROW3_PORT->DIR &=~ ROW3_PIN;
    ROW3_PORT->OUT |=  ROW3_PIN;

    COL0_PORT->DIR &=~ COL0_PIN;
    COL0_PORT->OUT |=  COL0_PIN;
    COL1_PORT->DIR &=~ COL1_PIN;
    COL1_PORT->OUT |=  COL1_PIN;
    COL2_PORT->DIR &=~ COL2_PIN;
    COL2_PORT->OUT |=  COL2_PIN;
}

//This function sets up the ports for keypad
void SetupKeypadPort(void)
{
    ROW0_PORT->SEL0 &=~ ROW0_PIN;
    ROW0_PORT->SEL1 &=~ ROW0_PIN;
    ROW0_PORT->DIR  &=~ ROW0_PIN;
    ROW0_PORT->REN  |=  ROW0_PIN;
    ROW0_PORT->OUT  |=  ROW0_PIN;

    ROW1_PORT->SEL0 &=~ ROW1_PIN;
    ROW1_PORT->SEL1 &=~ ROW1_PIN;
    ROW1_PORT->DIR  &=~ ROW1_PIN;
    ROW1_PORT->REN  |=  ROW1_PIN;
    ROW1_PORT->OUT  |=  ROW1_PIN;

    ROW2_PORT->SEL0 &=~ ROW2_PIN;
    ROW2_PORT->SEL1 &=~ ROW2_PIN;
    ROW2_PORT->DIR  &=~ ROW2_PIN;
    ROW2_PORT->REN  |=  ROW2_PIN;
    ROW2_PORT->OUT  |=  ROW2_PIN;

    ROW3_PORT->SEL0 &=~ ROW3_PIN;
    ROW3_PORT->SEL1 &=~ ROW3_PIN;
    ROW3_PORT->DIR  &=~ ROW3_PIN;
    ROW3_PORT->REN  |=  ROW3_PIN;
    ROW3_PORT->OUT  |=  ROW3_PIN;

    COL0_PORT->SEL0 &=~ COL0_PIN;
    COL0_PORT->SEL1 &=~ COL0_PIN;
    COL0_PORT->DIR  &=~ COL0_PIN;
    COL0_PORT->REN  |=  COL0_PIN;
    COL0_PORT->OUT  |=  COL0_PIN;

    COL1_PORT->SEL0 &=~ COL1_PIN;
    COL1_PORT->SEL1 &=~ COL1_PIN;
    COL1_PORT->DIR  &=~ COL1_PIN;
    COL1_PORT->REN  |=  COL1_PIN;
    COL1_PORT->OUT  |=  COL1_PIN;

    COL2_PORT->SEL0 &=~ COL2_PIN;
    COL2_PORT->SEL1 &=~ COL2_PIN;
    COL2_PORT->DIR  &=~ COL2_PIN;
    COL2_PORT->REN  |=  COL2_PIN;
    COL2_PORT->OUT  |=  COL2_PIN;
}

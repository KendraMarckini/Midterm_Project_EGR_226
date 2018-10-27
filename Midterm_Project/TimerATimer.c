/*
 * TimerATimer.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "time.h"
#include "TimerATimer.h"

void TimerAInit(void)
{
    //Door
    P2->SEL0 |=  (BIT4);  //setup pwm pin
    P2->SEL1 &=~ (BIT4);  //setup pwm pin
    P2->DIR  |=  (BIT4);  //pwd pin as output


    TIMER_A0->CCR[0]  = PERIOD_DOOR - 1;              //PWM period 40HZ
    TIMER_A0->CTL     = TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__UP |
                        TIMER_A_CTL_CLR |
                        TIMER_A_CTL_ID_1;        //use SMCLK, up mode, clear tar
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode

    /*****************************************************************************************/

    //Motor & Lights
    P7->SEL0 |=  (BIT4|BIT5|BIT6|BIT7);  //setup pwm pin
    P7->SEL1 &=~ (BIT4|BIT5|BIT6|BIT7);  //setup pwm pin
    P7->DIR  |=  (BIT4|BIT5|BIT6|BIT7);  //pwd pin as output


    TIMER_A1->CCR[0]  = PERIOD - 1;              //PWM period 40HZ
    TIMER_A1->CTL     = TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__UP |
                        TIMER_A_CTL_CLR |
                        TIMER_A_CTL_ID_1;        //use SMCLK, up mode, clear tar
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode
    TIMER_A1->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode
    TIMER_A1->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode
    TIMER_A1->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode

    /*****************************************************************************************/

    //Interrupts

    //Lights interrupt
    P6->SEL0 &=~ BIT7;  //setup pwm pin
    P6->SEL1 &=~ BIT7;  //setup pwm pin
    P6->DIR  &=~ BIT7;  //pwd pin as output
    P6->REN  |=  BIT7;      //Enable pull-up resistor
    P6->OUT  |=  BIT7;
    P6->IES  |=  BIT7; //Set pin interrupt to trigger when it goes from high to low
    P6->IE   |=  BIT7; // Enable interrupt
    P6->IFG   =  0;
    NVIC ->ISER[1] = 1 << ((PORT6_IRQn) & 31);

    //Motor Interrupt
    P5->SEL0 &=~ BIT7;  //setup pwm pin
    P5->SEL1 &=~ BIT7;  //setup pwm pin
    P5->DIR  |=  BIT7;  //pwd pin as output
    P5->REN  |=  BIT7;       //Enable pull-up resistor
    P5->OUT  &=~ BIT7;
    P5->IES  |=  BIT7; //Set pin interrupt to trigger when it goes from high to low
    P5->IE   |=  BIT7; // Enable interrupt
    P5->IFG   =  0;
    NVIC ->ISER[1] = 1 << ((PORT5_IRQn) & 31);
}


//Door
void GetTimeOnForDoor(float DutyCycle)
{
    float time_on = 0;

    time_on = DutyCycle * PERIOD_DOOR;

    Timer_A0_Door(time_on);
}
void Timer_A0_Door(float timeOn)
{
    TIMER_A0 -> CCR[1]  = timeOn;                 //CCR4 PWM duty cycle
}


//Lights
void GetTimeOnForLights(float Brightness, int LEDColor)
{
    float time_on = 0;

    time_on = (Brightness /100.0) * PERIOD;

    Timer_A1_Lights(time_on, LEDColor);
}
void Timer_A1_Lights(float timeOn, int LED_)
{
    TIMER_A1 -> CCR[LED_]  = timeOn;
}


//Motor
void GetTimeOnForMotor(float DutyCycle)
{
    float time_on = 0;

    time_on = (DutyCycle /100.0) * PERIOD;

    Timer_A1_Motor(time_on);
}
void Timer_A1_Motor(float timeOn)
{
    TIMER_A1 -> CCR[4]  = timeOn;
}

int LightState = 0;
float Red_Led=0, Green_Led=0, Blue_Led=0;
//Lights Interrupt
void PORT6_IRQHandler(void)
{
    if(P6->IFG & BIT7)
    {
        //turn off lights
        if(LightState == 0)
        {
            Red_Led   = TIMER_A1 -> CCR[1];//red
            Green_Led = TIMER_A1 -> CCR[2];//green
            Blue_Led  = TIMER_A1 -> CCR[3];//blue

            TIMER_A1 -> CCR[1] = 0;//red
            TIMER_A1 -> CCR[2] = 0;//green
            TIMER_A1 -> CCR[3] = 0;//blue
            LightState = 1;
        }
        //turn on lights
        else
        {
            TIMER_A1 -> CCR[1] = Red_Led;//red
            TIMER_A1 -> CCR[2] = Green_Led;//green
            TIMER_A1 -> CCR[3] = Blue_Led;//blue
            LightState = 0;
        }
    }
    P6 -> IFG &=~ BIT7;
}

//Motor Interrupt
void PORT5_IRQHandler(void)
{
    if (P5->IFG & BIT7)
    {
        TIMER_A1 -> CCR[4]  = 0;
    }
    P5 -> IFG  &=~ (BIT7);
}

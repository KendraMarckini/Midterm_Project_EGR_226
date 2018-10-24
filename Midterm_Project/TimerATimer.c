/*
 * TimerATimer.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "time.h"
#include "TimerATimer.h"
#include "LCDDisplay.h"

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

    //Lights
    P7->SEL0 |=  (BIT5|BIT6|BIT7);  //setup pwm pin
    P7->SEL1 &=~ (BIT5|BIT6|BIT7);  //setup pwm pin
    P7->DIR  |=  (BIT5|BIT6|BIT7);  //pwd pin as output


    TIMER_A1->CCR[0]  = PERIOD - 1;              //PWM period 40HZ
    TIMER_A1->CTL     = TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__UP |
                        TIMER_A_CTL_CLR |
                        TIMER_A_CTL_ID_1;        //use SMCLK, up mode, clear tar
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode
    TIMER_A1->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode
    TIMER_A1->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode

    /*****************************************************************************************/

    //Motor
    P5->SEL0 |=  (BIT6);  //setup pwm pin
    P5->SEL1 &=~ (BIT6);  //setup pwm pin
    P5->DIR  |=  (BIT6);  //pwd pin as output

    TIMER_A2->CCR[0]  = PERIOD - 1;              //PWM period 40HZ
    TIMER_A2->CTL     = TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__UP |
                        TIMER_A_CTL_CLR |
                        TIMER_A_CTL_ID_1;        //use SMCLK, up mode, clear tar
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode

    /*****************************************************************************************/

    //Interrupts
    P10->SEL0 |=  (BIT5|BIT6);  //setup pwm pin
    P10->SEL1 &=~ (BIT5|BIT6);  //setup pwm pin
    P10->DIR  |=  (BIT5|BIT6);  //pwd pin as output

    //Bell
    P8->SEL0 |=  (BIT2);  //setup pwm pin
    P8->SEL1 &=~ (BIT2);  //setup pwm pin
    P8->DIR  |=  (BIT2);  //pwd pin as output

    TIMER_A3->CCR[0]  = PERIOD - 1;              //PWM period 40HZ
    TIMER_A3->CTL     = TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__UP |
                        TIMER_A_CTL_CLR |
                        TIMER_A_CTL_ID_1;   //use SMCLK, up mode, clear tar

    TIMER_A3->CCTL[0] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode      //lights
    TIMER_A3->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode      //motor
    TIMER_A3->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;  //CCR4 reset/set mode      //Bell

    TIMER_A3 -> CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A3 -> CCTL[1] = TIMER_A_CCTLN_CCIE;

    TIMER_A3 ->CTL &=~ TIMER_A_CTL_IFG;
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
    TIMER_A1 -> CCR[LED_]  = timeOn;                 //CCR4 PWM duty cycle
}


//Motor
void GetTimeOnForMotor(float DutyCycle)
{
    float time_on = 0;

    time_on = (DutyCycle /100.0) * PERIOD;

    Timer_A2_Motor(time_on);
}
void Timer_A2_Motor(float timeOn)
{
    TIMER_A2 -> CCR[1]  = timeOn;                 //CCR4 PWM duty cycle
}


//Lights Interrupt
void TA3_0_IRQHandler(void)
{
    if(TIMER_A3 -> CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        //Toggle LEDs
        P7 -> OUT ^= (BIT5|BIT6|BIT7);

        //Clear Timer_A3 flag
        TIMER_A3 -> CCTL[0] &=~ TIMER_A_CCTLN_CCIFG;
    }
}

//Motor Interrupt
void PORT6_IRQHandler(void)
{
    if (P6->IFG & BIT6)
    {
        P5 -> OUT ^= BIT6;
    }
    P6 -> IFG  &=~ (BIT6);
}

void initMotorButton()
{
    P6 -> SEL0 &=~ (BIT6);
    P6 -> SEL1 &=~ (BIT6);
    P6 -> DIR  &=~ (BIT6);
    P6 -> REN  |=  (BIT6);
    P6 -> OUT  |=  (BIT6);
    P6 -> IES  |=  (BIT6);
    P6 -> IE   |=  (BIT6);
    P6 -> IFG  &=~ (BIT6);
}






//Bell
void GetTimeOnForBell(float Freq, float duration)
{
    float period = 0;

    period = (1.0/Freq) / (4.0 * (1.0/3000000.0));

    Timer_A3_Bell(period);

    delay_milli(duration);
}
void Timer_A3_Bell(float Period)
{
    TIMER_A3->CCR[0]  = Period;
    TIMER_A3->CCR[2]  = (0.5)*Period;
}

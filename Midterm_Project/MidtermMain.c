//preprocessor directives
#include "msp.h"
#include "stdio.h"
#include "stdlib.h"

#include "DisplayMenu.h"
#include "LCDDisplay.h"
#include "KeyPadInput.h"
#include "DoorSelection.h"
#include "MotorSelection.h"
#include "TimerATimer.h"
#include "LightsSelection.h"
#include "DoorBellSelection.h"
#include "Jingle.h"
//Hi
//states for switch case
enum states{
    Menu1,
    Door,
    Motor,
    Menu2,
    Lights,
    Bell};

//main function
int main()
{
    //stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //disable interrupts
    __disable_irq();

    //inits from DoorSelection.h
    initDoorLEDs();

    //inits from LCDDisplay
    SetupPort4();
    SetupSysTick();
    SetupLCD();

    //inits from Keypad.h
    SetupKeypadPort();

    //inits from TimerATimer.h
    TimerAInit();
    initMotorButton();

    int KeyPress = 0, NewDoor = 0;

    enum states display = Menu1;

    //cursor invisible
    ComWrite(0x0C);

    //set interrupts
    NVIC -> ISER[6] = 1 << ((PORT6_IRQn) & 31);

    //allow interrupts
    __enable_irq();

    while(1)
    {
        switch(display)
        {
            case Menu1:
                KeyPress = MainMenuDisplay1();
                if(KeyPress == 11)
                    display = Menu2;
                else if(KeyPress == 1)
                    display = Door;
                else if(KeyPress == 2)
                    display = Motor;
                break;
            case Door:
                NewDoor = chooseDoor();
                if((NewDoor == 1)|(NewDoor == 2))
                    changeDoor(NewDoor);
                display = Menu1;
                break;
            case Motor:
                MotorFunction();
                display = Menu1;
                break;
            case Menu2:
                KeyPress = MainMenuDisplay1();
                if(KeyPress == 12)
                    display = Menu1;
                else if(KeyPress == 1)
                    display = Lights;
                else if(KeyPress == 2)
                    display = Bell;
                break;
            case Lights:
                ChooseLED();
                display = Menu2;
                break;
            case Bell:
                Song();
                display = Menu2;
                break;
            default:
                display = Menu1;
                break;
        }
    }
}

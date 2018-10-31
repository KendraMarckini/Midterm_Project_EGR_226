/* Name: Kendra Marckini
 * Date: 10/16/2018
 * Class: EGR 226-902
 * Program: Midterm Project!!
 * Note: some code was used from Zuidema, Brakora and possibly Kandalaft*/

//preprocessor directives
#include "msp.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "TimerATimer.h"
//Macros
#define dotted_half_note    3000000
#define half_note           2000000
#define dotted_quarter_note 1500000
#define quarter_note        1000000
#define eighth_note         500000
#define BREATH_TIME         50000
#define REST                0
#define Hd                  1174.7
#define Hd_Flat             1108.7
#define Hc                  1046.5
#define Hb                  987.77
#define Hb_Flat             932.33
#define a                   880.00
#define a_Flat              830.61
#define g                   783.99
#define f_Sharp             739.99
#define f                   689.46
#define e                   659.26
#define d_Sharp             622.25
#define b                   493.88
#define b_Flat              466.16
#define MAX_NOTE            33
#define DB0_PORT P4
#define DB0_PIN  BIT0
#define DB1_PORT P4
#define DB1_PIN  BIT1
#define DB2_PORT P4
#define DB2_PIN  BIT2
#define DB3_PORT P4
#define DB3_PIN  BIT3
#define DB4_PORT P4
#define DB4_PIN  BIT4
#define DB5_PORT P4
#define DB5_PIN  BIT5
#define DB6_PORT P4
#define DB6_PIN  BIT6
#define DB7_PORT P4
#define DB7_PIN  BIT7
#define RS_PORT P6
#define RS_PIN  BIT0
#define E_PORT P3
#define E_PIN  BIT2
#define ROW0_PORT P3
#define ROW0_PIN  BIT6
#define ROW1_PORT P5
#define ROW1_PIN  BIT2
#define ROW2_PORT P5
#define ROW2_PIN  BIT0
#define ROW3_PORT P1
#define ROW3_PIN  BIT7
#define COL0_PORT P3
#define COL0_PIN  BIT7
#define COL1_PORT P3
#define COL1_PIN  BIT5
#define COL2_PORT P5
#define COL2_PIN  BIT1
//global variables
int note = 0;
int breath = 0;
float music_note_sequence[][2] = {{0, half_note},{b, quarter_note},{e, dotted_quarter_note},
                                  {g, eighth_note},{f_Sharp, quarter_note},{e, half_note},
                                  {Hb, quarter_note},{a, dotted_half_note},{f_Sharp, dotted_half_note},
                                  {e, dotted_quarter_note},{g, eighth_note},{f_Sharp, quarter_note},
                                  {d_Sharp, half_note},{f, quarter_note},{b, dotted_half_note},
                                  {0, half_note},{b, quarter_note},{e, dotted_quarter_note},
                                  {g, eighth_note},{f_Sharp, quarter_note},{e, half_note},
                                  {Hb, quarter_note},{Hd, half_note},{Hd_Flat, quarter_note},
                                  {Hc, half_note},{a_Flat, quarter_note},{Hc, dotted_quarter_note},
                                  {Hb, eighth_note},{Hb_Flat, quarter_note},{b_Flat, half_note},
                                  {g, quarter_note},{e, dotted_half_note},{0, dotted_half_note}};
//prototypes
void SetupTimer32s();
void ControlBrightness(int LED_Color);
void ChooseLED();
void MotorFunction();
void initDoorLEDs();
void changeDoor(int Input);
void chooseDoor();
int  Read_Keypad();
void reset_function(void);
void SetupKeypadPort(void);
void PrintStringWithLength(char *STRING, int size);
void SetupPort4();
void SetupSysTick(void);
void SetupLCD();
void ComWrite(uint8_t command);
void DataWrite (uint8_t data);
void pushByte(uint8_t byte);
void pushNibble(uint8_t nibble);
void PulseE();
void delay_micro(uint32_t us);
void delay_milli(uint32_t ms);
void PrintString(char *STRING);
//switch case states
enum states{
    Menu1,Menu1_Options,Menu2,Menu2_Options,
    LightsMenu,SelectLED,Door,
    Motor,
    Bell,
    Lights,
    Door1,
    Door2};

//main function
int main()
{
    //stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    //turn off interrupts
    __disable_interrupt();
    //initialize LCD
    SetupPort4();
    SetupSysTick();
    SetupLCD();
    //Initialize keypad
    SetupKeypadPort();
    //Init TimerA
    TimerAInit();
    //Init Door
    initDoorLEDs();
    //turn on red led
    P2->OUT |= BIT5;
    //enables Timer 32 1 Flag
    NVIC_EnableIRQ(T32_INT1_IRQn);
    //series of character strings that will be displayed on the LCD
    char String1[] = "______Menu 1____";
    char String2[] = "1     Door      ";
    char String3[] = "2    Motor      ";
    char String4[] = "Push * to scroll";
    char String5[] = "______Menu 2____";
    char String6[] = "3    Lights     ";
    char String7[] = "4     Bell      ";
    char String8[] = "Push * to scroll";
    //variable for returning button press
    int keypress = 15;
    //switch case "display" will start in Menu1
    enum states display = Menu1;
    //turn on interrupts
    __enable_interrupt();
    while(1)
    {
        //"display" switch
        switch (display)
        {
            case Menu1:
                //reconfigure LCD to original settings
                SetupLCD();
                //home cursor
                ComWrite(0x02);
                delay_micro(100);
                //print to first line of LCD
                PrintString(String1);
                delay_micro(100);
                //Moves cursor to the second line on LCD
                ComWrite(0xC0);
                delay_micro(100);
                //print string 2 on second line of LCD
                PrintString(String2);
                delay_micro(100);
                //Moves cursor to the third line on LCD
                ComWrite(0x90);
                delay_micro(100);
                //print third string
                PrintString(String3);
                delay_micro(100);
                //Moves cursor to the fourth line on LCD
                ComWrite(0xD0);
                delay_micro(100);
                //print string 2 on second line of LCD
                PrintString(String4);
                delay_micro(100);
                //change state
                display = Menu1_Options;
                break;
            case Menu1_Options:
                //read the button pressed
                keypress = Read_Keypad();
                if(keypress == 1)
                    display = Door;
                else if(keypress == 2)
                    display = Motor;
                else if(keypress == 11)
                    display = Menu2;
                break;
            case Door:
                chooseDoor();
                display = Menu1;
                break;
            case Motor:
                MotorFunction();
                display = Menu1;
                break;
            case Menu2:
                //reconfigure LCD to original settings
                SetupLCD();
                delay_micro(100);
                //home cursor
                ComWrite(0x02);
                delay_micro(100);
                //print to first line of LCD
                PrintString(String5);
                delay_micro(100);
                //Moves cursor to the second line on LCD
                ComWrite(0xC0);
                delay_micro(100);
                //print string 2 on second line of LCD
                PrintString(String6);
                delay_micro(100);
                //Moves cursor to the third line on LCD
                ComWrite(0x90);
                delay_micro(100);
                //print third string
                PrintString(String7);
                delay_micro(100);
                //Moves cursor to the fourth line on LCD
                ComWrite(0xD0);
                delay_micro(100);
                //print string on second line of LCD
                PrintString(String8);
                delay_micro(100);
                //switch case
                display = Menu2_Options;
                break;
            case Menu2_Options:
                //read button press
                keypress = Read_Keypad();
                if(keypress == 3)
                    display = Lights;
                else if(keypress == 4)
                    display = Bell;
                else if(keypress == 11)
                    display = Menu1;
                break;
            case Lights:
                ChooseLED();
                display = Menu2;
                break;
            case Bell:
                SetupTimer32s();
                display = Menu2;
                break;
            default:
                display = Menu1;
                break;
        }
    }
}

//Bell
void T32_INT2_IRQHandler()
{
    TIMER32_2->INTCLR = 1;                                      //Clear interrupt flag so it does not interrupt again immediately.
    if(breath) {                                                //Provides separation between notes
        TIMER_A2->CCR[0] = 0;                                   //Set output of TimerA to 0
        TIMER_A2->CCR[1] = 0;
        TIMER_A2->CCR[2] = 0;
        TIMER32_2->LOAD = BREATH_TIME;                          //Load in breath time to interrupt again
        breath = 0;                                             //Next Timer32 interrupt is no longer a breath, but is a note
    }
    else {                                                      //If not a breath (a note)
        TIMER32_2->LOAD = music_note_sequence[note][1] - 1;     //Load into interrupt count down the length of this note
        if(music_note_sequence[note][0] == REST) {              //If note is actually a rest, load in nothing to TimerA
            TIMER_A2->CCR[0] = 0;
            TIMER_A2->CCR[1] = 0;
            TIMER_A2->CCR[2] = 0;
        }
        else {
            TIMER_A2->CCR[0] = 3000000 / music_note_sequence[note][0];  //Math in an interrupt is bad behavior, but shows how things are happening.  This takes our clock and divides by the frequency of this note to get the period.
            TIMER_A2->CCR[1] = 1500000 / music_note_sequence[note][0];  //50% duty cycle
            TIMER_A2->CCR[2] = TIMER_A0->CCR[0];                        //Had this in here for fun with interrupts.  Not used right now
        }
        note = note + 1;                                                //Next note
        if(note >= MAX_NOTE) {                                          //Go back to the beginning if at the end
            note = 0;
            TIMER32_2->CONTROL = 0b11000011;                //Sets timer 2 for Enabled, Periodic, With Interrupt, No Prescaler, 32 bit mode, One Shot Mode.  See 589 of the reference manual
        }
        breath = 1;                                             //Next time through should be a breath for separation.
    }
}

//bell interrupt
void TA0_N_IRQHandler()
{
    //If CCTL1 is the reason for the interrupt (BIT0 holds the flag)
    if(TIMER_A2->CCTL[1] & BIT0){}
}

//init Timer32
void SetupTimer32s()
{
    P5->SEL0 |=  (BIT6);  //setup pwm pin
    P5->SEL1 &=~ (BIT6);  //setup pwm pin
    P5->DIR  |=  (BIT6);  //pwd pin as output
    TIMER32_2->CONTROL = 0b11100011;                //Sets timer 2 for Enabled, Periodic, With Interrupt, No Prescaler, 32 bit mode, One Shot Mode.  See 589 of the reference manual
    NVIC_EnableIRQ(T32_INT2_IRQn);                  //Enable Timer32_2 interrupt.  Look at msp.h if you want to see what all these are called.
    TIMER32_2->LOAD = 3000000 - 1;                  //Set to a count down of 1 second on 3 MHz clock
    TIMER_A2->CCR[0] = 0;                           // Turn off timerA to start
    TIMER_A2->CCTL[1] = 0b0000000011110100;         // Setup Timer A0_1 Reset/Set, Interrupt, No Output
    TIMER_A2->CCR[1] = 0;                           // Turn off timerA to start
    TIMER_A2->CCR[2] = 0;                           // Turn off timerA to start
    TIMER_A2->CTL = 0b0000001000010100;             // Count Up mode using SMCLK, Clears, Clear Interrupt Flag
    NVIC_EnableIRQ(TA0_N_IRQn);                     // Enable interrupts for CCTL1-6 (if on)
}

//function allows user to choose which LED they want to turn on
void ChooseLED()
{
    //init LCD
    SetupLCD();
    delay_micro(100);
    //define variables
    char sentence[]=  "  Choose an LED ";
    char      Red[]=  "1      Red      ";
    char    Green[]=  "2     Green     ";
    char     Blue[]=  "3      Blue     ";
    int SelectedLED = 0;
    //first case in switch
    enum states lights = LightsMenu;
    //while 1, 2, or 3 are not pressed from the Keypad
    while((SelectedLED == 0) | (SelectedLED > 3))
    {
        //switch case for LEDs
        switch(lights)
        {
            case LightsMenu:
                SetupLCD();
                delay_micro(100);
                //prints string on first line on LCD
                PrintString(sentence);
                delay_micro(100);
                //Prints string on third line on LCD
                PrintString(Green);
                delay_micro(100);
                //Moves cursor to the second line on LCD
                ComWrite(0xC0);
                delay_micro(100);
                //Prints string on second line on LCD
                PrintString(Red);
                delay_micro(100);
                //Prints string on fourth line on LCD
                PrintString(Blue);
                delay_milli(100);
                lights = SelectLED;
                break;
            case SelectLED:
                SelectedLED = Read_Keypad();
                //if red
                if(SelectedLED == 1)
                {
                    ControlBrightness(1);
                    return;
                }
                //if green
                else if(SelectedLED == 2)
                {
                    ControlBrightness(2);
                    return;
                }
                //if blue
                else if(SelectedLED == 3)
                {
                    ControlBrightness(3);
                    return;
                }
                break;
            default:
                lights = LightsMenu;
                break;
        }
    }
}

//function allows user to control the duty cycle or brightness of the LEDs
void ControlBrightness(int LED_Color)
{
    //define variables
    int j=0, KeyPressed = 0, PIN[3]={0};
    float Brightness = 0;
    char Current[] = "";
    char Brightness1[] = "Enter brightness";
    char Brightness2[] = "between 0 & 100 ";
    char Brightness3[] = "End Input with #";
    char InputBrightness[] ="";
    int length = 0;
    char ReEnter1[] = "A Brightness was";
    char ReEnter2[] = "  not entered.  ";
    char TooLarge1[] = " Brightness is  ";
    char TooLarge2[] = "too big. Must be";
    char TooLarge3[] = "between 0 & 100 ";
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
    delay_micro(100);
    //while loop runs until the user is finished with input by pressing "#" or button 12
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
                //reset up the lCD so it can display current input
                SetupLCD();
                delay_micro(100);
            }
            //place current input into an array
            sprintf(Current, "%d", KeyPressed);
            //print the current array / current input
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
            //put the users input into an array
            sprintf(InputBrightness, "Brightness: %0.0f", Brightness);
            //get length of the array
            length = strlen(InputBrightness);
            SetupLCD();
            delay_micro(100);
            //call function to print the array to the LCD
            PrintStringWithLength(InputBrightness, length);
            delay_milli(1500);
            //this function will turn the LED on
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
            //if user did not input anything, let user know
            if(j==0)
            {
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
                delay_milli(3000);
            }
            //if user input is too large, let user know
            if(!(Brightness<=100))
            {
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
                delay_milli(3000);
            }
            //these lines of code will reset the array in this function, along with counter j
            PIN[0]=0;
            PIN[1]=0;
            PIN[2]=0;
            j = 0;
        }
}

//this function allows the user to input the duty cycle
void MotorFunction()
{
    //define variables
    int j=0, KeyPressed = 0, PIN[3]={0};
    float Duty_Cycle = 0;
    char Current[] = "";
    char EnterDutyCycle[] = "Enter Duty Cycle";
    char EndInput[]       = "End Input with #";
    char InputDutyCycle[] ="";
    int length = 0;
    char NotEnter1[] = "A Duty Cycle was";
    char NotEnter2[] = "  not entered.  ";
    char NotEnter3[] = "Please try again";
    char TooLarge1[] = "Duty Cycle is   ";
    char TooLarge2[] = "too big. Must be";
    char TooLarge3[] = "between 0 & 100 ";
    char TooLarge4[] = "Please try again";
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
    delay_micro(100);
    //while the user is still adding input
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
                //reset the LCD to display current input
                SetupLCD();
                delay_micro(100);
            }
            //Place the users input into an array
            sprintf(Current, "%d", KeyPressed);
            //print the users current input
            PrintStringWithLength(Current, 1);
            delay_micro(100);
            j++;
           }
    }
    //puts the array values as one number and sets it equal to the duty cycle
    Duty_Cycle = PIN[0] + (PIN[1]*10) + (PIN[2]*100);
    //if the "#" key was pressed, and the duty cycle is less than 100%
    if((KeyPressed == 12) && (Duty_Cycle <=100) && (j>0))
    {
        //place the duty cycle into an array
        sprintf(InputDutyCycle, " Duty Cycle: %0.0f", Duty_Cycle);
        //get length of the array
        length = strlen(InputDutyCycle);
        SetupLCD();
        delay_micro(100);
        //print users input to LCD
        PrintStringWithLength(InputDutyCycle, length);
        delay_milli(100);
        //function used to control the motor
        GetTimeOnForMotor(Duty_Cycle);
        //these lines of code will reset the array in this function, along with counter j
        PIN[0]=0;
        PIN[1]=0;
        PIN[2]=0;
        j = 0;
    }
    //if "#" is pressed and either no digits were entered or the duty cycle was greater than 100
    else if((KeyPressed == 12) && ( (j==0) | (!(Duty_Cycle<=100))))
    {
        //if no input
        if(j==0)
        {
            SetupLCD();
            delay_micro(100);
            //prints string on first line on LCD
            PrintString(NotEnter1);
            delay_micro(100);
            //prints string on third line on LCD
            PrintString(NotEnter3);
            delay_micro(100);
            //Moves cursor to the second line on LCD
            ComWrite(0xC0);
            delay_micro(100);
            //Prints string on second line on LCD
            PrintString(NotEnter2);
            delay_milli(3000);
        }
        //if duty cycle is greater than 100
        if(!(Duty_Cycle<=100))
        {
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
            delay_micro(100);
            //Prints string on fourth line on LCD
            PrintString(TooLarge4);
            delay_milli(3000);
        }
    }
    //these lines of code will reset the array in this function, along with counter j
    PIN[0]=0;
    PIN[1]=0;
    PIN[2]=0;
    j = 0;
}

//this function asks the user if they would like to open or close the garage door
void chooseDoor()
{
    //define variables
    int Input = 0;
    char  Open[] = "1   Close Door  ";
    char Close[] = "2    Open Door  ";
    //set first case in switch
    enum states door_Case = Door1;
    //loop runs until user inputs either 1 or 2
    while (!((Input == 1) | (Input == 2)))
    {
        switch(door_Case)
        {
            case Door1:
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
                door_Case = Door2;
                break;
            case Door2:
                //read keypad
                Input = Read_Keypad();
                //if user wants to close the door
                if(Input == 1)
                    changeDoor(Input);
                //if user wants to open the door
                if(Input == 2)
                    changeDoor(Input);
                break;
        }
    }
}

//this function powers the servo to change the door
void changeDoor(int Input)
{
    //user want to close door
    if(Input == 1)
    {
        //turn off GREEN light
        P2->OUT &=~ BIT6;
        //turn on RED light
        P2->OUT |= BIT5;
        //function call to change servo position
        GetTimeOnForDoor(0.09);
    }
    //user wants to open the door
    else if(Input == 2)
    {
        //turn off RED light
        P2->OUT &=~ BIT5;
        //turn on GREEN light
        P2->OUT |=  BIT6;
        //function call to change servo position
        GetTimeOnForDoor(0.05);
    }
}

//initializae the garage door LEDs
void initDoorLEDs()
{
    P2->SEL0 &= ~(BIT5|BIT6);
    P2->SEL1 &= ~(BIT5|BIT6);
    P2->DIR  |=  (BIT5|BIT6);
    P2->OUT  &= ~(BIT5|BIT6);
}

//function reads button press and returns the button pressed
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

//Reset function - resets all columns and rows to their initialization state
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

//Function sets up the pins for keypad as GPIO
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

//This function prints the string
void PrintStringWithLength(char *STRING, int size)
{
    //define variables
    int i=0;
    //while loop will loop for the length of the string
    while(i<size)
    {
        //call function to print the string one character at a time
        DataWrite(STRING[i]);
        //increment to continue while loop
        i++;
    }
}

//This function prints the string
void PrintString(char *STRING)
{
    //define variables
    int i=0;
    //while loop will loop for the length of the string
    while(i<16)
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
    //define variables
    int j;
    //for-loop: this loop will run until the j variable is equal to the value of the variable ms
    for(j=0;j<ms;j++)
    {
        //1 ms count down to zero - multiplies the delay time by 3000 to "convert it to milliseconds"
        SysTick -> LOAD = (ms*3000 - 1);
    }
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
    delay_milli(10);
    ComWrite(0x03);
    delay_micro(200);
    ComWrite(0x03);
    delay_milli(100);
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
    //Invisible cursor
    ComWrite(0x0C);
    delay_micro(100);
}

//This function sets up the pins for the LCD
void SetupPort4()
{
    //DB0=BIT0, DB1=BIT1, DB2=BIT2, DB3=BIT3, DB4=BIT4, DB5=BIT5, DB6=BIT6, DB7=BIT7
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
    E_PORT ->SEL0 &=~ E_PIN;
    E_PORT ->SEL1 &=~ E_PIN;
    E_PORT ->DIR  |=  E_PIN;
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

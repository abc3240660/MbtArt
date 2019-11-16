//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

//******************************************************************************
// File:   001_Tick_10ms.h
// Author: Hans Desmet
// Comments: Initial Real time 
// Revision history: Version 01.00
// Date 26/12/2018
//******************************************************************************

#include "001_Tick_10ms.h"
#include "006_Gpio.h"
#include "007_Uart.h"
#include "015_Common.h"

// Must be multiple of 10ms
// Default 10ms ON + 10ms OFF
static u32 gs_beep_all_time = 100;// Beep ON + OFF period
static u32 gs_beep_on_time  = 50; // Beep Only ON time

// Only for blink period if blink enabled
// Must be multiple of 10ms
// Default 100ms PWM + 100ms all OFF
static u32 gs_leds_all_time = 200;// LED PWM ON+IDEL time
static u32 gs_leds_on_time  = 100; // LED PWM ON time

static unsigned long MobitTimesT1 = 0UL;// unit: ms
static unsigned long MobitTimesT2 = 0UL;// unit: ms
static unsigned long MobitTimesT3 = 0UL;// unit: ms
// static unsigned long MobitTimesT4 = 0UL;// unit: ms

// --
// ---------------------- global variables -------------------- //
// --
extern u8 g_ring_times;

//******************************************************************************
//* Timer 1
//* Timer to make 1ms Tick
//******************************************************************************
void Configure_Tick1(void)
{
    TMR1 = 0x00;              // Clear timer register
    T1CONbits.TCKPS = 2;      // Select 1:64 Prescaler

    // Fcy = Fosc/2 = 16M
    // 250*(1/(16M/64)) = 250*4us = 1ms
    PR1 = 250;                // Load the period value

    IPC0bits.T1IP = IPL_MID;  // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;        // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;        // Enable Timer1 interrupt
    T1CONbits.TON = 1;        // Start Timer
}

//******************************************************************************
//* Timer 2
//* Timer to make 10ms Tick
//******************************************************************************
void Configure_Tick2(void)
{
    T2CONbits.T32 = 0;        // Timer2/3 -> Two 16 Bit Timer

    TMR2 = 0x00;              // Clear timer register
    T2CONbits.TCKPS = 2;      // Select 1:64 Prescaler

    // Fcy = Fosc/2 = 16M
    // 2500*(1/(16M/64)) = 2500*4us = 10ms
    PR2 = 2500;               // Load the period value

    IPC1bits.T2IP = IPL_LOW;  // Set Timer2 Interrupt Priority Level
    IFS0bits.T2IF = 0;        // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;        // Enable Timer2 interrupt
    T2CONbits.TON = 1;        // Start Timer
}

//******************************************************************************
//* Timer 3
//* Timer to make 100ms Tick
//******************************************************************************
void Configure_Tick3(void)
{
    T2CONbits.T32 = 0;        // Timer2/3 -> Two 16 Bit Timer

    TMR3 = 0x00;              // Clear timer register
    T3CONbits.TCKPS = 2;      // Select 1:64 Prescaler

    // Fcy = Fosc/2 = 16M
    // 25000*(1/(16M/64)) = 25000*4us = 100ms
    PR3 = 25000;              // Load the period value

    _T3IP = IPL_LOW;          // Set Timer2 Interrupt Priority Level
    _T3IF = 0;                // Clear Timer2 Interrupt Flag
    _T3IE = 1;                // Enable Timer2 interrupt
    T3CONbits.TON = 1;        // Start Timer
}

void Enable_Tick1(void)
{
    TMR1 = 0x00;              // Clear timer register
    PR1 = 250;                // Load the period value
    T1CONbits.TON = 1;
}

void Disable_Tick1(void)
{
    T1CONbits.TON = 0;
}

void Enable_Tick2(void)
{
    TMR2 = 0x00;              // Clear timer register
    PR2 = 2500;               // Load the period value
    T2CONbits.TON = 1;
}

void Disable_Tick2(void)
{
    T2CONbits.TON = 0;
}


void Enable_Tick3(void)
{
    TMR3 = 0x00;              // Clear timer register
    PR3 = 25000;              // Load the period value
    T3CONbits.TON = 1;
}

void Disable_Tick3(void)
{
    T3CONbits.TON = 0;
}

//******************************************************************************
//* Timer 1 IRQ: 1ms
//******************************************************************************
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    MobitTimesT1 += 1;
    if(MobitTimesT1 > 10000000UL){
        MobitTimesT1 = 0;
    }

    if (0 == MobitTimesT1%10000) {
        DEBUG("T1 10s...\n");
    }

    IFS0bits.T1IF = 0;// Clear Timer1 interrupt flag
}

//******************************************************************************
//* Timer 2 IRQ: 10ms
//******************************************************************************
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    static u8 start_flag = 0;
    static u32 loop_time = 0;

    MobitTimesT2 += 1;
    if(MobitTimesT2 > 10000000UL){
        MobitTimesT2 = 0;
    }

#if 1// Beep Ctrl
    if (start_flag) {
        if (loop_time%(gs_beep_all_time/10) < (gs_beep_on_time/10)) {
            Beep_High();
        } else {
            Beep_Low();
        }

        loop_time++;

        if (loop_time >= (gs_beep_all_time/10)) {
            start_flag = 0;
            loop_time = 0;
        }
    } else {
        Beep_Low();
        loop_time = 0;
    }

    // Wait till a whole period's begin
    if (0 == MobitTimesT2%(gs_beep_all_time/10)) {
        if (g_ring_times > 0) {
            g_ring_times--;
            loop_time = 0;
            start_flag = 1;
        }
    }
#endif

#if 1// LEDs Ctrl
    if (0 == MobitTimesT2%2) {// 10ms ON + 10ms OFF
        if (GetLedsStatus(MAIN_LED_B)) {
            if (GetLedsMode(MAIN_LED_B)) {
                if ((MobitTimesT2%(gs_leds_all_time/10) < (gs_leds_on_time/10))) {
                    LEDs_Ctrl(MAIN_LED_B, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_B, LED_ON);
            }
        }

        if (GetLedsStatus(MAIN_LED_R)) {
            if (GetLedsMode(MAIN_LED_R)) {
                if ((MobitTimesT2%(gs_leds_all_time/10) < (gs_leds_on_time/10))) {
                    LEDs_Ctrl(MAIN_LED_R, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_R, LED_ON);
            }
        }

        if (GetLedsStatus(MAIN_LED_G)) {
            if (GetLedsMode(MAIN_LED_G)) {
                if ((MobitTimesT2%(gs_leds_all_time/10) < (gs_leds_on_time/10))) {
                    LEDs_Ctrl(MAIN_LED_G, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_G, LED_ON);
            }
        }
    } else if (1 == MobitTimesT2%2) {
        LEDs_Ctrl(MAIN_LED_B, LED_OFF);
        LEDs_Ctrl(MAIN_LED_R, LED_OFF);
        LEDs_Ctrl(MAIN_LED_G, LED_OFF);
    }
#endif

#if 0// Purple
    if (0 == MobitTimesT2%2) {
        GPIOx_Output(BANKD, MAIN_LED_R, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_R, 0);
    }
    if (0 == MobitTimesT2%2) {
        GPIOx_Output(BANKD, MAIN_LED_B, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_B, 0);
    }
#endif

#if 0// Orange
    if (0 == MobitTimesT2%3) {
        GPIOx_Output(BANKD, MAIN_LED_R, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_R, 0);
    }
    if (0 == MobitTimesT2%5) {
        GPIOx_Output(BANKD, MAIN_LED_G, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_G, 0);
    }
#endif

    if (0 == MobitTimesT2%1000) {
        DEBUG("T2 10s...\n");
    }

    IFS0bits.T2IF = 0;// Clear Timer2 interrupt flag
}

//******************************************************************************
//* Timer 3 IRQ: 100ms
//******************************************************************************
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    static u8 start_flag = 0;
    static u32 loop_time = 0;

    MobitTimesT3 += 1;
    if(MobitTimesT3 > 10000000UL){
        MobitTimesT3 = 0;
    }

    if (0 == MobitTimesT3%100) {
        DEBUG("T3 10s...\n");
    }

    IFS0bits.T3IF = 0;// Clear Timer2 interrupt flag
}

//******************************************************************************************
// FuncName: delay_ms
// Descriptions: delay some time(unit: millisecond)
//           (IN) val: how long to delay
// Return:   NONE
//******************************************************************************************
void delay_ms(unsigned long val)
{
    unsigned long start_time = GetTimeStamp();

    while (!isDelayTimeout(start_time,val));
}

// Delay Unit: 1ms
//******************************************************************************************
// FuncName: delay_ms_nop
// Descriptions: delay some time(unit: millisecond)
//           (IN) val: how long to delay
// Return:   NONE
//******************************************************************************************
void delay_ms_nop(u32 cnt)
{
    u32 i = 0;
    u32 j = 0;

    for (i=0; i<cnt; i++)
        for (j=0; j<1150; j++);
}

// Delay Unit: 1us
//******************************************************************************************
// FuncName: delay_us_nop
// Descriptions: delay some time(unit: millisecond)
//           (IN) val: how long to delay
// Return:   NONE
//******************************************************************************************
void delay_us_nop(u32 cnt)
{
    u32 i = 0;
    u32 j = 0;

    for (i=0; i<cnt; i++)
        for (j=0; j<1; j++);
}

//******************************************************************************************
// FuncName: GetTimeStamp
// Descriptions: get the time stamp after program started
// Return:   number of milliseconds passed since the program started
//******************************************************************************************
unsigned long GetTimeStamp()
{
    return MobitTimesT1;
}
//******************************************************************************************
// FuncName: isDelayTimeout
// Descriptions: determine whether the timeout
// Return:   timeout->true,no timeout->false
//******************************************************************************************
bool isDelayTimeout(unsigned long start_time,unsigned long delayms)
{
    // delay at least 1ms
    if(delayms < 1){
        delayms = 1;
    }
    if(MobitTimesT1 >= start_time){
        if((MobitTimesT1 - start_time) > delayms){
            return 1;
        }
    }else{
        if((10000000UL-start_time+MobitTimesT1) > delayms){
            printf("Timer1 Overload...\n");
            return 1;
        }
    }
    return 0;
}

//******************************************************************************
//* END OF FILE
//******************************************************************************

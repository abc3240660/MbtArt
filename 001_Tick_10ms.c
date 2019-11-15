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

static u32 gs_beep_all_time = 3000;// Beep PWM ON+IDEL time
static u32 gs_beep_on_time  = 2000;// Beep PWM ON time

static u32 gs_leds_all_time = 1360;// LED PWM ON+IDEL time
static u32 gs_leds_on_time  = 680; // LED PWM ON time

static unsigned long MobitTimesT1 = 0UL;// unit: ms
static unsigned long MobitTimesT2 = 0UL;// unit: ms
// static unsigned long MobitTimesT3 = 0UL;// unit: ms
// static unsigned long MobitTimesT4 = 0UL;// unit: ms

// --
// ---------------------- global variables -------------------- //
// --
extern u8 g_ring_times;

//******************************************************************************
//* Timer 1
//* Timer to make 10ms Tick
//******************************************************************************
void Configure_Tick1_10ms(void)
{
    TMR1 = 0x00;                  // Clear timer register

#ifdef OSC_32M_USE
    T1CONbits.TCKPS = 2;          // Select 1:64 Prescaler
    // Fcy = Fosc/2 = 16M
    // 2500*(1/(16M/64)) = 2500*4us = 10ms
    PR1 = 2500;                   // Load the period value
#else// 20MHz
    T1CONbits.TCKPS = 3;          // Select 1:256 Prescaler
    // Fcy = Fosc/2 = 10M
    // 390*(1/(10M/256)) = 9984us = 10ms
    // 3906*(1/(10M/256)) = 100ms
    PR1 = 390;                    //Period = 0.0100096 s; Frequency = 10000000 Hz; PR1 390;
#endif

    IPC0bits.T1IP = IPL_MID;      // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;            // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;            // Enable Timer1 interrupt
    T1CONbits.TON = 1;            // Start Timer
}

//******************************************************************************
//* Timer 2
//* Timer to make 100ms Tick
//******************************************************************************
void Configure_Tick2_10ms(void)
{
    T2CONbits.T32 = 0;
    TMR2 = 0x00;              // Clear timer register

#ifdef OSC_32M_USE
    T2CONbits.TCKPS = 2;      // Select 1:64 Prescaler
    // 36-NG
    // 37*4 = 148us
    PR2 = 37;                 // Load the period value
#else
    T2CONbits.TCKPS = 3;      // Select 1:256 Prescaler
    PR2 = 3906;
#endif

    IPC1bits.T2IP = IPL_LOW;  // Set Timer2 Interrupt Priority Level
    IFS0bits.T2IF = 0;        // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;        // Enable Timer2 interrupt
    T2CONbits.TON = 1;        // Start Timer
}

void Enable_Tick1(void)
{
    TMR1 = 0x00;            // Clear timer register

#ifdef OSC_32M_USE
    PR1 = 25000;            // Load the period value
#else
    PR1 = 3906;             // reload the count
#endif

    T1CONbits.TON = 1;
}

void Disable_Tick1(void)
{
    T1CONbits.TON = 0;
}

void Enable_Tick2(void)
{
    TMR2 = 0x00;            // Clear timer register

#ifdef OSC_32M_USE
    PR2 = 25000;            // Load the period value
#else
    PR2 = 3906;             // reload the count
#endif

    T2CONbits.TON = 1;
}

void Disable_Tick2(void)
{
    T2CONbits.TON = 0;
}

//******************************************************************************
//* Timer 1 IRQ: 10ms
//******************************************************************************
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    MobitTimesT1 += 1;
    if(MobitTimesT1 > 100000UL){
        MobitTimesT1 = 0;
    }

    if (0 == MobitTimesT1%1000) {
        DEBUG("T1 1000 count...\n");
    }

    IFS0bits.T1IF = 0;// Clear Timer1 interrupt flag
}

//******************************************************************************
//* Timer 2 IRQ: 168us
//******************************************************************************
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    static u8 start_flag = 0;
    static u32 loop_time = 0;

    MobitTimesT2 += 1;
    if(MobitTimesT2 > 10000000UL){
        MobitTimesT2 = 0;
    }

    // every 68*148us = 10ms H->L or L->H
    // ===> Period = 136*148us = 20ms -> PWM 50Hz
    if (0 == MobitTimesT2%136) {
        if (GetLedsStatus(MAIN_LED_B)) {
            // Blink: 50Hz PWM ON 100ms then OFF 100ms
            if (GetLedsMode(MAIN_LED_B)) {
                if ((MobitTimesT2%1360 < 680)) {
                    LEDs_Ctrl(MAIN_LED_B, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_B, LED_ON);
            }
        }

        if (GetLedsStatus(MAIN_LED_R)) {
            // Blink: 50Hz PWM ON 100ms then OFF 100ms
            if (GetLedsMode(MAIN_LED_R)) {
                if ((MobitTimesT2%1360 < 680)) {
                    LEDs_Ctrl(MAIN_LED_R, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_R, LED_ON);
            }
        }

        if (GetLedsStatus(MAIN_LED_G)) {
            // Blink: 50Hz PWM ON 100ms then OFF 100ms
            if (GetLedsMode(MAIN_LED_G)) {
                if ((MobitTimesT2%gs_leds_all_time < gs_leds_on_time)) {
                    LEDs_Ctrl(MAIN_LED_G, LED_ON);
                }
            } else {
                LEDs_Ctrl(MAIN_LED_G, LED_ON);
            }
        }
    } else if (68 == MobitTimesT2%136) {
        LEDs_Ctrl(MAIN_LED_B, LED_OFF);
        LEDs_Ctrl(MAIN_LED_R, LED_OFF);
        LEDs_Ctrl(MAIN_LED_G, LED_OFF);
    }

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

#if 0// Purple
    if (0 == MobitTimesT2%3) {
        GPIOx_Output(BANKD, MAIN_LED_R, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_R, 0);
    }
    if (0 == MobitTimesT2%5) {
        GPIOx_Output(BANKD, MAIN_LED_B, 1);
    } else {
        GPIOx_Output(BANKD, MAIN_LED_B, 0);
    }
#endif

#if 1
    if (start_flag) {
        // ON 330ms then OFF 660ms
        if (loop_time%gs_beep_all_time < gs_beep_on_time) {
            if (loop_time%2) {
                Beep_High();
            } else {
                Beep_Low();
            }
        } else {
            Beep_Low();
        }

        loop_time++;

        if (loop_time >= gs_beep_all_time) {
            start_flag = 0;
            loop_time = 0;
        }
    } else {
        Beep_Low();
        loop_time = 0;
    }

    // 148*1500 = 222ms
    // 168*1500 = 252ms
    if (0 == MobitTimesT2%gs_beep_all_time) {
        if (g_ring_times > 0) {
            g_ring_times--;
            loop_time = 0;
            start_flag = 1;
        }
    }
#endif

    if (0 == MobitTimesT2%6000) {
        DEBUG("T2 1000 count...\n");
    }

    IFS0bits.T2IF = 0;// Clear Timer2 interrupt flag
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
    // delay at least 10ms
    if(delayms < 10){
        delayms = 10;
    }
    if(MobitTimesT1 >= start_time){
        if((MobitTimesT1 - start_time) > (delayms/10UL)){
            return 1;
        }
    }else{
        if((100000UL-start_time+MobitTimesT1) > (delayms/10UL)){
            return 1;
        }
    }
    return 0;
}

//******************************************************************************
//* END OF FILE
//******************************************************************************

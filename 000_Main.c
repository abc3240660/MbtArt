//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

/******************************************************************************
 * Entrance of Application
 * This file is entrance of APP
 *
 * Copyright (c) 2019 Mobit technology inc.
 * @Author       : Damon
 * @Create time  : 03/11/2019
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <p24fxxxx.h>

#include "001_Tick_10ms.h"
#include "002_CLRC663.h"
#include "004_LB1938.h"
#include "005_BNO055.h"
#include "006_Gpio.h"
#include "007_Uart.h"
#include "009_System.h"
#include "012_CLRC663_NFC.h"
#include "019_ADC0.h"

#define TESTVER "11182250"

// static u8 is_mode_nb = 0;
static u32 start_time_nfc = 0;
static u8 gs_charge_sta = 0;
static u8 gs_got_card = 0;

u32 g_led_times = 0;
u8 g_ring_times = 0;

// 0-normal 1-move interrupt
u8 g_bno055_move = 0;

static void PowerOffBG96Module(void)
{
    _ANSB9 = 0;

    GPIOx_Config(BANKB, 9, OUTPUT_DIR);// PWRKEY
    GPIOx_Output(BANKB, 9, 0);// default SI2302 HIGH
    delay_ms_nop(100);
    GPIOx_Output(BANKB, 9, 1);// MCU High -> SI2302 Low  -> PowerOff
    delay_ms_nop(800);// SI2302 Low >= 0.65s :  On -> Off
    GPIOx_Output(BANKB, 9, 0);// release to default SI2302 HIGH
}

void SwitchToLowClock(void)
{
    _GIE = 0;
    __builtin_write_OSCCONH(5);// LPRC
    __builtin_write_OSCCONL(1);// 32MHz change to 32KHz
    while(OSCCONbits.OSWEN);
    _GIE = 1;
}

void SwitchToNormalClock(void)
{
    _GIE = 0;

#ifdef EXT_CRYSTAL
    __builtin_write_OSCCONH(3);// PRIPLL
#else
    __builtin_write_OSCCONH(1);// FRCPLL
#endif

    __builtin_write_OSCCONL(1);// 32KHz change to 32MHz
    while(OSCCONbits.OSWEN);
    _GIE = 1;
}

void EnterToSleep(void)
{
    Sleep();
    Nop();
}

int main(void)
{
    float cur_pitch = 0;
    float cur_yaw = 0;
    float cur_roll = 0;

    u8 try_cnt = 0;
    u8 nfc_ret = 0;
    u8 net_sta = 0;
    u8 nfc_enable = 0;

    u32 adc_val = 0;
    u32 task_cnt = 0;
    u16 nfc_time = 10;
	
    u8 intr_type = 0;

    // 0-normal 1-low power
    u8 bno055_mode = 0;

    System_Config();

    Uart1_Init();
	Uart3_Init();
    Uart4_Init();

    delay_ms_nop(100);
    
    DEBUG("ART Application running...\r\n");

#if 0// TypeC Check
    while (1) {
        if (Charge_InsertDetect()) {
            break;
        }

        delay_ms_nop(2000);
    }
#endif

    // If TypeC-Power detect, beep*1
    g_ring_times = 1;
    Configure_Tick1();
    Configure_Tick2();

    Beep_Init();
    // PowerOffBG96Module();

    LEDs_Init();
    Charge_Init();
    LB1938_Init();
    LockSwitch_Init();

    // CLRC663_PowerUp();
    BNO055_PowerUp();

#if 0// Orange
    SetLedsMode(MAIN_LED_O, LED_BLINK);
    SetLedsStatus(MAIN_LED_O, LED_ON);
#endif

#if 0// Purple
    SetLedsMode(MAIN_LED_P, LED_BLINK);
    SetLedsStatus(MAIN_LED_P, LED_ON);
#endif
#if 1// BNO055 Testing
	BNO055_init();

#if 0
	if (0 == bno055_get_euler(&cur_pitch, &cur_yaw, &cur_roll)) {
		DEBUG("Euler Base: %f %f %f\n", (double)cur_pitch, (double)cur_yaw, (double)cur_roll);
	} else {
		DEBUG("Get base eurl failed \n");
    }
#endif

    ExtIntr_Initialize();
    bno055_clear_int();// clear INT
#endif

    while(1)
    {
#if 0
        if (g_bno055_move) {// BNO055 wakeup
            if (IsLockSwitchOpen()) {
                Sleep();
                Nop();
            } else {
                nfc_enable = 1;
            }
        } else {// WDT wakeup for Charge Detect
            if (Charge_InsertDetect()) {
                gs_charge_sta |= 0x80;

                ADC0_Init();

                if (ADC0_GetValue(&adc_val)) {
                    if (adc_val > 1000) {
                        Charge_Disable();
                        SetLedsStatus(MAIN_LED_G, LED_ON);
                    } else {
                        SetLedsMode(MAIN_LED_B, LED_BLINK);
                        SetLedsStatus(MAIN_LED_B, LED_ON);
                    }
                }

                ADC0_Disable();
            } else {
                if (0x80 == gs_charge_sta) {
                    gs_charge_sta = 0;
                    SetLedsMode(MAIN_LED_B, LED_BLINK);
                    SetLedsStatus(MAIN_LED_B, LED_OFF);
                    SetLedsStatus(MAIN_LED_G, LED_OFF);
                }
            }
        }
#endif

#if 1
        if (!g_ring_times && !g_led_times) {
            Disable_Tick2();
        }

        // -- use accuate time gap to hbeat
        // ---------------------- H Beat -------------------- //
        // --
        if (1 == nfc_enable) {
            if (0 == start_time_nfc) {
                DEBUG("start_time_nfc ...\n");
                g_ring_times = 2;
                g_led_times = 10000;
                SetLedsStatus(MAIN_LED_P, LED_ON);
                Enable_Tick2();
                start_time_nfc = GetTimeStamp();
            }

            if (start_time_nfc != 0) {
                if (isDelayTimeout(start_time_nfc,nfc_time*1000UL)) {
                    nfc_enable = 0;
                    DEBUG("nfc_enable = 0\n");

                    g_ring_times = 3;
                    Enable_Tick2();

                    g_led_times = 0;
                    delay_ms(15);

                    g_led_times = 5000;
                    SetLedsStatus(MAIN_LED_R, LED_ON);
                    // SetLedsStatus(MAIN_LED_G, LED_OFF);
                    // SetLedsStatus(MAIN_LED_B, LED_OFF);
                }
            }
        } else {
            start_time_nfc = 0;
        }
#endif
        if (0 == (task_cnt++%200)) {
        }

        // --
        // ---------------------- TASK 1 -------------------- //
        // --
        if (0 == (task_cnt%2)) {// every 0.1s
        }

        // --
        // ---------------------- TASK 2 -------------------- //
        // --
        if (0 == (task_cnt%22)) {  // every 0.5s
            if (nfc_enable) {
                DEBUG("before ReadMobibNFCCard\n");
                if (0 == ReadMobibNFCCard()) {
                    g_ring_times = 1;
                    Enable_Tick2();

                    LB1938_OpenLock();

                    g_led_times = 0;
                    delay_ms(15);

                    g_led_times = 3000;
                    SetLedsStatus(MAIN_LED_G, LED_ON);

                    nfc_enable = 0;
                }
                DEBUG("after ReadMobibNFCCard\n");
            }
        }

        // --
        // ---------------------- TASK 3 -------------------- //
        // --

        if (0 == (task_cnt%21)) {  // every 1.0s
            if (1 == GetBNOIntrFlag()) {
                // INT occur
                ClearBNOIntrFlag();
                DEBUG("New Intr Occur...\n");
                intr_type = bno055_get_int_src();//get the INT source
                
                DEBUG("Intr Reg = %.2X\n", intr_type);
                bno055_clear_int();//clear INT
                
                if (!nfc_enable) {
                    if (intr_type&0x80) {
                        bno055_mode = 1;// During Low Power Mode
                        Disable_Tick2();

                        CLRC663_PowerOff();
                        DEBUG("Enter low power %s\n", TESTVER);
                        bno055_enter_lower_mode();

                        if (0 == GetBNOIntrFlag()) {
                            SwitchToLowClock();

                            Sleep();
                            Nop();

#if 0
                            LEDs_Ctrl(MAIN_LED_G, LED_ON);
                            delay_us_nop(100);
                            LEDs_Ctrl(MAIN_LED_G, LED_OFF);
#endif

                            SwitchToNormalClock();

                            // Enable_Tick1();
                            DEBUG("Wake Up...\r\n");
                        } else {
                            DEBUG("Intr happen again...\n");
                        }

                        Enable_Tick2();
                    } else if (intr_type&0x40) {
                        DEBUG("Enter normal %s\n", TESTVER);
                        bno055_enter_normal_mode();//
                        bno055_mode = 0;// During Normal Mode

                        if (!IsLockSwitchOpen()) {
                            CLRC663_PowerUp();
                            nfc_enable = 1;
                            DEBUG("Ready to read NFC...\n");
                        } else {
                            CLRC663_PowerOff();
                            DEBUG("Lock is already open.\n");
                        }
                    }
                }
            } else if (0 == bno055_mode) {//
#if 0
                if (0 == bno055_euler_check(cur_pitch, cur_yaw, cur_roll))
                    DEBUG("Euler OK\n");
                else
                    DEBUG("Euler NG\n");
#endif
            }
        }

        // --
        // ---------------------- TASK 4 -------------------- //
        // --
        if (0 == (task_cnt%31)) {  // every 1.5s
        }

        // --
        // ---------------------- TASK 5 -------------------- //
        // --
        if (0 == (task_cnt%41)) {  // every 2.0s
        }

        // --
        // ---------------------- TASK 6 -------------------- //
        // --
        if (0 == (task_cnt%99)) {  // every 5.0s
        }

        // -- just for engineer testing
        // ---------------------- TASK 7 -------------------- //
        // --
        if (0 == (task_cnt%199)) { // every 10.0s
            DEBUG("task active %s\n", TESTVER);
        }

        if (10000 == task_cnt) {
            task_cnt = 0;
        }

        if (0 == (task_cnt%20)) {// every 1.0s
        }

        delay_ms(50);
    }

    return 0;
}
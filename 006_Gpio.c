//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

/******************************************************************************
 * A library for PIC24F GPIO Configuration
 * This file is about the GPIO API of PIC24
 *
 * Copyright (c) 2019 Mobit technology inc.
 * @Author       : Damon
 * @Create time  : 03/11/2019
******************************************************************************/

#include <stdio.h>
#include <p24fxxxx.h>
#include "001_Tick_10ms.h"
#include "006_Gpio.h"

// #define LED_DEBUG	1

u8 gs_leds_sta = 0;

// 0-normal 1-blink
u8 gs_leds_mod = 0;

void GPIOx_Config(GPIO_BANKx port, u8 pin, GPIO_DIR dir)
{
    if (BANKB == port) {
        if(dir){
            TRISB |= (1<<pin);
        } else {
            TRISB &= ~(1<<pin);
        }
    } else if (BANKC == port) {
        if(dir){
            TRISC |= (1<<pin);
        } else {
            TRISC &= ~(1<<pin);
        }
    } else if (BANKD == port) {
        if(dir){
            TRISD |= (1<<pin);
        } else {
            TRISD &= ~(1<<pin);
        }
    } else if (BANKE == port) {
        if(dir){
            TRISE |= (1<<pin);
        } else {
            TRISE &= ~(1<<pin);
        }
    } else if (BANKF == port) {
        if(dir){
            TRISF |= (1<<pin);
        } else {
            TRISF &= ~(1<<pin);
        }
    } else if (BANKG == port) {
        if(dir){
            TRISG |= (1<<pin);
        } else {
            TRISG &= ~(1<<pin);
        }
    }
}

void GPIOx_Pull(GPIO_BANKx port, u8 pin, GPIO_PUL value)
{
    if (BANKB == port) {
        if(PULL_UP == value){
            IOCPDB &= ~(1<<pin);
            IOCPUB |= (1<<pin);
        } else {
            IOCPUB &= ~(1<<pin);
            IOCPDB |= (1<<pin);
        }
    } else if (BANKC == port) {
        if(PULL_UP == value){
            IOCPDC &= ~(1<<pin);
            IOCPUC |= (1<<pin);
        } else {
            IOCPUC &= ~(1<<pin);
            IOCPDC |= (1<<pin);
        }
    } else if (BANKD == port) {
        if(PULL_UP == value){
            IOCPDD &= ~(1<<pin);
            IOCPUD |= (1<<pin);
        } else {
            IOCPUD &= ~(1<<pin);
            IOCPDD |= (1<<pin);
        }
    } else if (BANKE == port) {
        if(PULL_UP == value){
            IOCPDE &= ~(1<<pin);
            IOCPUE |= (1<<pin);
        } else {
            IOCPUE &= ~(1<<pin);
            IOCPDE |= (1<<pin);
        }
    } else if (BANKF == port) {
        if(PULL_UP == value){
            IOCPDF &= ~(1<<pin);
            IOCPUF |= (1<<pin);
        } else {
            IOCPUF &= ~(1<<pin);
            IOCPDF |= (1<<pin);
        }
    } else if (BANKG == port) {
        if(PULL_UP == value){
            IOCPDG &= ~(1<<pin);
            IOCPUG |= (1<<pin);
        } else {
            IOCPUG &= ~(1<<pin);
            IOCPDG |= (1<<pin);
        }
    }
}

void GPIOx_Output(GPIO_BANKx port, u8 pin, u8 value)
{
    if (BANKB == port) {
        if(value){
            LATB |= (1<<pin);
        } else {
            LATB &= ~(1<<pin);
        }
    } else if (BANKC == port) {
        if(value){
            LATC |= (1<<pin);
        } else {
            LATC &= ~(1<<pin);
        }
    } else if (BANKD == port) {
        if(value){
            LATD |= (1<<pin);
        } else {
            LATD &= ~(1<<pin);
        }
    } else if (BANKE == port) {
        if(value){
            LATE |= (1<<pin);
        } else {
            LATE &= ~(1<<pin);
        }
    } else if (BANKF == port) {
        if(value){
            LATF |= (1<<pin);
        } else {
            LATF &= ~(1<<pin);
        }
    } else if (BANKG == port) {
        if(value){
            LATG |= (1<<pin);
        } else {
            LATG &= ~(1<<pin);
        }
    }
}

u8 GPIOx_Input(GPIO_BANKx port, u8 pin)
{
    if (BANKB == port) {
        if (PORTB & (1<<pin)) {
            return 1;
        }
    } else if (BANKC == port) {
        if (PORTC & (1<<pin)) {
            return 1;
        }
    } else if (BANKD == port) {
        if (PORTD & (1<<pin)) {
            return 1;
        }
    } else if (BANKE == port) {
        if (PORTE & (1<<pin)) {
            return 1;
        }
    } else if (BANKF == port) {
        if (PORTF & (1<<pin)) {
            return 1;
        }
    } else if (BANKG == port) {
        if (PORTG & (1<<pin)) {
            return 1;
        }
    }

    return 0;
}

/*
RD3 -> LED_RED_E 	-> 	EXT_RED
RD2 -> LED_GREEN_E -> 	EXT_GREEN
RD1 -> LED_BLUE_E 	-> 	EXT_BLUE

RD6 -> LED_GREEN
RD5 -> LED_RED
RD4 -> LED_BLUE
*/

void LEDs_AllON(void)
{
    // Output Value:0-OFF 1-ON
    SetLedsStatus(MAIN_LED_B, LED_ON);
    SetLedsStatus(MAIN_LED_R, LED_ON);
    SetLedsStatus(MAIN_LED_G, LED_ON);
	
#ifdef LED_DEBUG
    GPIOx_Output(BANKD, MAIN_LED_B, 1);
    GPIOx_Output(BANKD, MAIN_LED_R, 1);
    GPIOx_Output(BANKD, MAIN_LED_G, 1);
#endif
}

void LEDs_AllOff(void)
{
    // Output Value:0-OFF 1-ON
    SetLedsStatus(MAIN_LED_B, LED_OFF);
    SetLedsStatus(MAIN_LED_R, LED_OFF);
    SetLedsStatus(MAIN_LED_G, LED_OFF);
	
#ifdef LED_DEBUG
    GPIOx_Output(BANKD, MAIN_LED_B, 0);
    GPIOx_Output(BANKD, MAIN_LED_R, 0);
    GPIOx_Output(BANKD, MAIN_LED_G, 0);
#endif
}

void LEDs_Init(void)
{
    GPIOx_Config(BANKD, MAIN_LED_B, OUTPUT_DIR);
    GPIOx_Config(BANKD, MAIN_LED_R, OUTPUT_DIR);
    GPIOx_Config(BANKD, MAIN_LED_G, OUTPUT_DIR);

#ifdef LED_DEBUG
    // Green LED ON to indicate powerup
    GPIOx_Output(BANKD, MAIN_LED_G, 1);
    delay_ms_nop(10);

    // Disable all LEDs
    GPIOx_Output(BANKD, MAIN_LED_B, 0);
    GPIOx_Output(BANKD, MAIN_LED_R, 0);
    GPIOx_Output(BANKD, MAIN_LED_G, 0);
#endif
}

// PORTx: just for read
// LATx: read -> modify -> write
void LEDs_Ctrl(LED_INDEX led_id,LED_STA led_sta)
{
    if(led_sta){
        LATD |= (1<<led_id);
    }else{
        LATD &= (~1<<led_id);
    }
}

void LockSwitch_Init(void)
{
    _ANSE4 = 0;
    // Default HW Pull Up
    GPIOx_Config(BANKE, 4, INPUT_DIR);

    // _ANSF5 = 0;
    // Default HW Pull Up ???
    GPIOx_Config(BANKF, 5, INPUT_DIR);
}

u8 IsLockSwitchOpen(void)
{
    if (!GPIOx_Input(BANKE, 4)) {
        return 1;// Open
    } else {
        return 0;// Close
    }
}

void Beep_Init(void)
{
    _ANSB13 = 0;

    GPIOx_Config(BANKB, 13, OUTPUT_DIR);
    GPIOx_Output(BANKB, 13, 0);// default low -> poweroff
}

void Beep_Low(void)
{
    GPIOx_Output(BANKB, 13, 0);
}

void Beep_High(void)
{
    GPIOx_Output(BANKB, 13, 1);
}

void InOutPurpose_Init(void)
{
    // IN:  RB0  for Voltage Mesurement(using ADC)
    // OUT: RB1  for CLRC663 IF3: output 1
    // OUT: RB2  for CLRC663 TX(config in later UARTx_Init)
    // IN:  RB3  for BG96: 1-power on finished
    // = 9
    // XX:  RB4  for CLRC663 IF1: unused
    // IN:  RB5  for CLRC663 RX(config in later UARTx_Init)
    // XX:  RB6  for ICSP use
    // XX:  RB7  for ICSP use
    // = F

    // OUT: RB8  for BG96: to detect if host is active or sleep
    // OUT: RB9  for BG96: to power up BG96
    // OUT: RB10 for BG96: 1-normal mode, 0-airplane mode
    // OUT: RB11 for BG96: reset
    // = 0

    // IN:  RB12 for BG96: 1-normal mode, 0-PSM mode
    // OUT: RB13 for BUZZER
    // OUT: RB14 for BG96 TX(config in later UARTx_Init)
    // IN:  RB15 for TouchPad IRQ
    // = 9

    TRISB &= 0x90F9;// Direction:0-OUT 1-IN

    // OUT: RB13 for BNO055: reset
    // OUT: RB14 for BNO055: 1-normal mode, 0-IAP
    // XX:  Other for unused
    TRISC &= 0xFF81;// Direction:0-OUT 1-IN

    // IN:  RD0 for BNO055 IRQ
    // OUT: RD1/2/3/4/5/6 for LEDs
    // XX:  RD7 for unused
    // OUT: RD8/9 for Motor
    // IN:  RD10 for BNO055 RX(config in later UARTx_Init)
    // OUT: RD11 for BNO055 TX(config in later UARTx_Init)
    // XX:  RD12~RD15 for unused
    TRISD &= 0xFF81;// Direction:0-OUT 1-IN

    // IN:  RE0 for BG96 UART DCD
    // OUT: RE1 for BG96 UART RTS
    // IN:  RE2 for BG96 URC IRQ
    // IN:  RE3 for CLRC663 IRQ
    // = D
    // IN:  RE4 for SW1 IRQ
    // OUT: RE5/6/7 for CLRC663
    // = 1
    // XX:  RE8~RE15 for unused
    // FF
    TRISE &= 0xFF81;// Direction:0-OUT 1-IN

    // IN:  RF0 for BG96 UART CTS
    // OUT: RF1 for BG96 UART DTR
    // IN:  RF2 for Charge Status(need internally pull-up): 0-during charging, 1-normal mode
    // XX:  RF3 for unused
    // OUT: RF4 for TouchPad: 0-low power mode, 1-normal mode
    // IN:  RF5 for SW2 IRQ
    // XX:  RF6~RD15 for unused
    TRISF &= 0xFF81;// Direction:0-OUT 1-IN

    // XX:  RG0~RG2 for unused
    // OUT: RG3 for Charge Enable: 1-disable charge, 0-allow charge
    // XX:  RG4~RG5 for unused
    // OUT: RG6 for BG96 RX(config in later UARTx_Init)
    // IN:  RG7 for BG96_GPS RX(config in later UARTx_Init)
    // OUT: RG8 for BG96_GPS TX(config in later UARTx_Init)
    // OUT: RG9 for TouchPad: sensitivity PWM
    // XX:  RG10~RG15 for unused
    TRISG &= 0xFF81;// Direction:0-OUT 1-IN
}

void ExtIntr2_Enable(void)
{
    GPIOx_Pull(BANKF, 2, PULL_UP);
    GPIOx_Config(BANKF, 2, INPUT_DIR);

    _INT2R = 30;// RF2->EXT_INT:INT2

    _INT2IP = IPL_MID;
    _INT2IF = 0;// Clear INT Flag
    _INT2EP = 1;// Fall Edge
    _INT2IE = 1;// Enable INT1 Interrupt
}

void ExtIntr2_Enable_RD1(void)
{
    GPIOx_Pull(BANKD, 1, PULL_UP);
    GPIOx_Config(BANKD, 1, INPUT_DIR);

    _INT2R = 24;// RD1->EXT_INT:INT2

    _INT2IP = IPL_MID;
    _INT2IF = 0;// Clear INT Flag
    _INT2EP = 0;// Rise Edge
    _INT2IE = 1;// Enable INT1 Interrupt
}

void ExtIntr2_Disable(void)
{
    _INT2IF = 0;// Clear INT Flag
    _INT2IE = 0;// Enable INT1 Interrupt
}

void Charge_Init(void)
{
    // Charge Status
    GPIOx_Pull(BANKF, 2, PULL_UP);
    GPIOx_Config(BANKF, 2, INPUT_DIR);

    // Charge Enable
    GPIOx_Config(BANKG, 3, OUTPUT_DIR);
    GPIOx_Output(BANKG, 3, 0);// allow charge
}

void Charge_Disable(void)
{
    GPIOx_Output(BANKG, 3, 1);// disable charge
}

u8 Charge_InsertDetect(void)
{
    if (!GPIOx_Input(BANKF, 2)) {
        DEBUG("Charge Status Level = 0 -> Charging Mode...\n");
        return 1;// charging mode
    } else {
        DEBUG("Charge Status Level = 1 -> Normal Mode...\n");
        return 0;// normal mode
    }
}

u8 GetLedsStatus(LED_INDEX led_id)
{
    return (gs_leds_sta&(1<<led_id)) ? 1 : 0;
}

void SetLedsStatus(LED_INDEX led_id, LED_STA led_sta)
{
    if (LED_ON == led_sta) {
        if (MAIN_LED_P == led_id) {
            gs_leds_sta |= (1<<MAIN_LED_R);
            gs_leds_sta |= (1<<MAIN_LED_B);
        } else if (MAIN_LED_O == led_id) {
            gs_leds_sta |= (1<<MAIN_LED_R);
            gs_leds_sta |= (1<<MAIN_LED_G);
        } else {
            gs_leds_sta |= (1<<led_id);
        }
    } else {
        if (MAIN_LED_P == led_id) {
            gs_leds_sta &= ~(1<<MAIN_LED_R);
            gs_leds_sta &= ~(1<<MAIN_LED_B);
        } else if (MAIN_LED_O == led_id) {
            gs_leds_sta &= ~(1<<MAIN_LED_R);
            gs_leds_sta &= ~(1<<MAIN_LED_G);
        } else {
            gs_leds_sta &= ~(1<<led_id);
        }
    }
}

void SetLedsMode(LED_INDEX led_id, LED_MOD led_mod)
{
    if (LED_BLINK == led_mod) {
        if (MAIN_LED_P == led_id) {
            gs_leds_mod |= (1<<MAIN_LED_R);
            gs_leds_mod |= (1<<MAIN_LED_B);
        } else if (MAIN_LED_O == led_id) {
            gs_leds_mod |= (1<<MAIN_LED_R);
            gs_leds_mod |= (1<<MAIN_LED_G);
        } else {
            gs_leds_mod |= (1<<led_id);
        }
    } else {
        if (MAIN_LED_P == led_id) {
            gs_leds_mod &= ~(1<<MAIN_LED_R);
            gs_leds_mod &= ~(1<<MAIN_LED_B);
        } else if (MAIN_LED_O == led_id) {
            gs_leds_mod &= ~(1<<MAIN_LED_R);
            gs_leds_mod &= ~(1<<MAIN_LED_G);
        } else {
            gs_leds_mod &= ~(1<<led_id);
        }
    }
}

u8 GetLedsMode(LED_INDEX led_id)
{
    return (gs_leds_mod&(1<<led_id)) ? 1 : 0;
}
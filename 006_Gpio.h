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

#ifndef GPIO_H
#define GPIO_H

#include "015_Common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EXTE_LED_B = 1,
    EXTE_LED_G,
    EXTE_LED_R,
    MAIN_LED_B,
    MAIN_LED_R,
    MAIN_LED_G,
} LED_INDEX;

typedef enum {
    LED_OFF = 0,
    LED_ON,
} LED_STA;

typedef enum {
    LED_NORMAL = 0,
    LED_BLINK,
} LED_MOD;

typedef enum {
    BANKB = 0,
    BANKC,
    BANKD,
    BANKE,
    BANKF,
    BANKG,
} GPIO_BANKx;

typedef enum {
    OUTPUT_DIR = 0,
    INPUT_DIR,
} GPIO_DIR;

typedef enum {
    PULL_DOWN = 0,
    PULL_UP,
} GPIO_PUL;

void GPIOx_Config(GPIO_BANKx port, u8 pin, GPIO_DIR dir);
void GPIOx_Output(GPIO_BANKx port, u8 pin, u8 value);
u8 GPIOx_Input(GPIO_BANKx port, u8 pin);
u8 GetLedsStatus(LED_INDEX led_id);
void SetLedsStatus(LED_INDEX led_id, LED_STA led_sta);

u8 GetLedsMode(LED_INDEX led_id);
void SetLedsMode(LED_INDEX led_id, LED_MOD led_sta);

void LEDs_Init(void);
void LEDs_Ctrl(LED_INDEX led_id,LED_STA led_sta);
void LEDs_AllON(void);
void LEDs_AllOff(void);

void Beep_Init(void);
void Beep_Low(void);
void Beep_High(void);

u8 IsLockSwitchOpen(void);
void LockSwitch_Init(void);

void Charge_Init(void);
void Charge_Disable(void);
u8 Charge_InsertDetect(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */

//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

//******************************************************************************
// File:   001_Tick_10ms.h
// Author: Hans Desmet
// Comments: Initial Real Time file
// Revision history: Version 01.00
// Date 26/12/2018
//******************************************************************************

#ifndef __Tick_10ms_H
#define __Tick_10ms_H

#include <p24fxxxx.h>

#include "015_Common.h"

void Configure_Tick1(void);
void Configure_Tick2(void);
void Configure_Tick3(void);

void Enable_Tick1(void);
void Disable_Tick1(void);
void Enable_Tick2(void);
void Disable_Tick2(void);
void Enable_Tick3(void);
void Disable_Tick3(void);

void delay_ms_nop(u32 cnt);
void delay_us_nop(u32 cnt);
void delay_ms(unsigned long val);
unsigned long GetTimeStamp();
bool isDelayTimeout(unsigned long start_time,unsigned long delayms);

#endif //__Tick_10ms_H

//******************************************************************************
//* END OF FILE
//******************************************************************************

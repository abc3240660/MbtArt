//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

//******************************************************************************
// File:   003_BG96.h
// Author: Hans Desmet
// Comments: BG96 header file
// Revision history: Version 01.00
// Date 26/12/2018
//******************************************************************************

#ifndef __LB1938_H
#define __LB1938_H

#include <p24fxxxx.h>

#include "015_Common.h"

#define MOTOR_HOLD_TIME 30

typedef enum {
    MOTOR_LEFT = 0,
    MOTOR_RIGHT,
} MOTOR_FWD;

void LB1938_Init(void);
void LB1938_OpenLock(void);
void LB1938_MotorCtrl(MOTOR_FWD motor_fwd, u16 hold_ms);

#endif //__LB1938_H

//******************************************************************************
//* END OF FILE
//******************************************************************************

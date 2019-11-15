/* 
 * File:   015_ADC0.h
 * Author: Administrator
 *
 * Created on August 11, 2019, 10:05 PM
 */

#ifndef __ADC0_H
#define __ADC0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdbool.h>

void ADC0_Init(void);
void ADC0_Enable(void);
void ADC0_Disable(void);
bool ADC0_GetValue(u32 *value);

#ifdef __cplusplus
}
#endif

#endif /* __ADC0_H */


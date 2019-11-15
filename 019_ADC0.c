#include <stdio.h>
#include "xc.h"
#include "001_Tick_10ms.h"
#include "019_ADC0.h"

void ADC0_Init(void)
{
#if 0
    _TRISB9 = 0 ;
    _PCFG0 = 0 ;
    ANSELBbits.ANSB0 = 1;

    AD1CON1 = 0x00E4 ;
    AD1CON2 = 0x0000 ;
    AD1CON3 = 0x0601 ;
    AD1CHS = 0x0000 ;
    AD1CSSL = 0 ;

    _AD1IF = 0 ;
    _AD1IE = 0 ;
    _AD1IP = 0 ;

    _ADON = 1 ;
#endif

    // ASAM disabled; DMABM disabled; ADSIDL disabled; DONE disabled; DMAEN disabled; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC Clearing sample bit ends sampling and starts conversion; MODE12 10-bit; ADON enabled;
    AD1CON1 = 0x8000;

    // CSCNA enabled; NVCFG0 AVSS; PVCFG AVDD; ALTS disabled; BUFM disabled; SMPI Generates interrupt after completion of every sample/conversion operation; BUFREGEN disabled;
    AD1CON2 = 0x400;

    // SAMC 10; EXTSAM disabled; PUMPEN disabled; ADRC FOSC/2; ADCS 10;
    AD1CON3 = 0xA0A;

    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS;
    AD1CHS = 0x00;

    // CSS25 disabled; CSS24 disabled; CSS23 disabled; CSS22 disabled; CSS21 disabled; CSS20 disabled; CSS30 disabled; CSS19 disabled; CSS18 disabled; CSS29 disabled; CSS17 disabled; CSS28 disabled; CSS16 disabled;
    AD1CSSH = 0x00;

    // CSS9 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 disabled; CSS4 disabled; CSS3 disabled; CSS2 disabled; CSS15 disabled; CSS1 disabled; CSS14 disabled; CSS0 enabled; CSS13 disabled; CSS12 disabled; CSS11 disabled; CSS10 disabled;
    AD1CSSL = 0x01;

    // DMABL Allocates 1 word of buffer to each analog input;
    AD1CON4 = 0x00;

    // ASEN disabled; WM Legacy operation; ASINT No interrupt; CM Less Than mode; BGREQ disabled; CTMREQ disabled; LPEN disabled;
    AD1CON5 = 0x00;

    // CHH20 disabled; CHH22 disabled; CHH21 disabled; CHH24 disabled; CHH23 disabled; CHH25 disabled; CHH17 disabled; CHH16 disabled; CHH19 disabled; CHH18 disabled;
    AD1CHITH = 0x00;

    // CHH9 disabled; CHH8 disabled; CHH7 disabled; CHH6 disabled; CHH5 disabled; CHH4 disabled; CHH3 disabled; CHH2 disabled; CHH1 disabled; CHH0 disabled; CHH11 disabled; CHH10 disabled; CHH13 disabled; CHH12 disabled; CHH15 disabled; CHH14 disabled;
    AD1CHITL = 0x00;

    // CTMEN23 disabled; CTMEN24 disabled; CTMEN21 disabled; CTMEN22 disabled; CTMEN20 disabled; CTMEN18 disabled; CTMEN19 disabled; CTMEN16 disabled; CTMEN17 disabled; CTMEN25 disabled;
    AD1CTMENH = 0x00;

    // CTMEN5 disabled; CTMEN6 disabled; CTMEN7 disabled; CTMEN8 disabled; CTMEN9 disabled; CTMEN12 disabled; CTMEN13 disabled; CTMEN10 disabled; CTMEN0 disabled; CTMEN11 disabled; CTMEN1 disabled; CTMEN2 disabled; CTMEN3 disabled; CTMEN4 disabled; CTMEN14 disabled; CTMEN15 disabled;
    AD1CTMENL = 0x00;

    // AD1RESDMA 0;
    AD1RESDMA = 0x00;

    // VBGADC disabled; VBGUSB disabled; VBGEN disabled; VBGCMP disabled;
    ANCFG = 0x00;

    // adc1_obj.intSample = AD1CON2bits.SMPI;
}

bool ADC0_GetValue(u32 *value)
{
    u8 trycnt = 30;
    u16 reg_val = 0;

    while (trycnt--) {
        AD1CON1bits.SAMP = 1;

        if (_AD1IF) {
            _AD1IF = 0 ;
            reg_val = ADC1BUF0 ;

            DEBUG("ADC Value = %d\n", reg_val);

            *value = reg_val;
            AD1CON1bits.SAMP = 0;
            return true;
        }

        AD1CON1bits.SAMP = 0;

        DEBUG("try again...\n");
        delay_ms(100);
    }

    return false;
}

void ADC0_Enable(void)
{
}

void ADC0_Disable(void)
{
}

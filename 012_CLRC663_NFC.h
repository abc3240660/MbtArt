//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

/******************************************************************************
 * A library for CLRC663 Module
 * This file is about the CLRC663 API
 *
 * Copyright (c) 2019 Mobit technology inc.
 * @Author       : Damon
 * @Create time  : 04/14/2019
******************************************************************************/

#ifndef CLRC663_NFC_H
#define CLRC663_NFC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "002_CLRC663.h"
#include "015_Common.h"

#define bitSet(x,n) (x|=0x01<<n)

//#define DEBUG_ISO14443B
//#define PRINT_IDENTIFICATION_NUMBERS

u8 read_iso14443B_nfc_card(u8* card_id, u8* serial_nr);
void read_iso14443A_nfc_card();

u8 QueryMobibCard(u8* card_dats);
u8 AddNewMobibCard(u8* card_id, u8* serial_nr);
void DeleteMobibCard(u8* card_id, u8* serial_nr);
void DeleteAllMobibCard(void);
u8 ReadMobibNFCCard(void);
void CardIDFlashBankInit(void);
void CLRC663_PowerUp(void);

#ifdef __cplusplus
}
#endif

#endif /* CLRC663_NFC_H */


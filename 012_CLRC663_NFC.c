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

#include <xc.h>
#include <string.h>
#include "001_Tick_10ms.h"
#include "004_LB1938.h"
#include "006_Gpio.h"
#include "007_Uart.h"
#include "012_CLRC663_NFC.h"
#include "015_Common.h"

static u8 gs_tmp_card_id[LEN_BYTE_SZ32+1] = {0};
static u8 gs_tmp_serial_nr[LEN_BYTE_SZ32+1] = {0};

static u8 gs_bind_cards[CNTR_MAX_CARD][LEN_BYTE_SZ64] = {{"1234567890"},// Default Binded 3 CardIDs
                                                         {"2345678901"},
                                                         {"3456789012"}};

// Hex print for blocks without DEBUG.
void print_block(u8 * block, u8 length)
{
    int i = 0;

    for (i=0; i<length; i++) {
        if (block[i] < 16) {
            DEBUG("%02x ", block[i]);
        } else {
            DEBUG("%02x ", block[i]);
        }
    }

    DEBUG("\r\n");
}

void print_block_01(u8* block, u8 lower_lim, u8 upper_lim)
{
    int i = 0;

    for (i=lower_lim; i<upper_lim; i++) {
        if (block[i] < 16) {
            DEBUG("%02x ", block[i]);
        } else {
            DEBUG("%02x ", block[i]);
        }
    }

    DEBUG(" \r\n");
}

void print_card_ID_block(u8 * block, u8 length)
{
    int i = 0;

    DEBUG("CardId: ");

    for (i = 3; i < 12; i++) {
        if (block[i] < 16) {
            DEBUG("%02x ", block[i]);
        } else {
            DEBUG("%02x ", block[i]);
        }
    }

    DEBUG("%02x ", block[12]>>4);
    DEBUG(" \r\n");
}

//  Card Blacklist and Card Invalidation
void check_valid_card(u8 * buf)
{
    if (buf[40] == 0x62 || buf [41] == 0x83)
        DEBUG(" ----INVALID CARD: card is locked----");
    else
        DEBUG("---- VALID CARD ----");
}

//  Print Pseudo-Unique PICC Identifier, Type B
void print_PUPI(u8* block)
{
    DEBUG("PUPI: ");
    print_block_01(block, 1, 5);
}

//  Print MOBIB Hardcoded Serial Number
void print_serial_nr(u8* block)
{
    DEBUG("Serial number: ");
    print_block_01(block, 23, 31);
}

u8 bitRead(u8 value,int pos)
{
    u8 temp = 0;

    temp = (value & (0x01 << pos)) >> pos;

    return temp;
}

//  Shift buffer with two bits (to determine MOBIB Original Card ID)
void shift(u8* buf, u8 buf_size)
{
    int i = 0;
    buf [3] = buf [3] << 2;
    for (i = 4; i < 13; i++) {
        u8 lastbit = bitRead(buf[i], 7);
        u8 secondlastbit = bitRead(buf[i], 6);
        if(lastbit == 1)
            bitSet(buf[i-1], 1);
        if(secondlastbit == 1)
            bitSet(buf[i-1], 0);
        buf[i] = buf[i] << 2;
    }
}

//  Print MOBIB Original Card ID
void print_card_ID(u8* buf, u8 bufsize, u8* card_id, u8* serial_nr)
{
    u8 i = 0;
    u8 k = 0;
    u8 tmp = 0;

    shift(buf, bufsize);

    for (i = 3; i < 13; i++){
        tmp = (buf[i]>>4)&0x0F;
        if ((tmp>=0x00) && (tmp<=0x09)) {
            card_id[k++] = tmp + '0';
        } else if ((tmp>=0x0A) && (tmp<=0x0F)) {
            card_id[k++] = tmp-0x0A + 'A';
        }
        if (i != 12) {
            tmp = buf[i]&0x0F;
            if ((tmp>=0x00) && (tmp<=0x09)) {
                card_id[k++] = tmp + '0';
            } else if ((tmp>=0x0A) && (tmp<=0x0F)) {
                card_id[k++] = tmp-0x0A + 'A';
            }
        }
        // DEBUG("%02X\n", buf[i]);
    }

    DEBUG("CardId = %s\n", card_id);
    // print_card_ID_block(buf, bufsize);
}

void read_iso14443A_nfc_card()
{
    CLRC663_configure_communication_protocol(CLRC630_PROTO_ISO14443A_106_MILLER_MANCHESTER); // Configure CLRC663

    //  WUPA
    u8 wupa_buffer [] = {CLRC630_ISO14443_CMD_WUPA};
    u8 atqa_buffer [2] = {0};
    u8 atqa_length;
    atqa_length = clrc663_communicate(wupa_buffer, sizeof(wupa_buffer), atqa_buffer);

    if (atqa_length != 0) { 
        DEBUG("\n  *** TYPE A TAG detected ***  \n"); 
    }

    u8 sak = 0;
    u8 uid[10] = {0};  // uids are maximum of 10 bytes long.

    // Select the card and discover its uid.
    u8 uid_len = clrc663_iso14443a_select(uid, &sak);

    if (uid_len != 0) {  // did we get an UID?
        DEBUG("UID of ");
        DEBUG("%d", uid_len);
        DEBUG(" bytes (SAK: ");
        DEBUG("%d", sak);
        DEBUG("): ");
        print_block(uid, uid_len);
        DEBUG("\n");
    }
}

u8 read_iso14443B_nfc_card(u8* card_id, u8* serial_nr)
{
    //  Configure CLRC663
    CLRC663_configure_communication_protocol(CLRC630_PROTO_ISO14443B_106_NRZ_BPSK);

    //  WUPB
    u8 wupb_buffer [] = {CLRC663_ISO14443B_CMD_APF, CLRC663_ISO14443B_CMD_AFI, CLRC663_ISO14443B_CMD_PARAM};
    u8 atqb_buffer [12] = {0};
    u8 atqb_length = clrc663_communicate(wupb_buffer, sizeof(wupb_buffer), atqb_buffer);

    if (atqb_length != 0) {
        //  ATTRIB
        u8 transmit_buffer [] = {0x1D, atqb_buffer[1], atqb_buffer[2], atqb_buffer[3], atqb_buffer[4], 0x00, 0x08, 0x01, 0x00};
        u8 receive_buffer [1] = {0};
        clrc663_communicate(transmit_buffer, sizeof(transmit_buffer), receive_buffer);

        //  CALYPSO --- Select Global Data Application 
        u8 apdu_transmit_buffer [] = APDU_SELECT_GLOBAL_DATA_APP_MOBIB_CARD;
        u8 apdu_receive_buffer [42] = {0};
        clrc663_communicate(apdu_transmit_buffer, sizeof(apdu_transmit_buffer), apdu_receive_buffer);

        //  CALYPSO --- Read record 1
        u8 apdu_transmit_buffer_1 [] = CARDISSUING_FILE_READ_RECORD_1;
        u8 apdu_receive_buffer_1 [32] = {0};
        clrc663_communicate(apdu_transmit_buffer_1, sizeof(apdu_transmit_buffer_1), apdu_receive_buffer_1);

#ifdef DEBUG_ISO14443B
        //  Print sended en received bytes
        DEBUG("  ---   WUPB   --- \n");
        DEBUG("Send: "); print_block(wupb_buffer, sizeof(wupb_buffer));
        DEBUG("Receive: "); print_block(atqb_buffer, sizeof(atqb_buffer));
        DEBUG("  ---   ATTRIB   --- \n");
        DEBUG("Send: "); print_block(transmit_buffer, sizeof(transmit_buffer));
        DEBUG("Receive: "); print_block(receive_buffer, sizeof(receive_buffer));
        DEBUG("  ---   CALYPSO --- APDU select global data application   --- \n");
        DEBUG("Send: "); print_block(apdu_transmit_buffer, sizeof(apdu_transmit_buffer));
        DEBUG("Receive: "); print_block(apdu_receive_buffer, sizeof(apdu_receive_buffer));
        DEBUG("  ---   CALYPSO --- APDU read record 1  --- \n");
        DEBUG("Send: "); print_block(apdu_transmit_buffer_1, sizeof(apdu_transmit_buffer_1));
        DEBUG("Receive: "); print_block(apdu_receive_buffer_1, sizeof(apdu_receive_buffer_1));
#endif

#ifdef PRINT_IDENTIFICATION_NUMBERS
        DEBUG(" \r\n");
        check_valid_card(apdu_receive_buffer);
        print_PUPI(atqb_buffer);
        print_serial_nr(apdu_receive_buffer);
        print_card_ID(apdu_receive_buffer_1, sizeof(apdu_receive_buffer_1));
#endif

        print_serial_nr(apdu_receive_buffer);
        print_card_ID(apdu_receive_buffer_1, sizeof(apdu_receive_buffer_1), card_id, serial_nr);

        if (('0'==card_id[0]) && ('0'==card_id[1]) && ('0'==card_id[2])) {
            return 0;
        }

        return 1;
    }

    return 0;
}

u8 ReadMobibNFCCard(void)
{
    u8 i = 0;
    u8 tmp_len = 0;

    memset(gs_tmp_card_id, 0, LEN_BYTE_SZ16);
    memset(gs_tmp_serial_nr, 0, LEN_BYTE_SZ16);

    Clrc663_Clear();

    if (read_iso14443B_nfc_card(gs_tmp_card_id, gs_tmp_serial_nr) > 0) {
        tmp_len = strlen((const char*)gs_tmp_card_id);
        tmp_len = strlen((const char*)gs_tmp_serial_nr);

        for (i=0; i<CNTR_MAX_CARD; i++) {
            if (0 == strncmp((const char*)gs_bind_cards[i], (const char*)gs_tmp_card_id, LEN_CARD_ID)) {
                LB1938_MotorCtrl(MOTOR_LEFT, MOTOR_HOLD_TIME);
            }
        }

        return 0;
    }

    return 1;
}

u8 AddNewMobibCard(u8* card_id, u8* serial_nr)
{
    u8 i = 0;
    u8 index = 88;

    if (NULL == card_id) {
        return 0;
    }

    for (i=0; i<CNTR_MAX_CARD; i++) {
        if (0 == strncmp((const char*)gs_bind_cards[i], (const char*)card_id, strlen((const char*)card_id))) {
            return 0;
        }
    }

    for (i=0; i<CNTR_MAX_CARD; i++) {
        if (0 == strlen((const char*)gs_bind_cards[i])) {
            memcpy(gs_bind_cards[i], card_id, LEN_BYTE_SZ32);
            memcpy(gs_bind_cards[i]+32, serial_nr, LEN_BYTE_SZ32);
            index = i;
            break;
        }
    }

    for (i=0; i<CNTR_MAX_CARD; i++) {
        if (strlen((const char*)gs_bind_cards[i]) != 0) {
            DEBUG("Binded CardId[%d]: %s\n", i, gs_bind_cards[i]);
        }
    }

    return index;
}

void DeleteMobibCard(u8* card_id, u8* serial_nr)
{
    u8 i = 0;

    if (NULL == card_id) {
        return;
    }

    for (i=0; i<CNTR_MAX_CARD; i++) {
        if (0 == strncmp((const char*)gs_bind_cards[i], (const char*)card_id, LEN_CARD_ID)) {
            memset(gs_bind_cards[i], 0, LEN_BYTE_SZ64);
        }
    }
}

void DeleteAllMobibCard(void)
{
    u8 i = 0;

    for (i=0; i<CNTR_MAX_CARD; i++) {
        memset(gs_bind_cards[i], 0, LEN_BYTE_SZ64);
    }
}

u8 QueryMobibCard(u8* card_dats)
{
    u8 i = 0;
    u8 count = 0;
    u8 offset = 0;

    if (NULL == card_dats) {
        return 0;
    }

    for (i=0; i<CNTR_MAX_CARD; i++) {
        if (strlen((const char*)gs_bind_cards[i]) > 0) {
            if (count != 0) {
                card_dats[offset++] = '|';
            }

            memcpy(card_dats+offset, gs_bind_cards[i], LEN_CARD_ID);

            count++;
        }
    }

    return count;
}

void CardIDFlashBankInit(void)
{
}

void CLRC663_PowerUp(void)
{
    GPIOx_Config(BANKB, 1, OUTPUT_DIR);
    GPIOx_Config(BANKE, 5, OUTPUT_DIR);
    GPIOx_Config(BANKE, 6, OUTPUT_DIR);
    GPIOx_Config(BANKE, 7, OUTPUT_DIR);

    GPIOx_Output(BANKE, 5, 0);// PDOWN Low -> Power UP Mode
    GPIOx_Output(BANKE, 6, 0);// IFSEL0 Low
    GPIOx_Output(BANKE, 7, 0);// IFSEL1 Low
    GPIOx_Output(BANKB, 1, 1);// IF3 HIGH
}

void CLRC663_PowerOff(void)
{
    GPIOx_Config(BANKB, 1, OUTPUT_DIR);
    GPIOx_Config(BANKE, 5, OUTPUT_DIR);
    GPIOx_Config(BANKE, 6, OUTPUT_DIR);
    GPIOx_Config(BANKE, 7, OUTPUT_DIR);

    GPIOx_Output(BANKE, 5, 1);// PDOWN Low -> Power UP Mode
    GPIOx_Output(BANKE, 6, 0);// IFSEL0 Low
    GPIOx_Output(BANKE, 7, 0);// IFSEL1 Low
    GPIOx_Output(BANKB, 1, 1);// IF3 HIGH
}
//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

//******************************************************************************
// File:   ---_Defines.h
// Author: Hans Desmet
// Comments: Initial Define file
// Revision history: Version 01.00
// Date 26/12/2018
//******************************************************************************

#ifndef __DEFINE_H
#define __DEFINE_H

#include <p24fxxxx.h>                                                                  // include processor files - each processor file is guarded.  

#define         SW_Major_Version        1
#define         SW_Minor_Version        1
#define         SW_Release              1

#define         HW_VER                  "201910041950"
#define         SW_VER                  "201910041950"

#define         IPL_HIGH                7
#define         IPL_MID                 5
#define         IPL_LOW                 3
#define         IPL_DIS                 0

#define         T1IPL                   5                                       // define Timer 1 IRQ priority levell for real tome Tick
#define         T2IPL                   1                                       // define Timer 2 IRQ priority levell for real tome Tick

#define         _XTAL_FREQ              12000000

#define false 0
#define true 1

#define LEN_BYTE_SZ8     8
#define LEN_BYTE_SZ16    16
#define LEN_BYTE_SZ32    32
#define LEN_BYTE_SZ64    64
#define LEN_BYTE_SZ128   128
#define LEN_BYTE_SZ256   256
#define LEN_BYTE_SZ512   512
#define LEN_BYTE_SZ1024  1024

#define LEN_CARD_ID      19
#define LEN_SERIAL_NR    16

#ifndef LEN_NET_TCP
#define LEN_NET_TCP      32
#endif

#define LEN_COMMON_USE  LEN_BYTE_SZ32
#define LEN_MD5_HEXSTR  LEN_BYTE_SZ32
#define LEN_MD5_HEX     LEN_BYTE_SZ16

// if store too more, TCP send buffer will be
// not enough, currently LEN_MAX_SEND = 512
// 20*19 = 380, will be suitable
#define CNTR_MAX_CARD           20
#define CNTR_INWORD_PER_CARD    8

// for common use
#define CNTR_INWORD_PER_COMM    8
#define CNTR_INWORD_PER_SMALL   2
#define CNTR_INWORD_PER_BIG     16

#define DEFAULT_HBEAT_GAP 120

// 0x0800 ~ 0x0FFF(2K): Params
// 0x1000 ~ 0x1FFF(4K): CardIDs

#define FLASH_PAGE_CARD_ID  0x0000
#define FLASH_BASE_CARD_ID  0x1000

#define FLASH_PAGE_PARAMS   0x0000
#define FLASH_BASE_PARAMS   0x0800

#define FLASH_PAGE_BOT  0x0005
#define FLASH_BASE_BOT  0x0000

#define FLASH_PAGE_APP  0x0000
#define FLASH_BASE_APP  0x2000

#define FLASH_PAGE_BAK   0x0002
#define FLASH_BASE_BAK   0x2000

#define FLASH_BASE_IP       FLASH_BASE_PARAMS
#define FLASH_SIZE_IP       CNTR_INWORD_PER_COMM

#define FLASH_BASE_PORT     (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*1)*2)
#define FLASH_SIZE_PORT     CNTR_INWORD_PER_COMM

#define FLASH_BASE_APN      (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2)*2)
#define FLASH_SIZE_APN      CNTR_INWORD_PER_BIG

#define FLASH_BASE_IAP_MD5  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG)*2)
#define FLASH_SIZE_IAP_MD5  CNTR_INWORD_PER_BIG

#define FLASH_BASE_1ST      (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2)*2)
#define FLASH_SIZE_1ST      CNTR_INWORD_PER_SMALL

#define FLASH_BASE_ALM_ON   (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*1)*2)
#define FLASH_SIZE_ALM_ON   CNTR_INWORD_PER_SMALL

#define FLASH_BASE_BEP_ON   (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*2)*2)
#define FLASH_SIZE_BEP_ON   CNTR_INWORD_PER_SMALL

#define FLASH_BASE_BEP_LV   (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*3)*2)
#define FLASH_SIZE_BEP_LV   CNTR_INWORD_PER_SMALL

#define FLASH_BASE_BOT_TM   (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*4)*2)
#define FLASH_SIZE_BOT_TM   CNTR_INWORD_PER_SMALL

#define FLASH_BASE_IAP_FG   (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*5)*2)
#define FLASH_SIZE_IAP_FG   CNTR_INWORD_PER_SMALL

#define FLASH_BASE_IAP_STA  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*6)*2)
#define FLASH_SIZE_IAP_STA  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_IAP_CNT  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*7)*2)
#define FLASH_SIZE_IAP_CNT  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U1  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*8)*2)
#define FLASH_SIZE_RSVD_U1  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U2  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*9)*2)
#define FLASH_SIZE_RSVD_U2  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U3  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*10)*2)
#define FLASH_SIZE_RSVD_U3  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U4  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*11)*2)
#define FLASH_SIZE_RSVD_U4  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U5  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*12)*2)
#define FLASH_SIZE_RSVD_U5  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U6  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*13)*2)
#define FLASH_SIZE_RSVD_U6  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U7  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*14)*2)
#define FLASH_SIZE_RSVD_U7  CNTR_INWORD_PER_SMALL

#define FLASH_BASE_RSVD_U8  (FLASH_BASE_PARAMS + (CNTR_INWORD_PER_COMM*2 + CNTR_INWORD_PER_BIG*2 + CNTR_INWORD_PER_SMALL*15)*2)
#define FLASH_SIZE_RSVD_U8  CNTR_INWORD_PER_SMALL

#define CNTR_INWD_PER_PAGE  1024
#define BYTE_ADDR_PER_INWD  2
#define BYTE_DATA_PER_INWD  3
#define CNTR_INWD_PER_BLK   128

#define BYTE_ADDR_PER_PAGE_SML  0x800   // One Page
#define BYTE_ADDR_PER_PAGE_LRG  0x10000 // One Large Page

typedef unsigned char bool;

// For Common
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef enum {
    PARAM_ID_SVR_IP = 0,
    PARAM_ID_SVR_PORT,
    PARAM_ID_SVR_APN,
    PARAM_ID_IAP_MD5,
    PARAM_ID_1ST_BOOT,
    PARAM_ID_ALM_ON,
    PARAM_ID_BEEP_ON,
    PARAM_ID_BEEP_LEVEL,
    PARAM_ID_BOOT_TM,
    PARAM_ID_IAP_FLAG,
    PARAM_ID_IAP_STA,
    PARAM_ID_IAP_CNT,
    PARAM_ID_RSVD_U1,// IAP BIN SIZE
    PARAM_ID_RSVD_U2,// FactoryRstReq
    PARAM_ID_RSVD_U3,
    PARAM_ID_RSVD_U4,
    PARAM_ID_RSVD_U5,
    PARAM_ID_RSVD_U6,
    PARAM_ID_RSVD_U7,
    PARAM_ID_RSVD_U8
} PARAM_ID;

typedef struct {
    u8 svr_ip[LEN_BYTE_SZ32];
    u8 svr_port[LEN_BYTE_SZ32];
    u8 svr_apn[LEN_BYTE_SZ64];
    u8 iap_md5[LEN_BYTE_SZ32];
    u32 is_first_boot;
    u32 is_alarm_on;
    u32 is_beep_on;
    u32 beep_level;
    u32 boot_time;

    // (APP SET)"1A2B" - need do update
    // (IAP SET)"3C4D" - update finished
    // (APP CLR)"8888" - idle(after app detect 3C4D)
    u32 need_iap_flag;

    // (IAP SET)"5281" - iap update NG
    // (APP CLR)"8888" - idle
    u32 iap_sta_flag;

    // try to jump into app
    // (IAP SET)0 -> 10 - if equal to 10, need do restore hex data from BAKOK sector into RUN sector
    // (APP CLR)0 - jump to app ok
    u32 iap_try_cnt;

    u32 reserved_use1;// IAP BIN SIZE in Bytes
    u32 reserved_use2;
    u32 reserved_use3;
    u32 reserved_use4;
    u32 reserved_use5;
    u32 reserved_use6;
    u32 reserved_use7;
    u32 reserved_use8;
} SYS_ENV;

#define IAP_REQ_ON  "1A2B"// APP Set: Apply Request
#define IAP_REQ_OK  "3C4D"// BOOT Set: IAP Success
#define IAP_REQ_NG  "5E6F"// BOOT Set: MD5 Check Error or other Errors
#define IAP_REQ_OFF "8888"// APP Set: IAP Finished or IDLE

#define FOR_DEBUG_USE 0

#define OSC_20M_USE 1
// #define DEMO_BOARD 1

#define BIN_SIZE_INT   132664
#define BIN_SIZE_STR  "132664"

#define OSC_32M_USE 1

// #define EXT_CRYSTAL 1

#define __DEBUG 1

#ifdef __DEBUG
#define DEBUG(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

#endif

//******************************************************************************
//* END OF FILE
//******************************************************************************

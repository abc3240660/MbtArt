//       10        20        30        40        50        60        70
//--*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*

//******************************************************************************
// File:   005_BNO055.h
// Author: Hans Desmet
// Comments: File handles BNO055 sensor
// Revision history: Version 01.00
// Date 26/12/2018
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <p24fxxxx.h>

#include "005_BNO055.h"
#include "001_Tick_10ms.h"
#include "007_Uart.h"
#include "006_Gpio.h"

static u8 cur_pos = 0;

static u8 bno055_int_flag = 0;
static u8 bno_send_buf[LEN_BYTE_SZ64+1] = {0};

u8 GetBNOIntrFlag(void)
{
    return bno055_int_flag;
}

void ClearBNOIntrFlag(void)
{
    bno055_int_flag = 0;
}

void __attribute__ ((weak)) EX_INT1_CallBack(void)
{
    // printf("!!!!!!!!!!!!!!!!!!!!!!!INT occur \n");
    bno055_int_flag = 1;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    EX_INT1_CallBack();

    IFS1bits.INT1IF = 0;// Clear INT Flag
}

void ExtIntr_Initialize(void)
{
    _TRISD0 = 1;
    RPINR0bits.INT1R = 0x00B;// RD0->EXT_INT:INT1

    IPC5bits.INT1IP = IPL_MID;
    IFS1bits.INT1IF = 0;// Clear INT Flag
    INTCON2bits.INT1EP = 0;// Rise Edge
    IEC1bits.INT1IE = 1;// Enable INT1 Interrupt
}

void bno_055_delay_ms(u32 ms)
{
    delay_ms((u16)(ms)*2);
}

void bno055_send_chars(u8 *data, int len)
{
    u16 i = 0;

    if (NULL == data) {
        return;
    }

    for (i=0; i<len; i++) {
        bno_055_delay_ms(20);
        Uart4_Putc(data[i]);
    }
}

void bno055_reve_buff_clear(void)
{
	cur_pos = 0;
	Uart4_Clear();
}

u8 bno055_getc_from_reve_buff(void)
{
    u8 try_cnt = 20;
    do {
		if (Uart4_GetSize() > cur_pos) {
            return Uart4_Read(cur_pos++);
        } else {
            bno_055_delay_ms(50);
        }
    } while (--try_cnt);

    return 0xFF;
}

static u16 bno055_write_byte(u8 reg_addr, u8 reg_data)
{

    u8 onebyte = 0;

	bno055_reve_buff_clear();
    memset(bno_send_buf, 0, LEN_BYTE_SZ64);

    bno_send_buf[0] = 0xAA;
    bno_send_buf[1] = 0x00;// WR
    bno_send_buf[2] = reg_addr;
    bno_send_buf[3] = 0x01;// LEN
    bno_send_buf[4] = reg_data;

    // send data
    bno055_send_chars(bno_send_buf, 5);

    // wait
    bno_055_delay_ms(20);
    onebyte = bno055_getc_from_reve_buff();

    if (0xEE == onebyte) {// NG ack response
        if (reg_addr != BNO055_SYS_TRIGGER) {
            onebyte = bno055_getc_from_reve_buff();
            if (onebyte != 0x01) {// 0x01 - WRITE_SUCCESS
                DEBUG("write failure code: 0x%X\r\n", onebyte);
                return 1;
            }
        }
    } else {// invalid ack
        DEBUG("receive invalid ack header: 0x%X\r\n", onebyte);
        return 1;
    }

    return 0;
}

static u16 bno055_write_bytes(u8 reg_addr, u8 *p_in, u8 len)
{
    u16 i = 0;
    u8 onebyte = 0;

	bno055_reve_buff_clear();
    if ((!p_in) || (len<=0)) {
        return 1;
    }

    memset(bno_send_buf, 0, LEN_BYTE_SZ64);

    bno_send_buf[0] = 0xAA;
    bno_send_buf[1] = 0x00;// WR
    bno_send_buf[2] = reg_addr;
    bno_send_buf[3] = len;// LEN

    for (i=0; i<len; i++) {
        bno_send_buf[4+i] = p_in[i];
    }

    // send data
    bno055_send_chars(bno_send_buf, len+4);

    // wait
    bno_055_delay_ms(100);
    onebyte = bno055_getc_from_reve_buff();

    if (0xEE == onebyte) {// NG ack response
        onebyte = bno055_getc_from_reve_buff();

        if (onebyte != 0x01) {// 0x01 - WRITE_SUCCESS
            DEBUG("write failure code: 0x%X\r\n", onebyte);

            return 1;
        }
    } else {// invalid ack
        DEBUG("receive invalid ack header: 0x%X\r\n", onebyte);

        return 1;
    }

    return 0;
}

static u8 bno055_read_byte(u8 reg_addr)
{
    u8 onebyte = 0;

    bno055_reve_buff_clear();
    memset(bno_send_buf, 0, LEN_BYTE_SZ64);

    bno_send_buf[0] = 0xAA;
    bno_send_buf[1] = 0x01;// RD
    bno_send_buf[2] = reg_addr;
    bno_send_buf[3] = 0x01;// LEN

    // send data
    bno055_send_chars(bno_send_buf, 4);

    // wait
    bno_055_delay_ms(50);

    // recv ack till empty or timeout
    onebyte = bno055_getc_from_reve_buff();

    if (0xEE == onebyte) {// NG ack response
        onebyte = bno055_getc_from_reve_buff();
        DEBUG("read failure code: 0x%X\r\n", onebyte);

        return 0;
    } else if (0xBB == onebyte) {// OK ack response
        onebyte = bno055_getc_from_reve_buff();// length

        if (onebyte != 0x01) {// length NG
            DEBUG("receive data lenght is ng\r\n");
            return 0;
        }

        return bno055_getc_from_reve_buff();// data
    } else {// invalid ack
        DEBUG("read invalid ack header: 0x%X\r\n", onebyte);

        return 0;
    }
}

static u16 bno055_read_bytes(u8 reg_addr, u8 len, u8 *p_out)
{
    u16 i = 0;
    u8 onebyte = 0;

    if ((!p_out) || (len<=0)) {
        DEBUG("[ERR] %s: Params Invalid\n", __func__);
        return 1;
    }

    bno_055_delay_ms(25);
    bno055_reve_buff_clear();
    memset(bno_send_buf, 0, LEN_BYTE_SZ64);

    bno_send_buf[0] = 0xAA;
    bno_send_buf[1] = 0x01;// RD
    bno_send_buf[2] = reg_addr;
    bno_send_buf[3] = len;// LEN

    // send data
    bno055_send_chars(bno_send_buf, 4);

    // wait
    bno_055_delay_ms(25);

    // recv ack till empty or timeout
    onebyte = bno055_getc_from_reve_buff();

    if (0xEE == onebyte) {// NG ack response
        onebyte = bno055_getc_from_reve_buff();
        DEBUG("read failure code: 0x%X\r\n", onebyte);

        return 1;
    } else if (0xBB == onebyte) {// OK ack response
        bno_055_delay_ms(25);
        onebyte = bno055_getc_from_reve_buff();// length

        if (onebyte != len) {// length NG
            DEBUG("receive data length is NG(%d-%d)\r\n", len, onebyte);
            return 1;
        }

        for (i=0; i<onebyte; i++) {
            bno_055_delay_ms(25);
            p_out[i] = bno055_getc_from_reve_buff();// length
        }
    } else {// invalid ack
        DEBUG("read invalid ack header: 0x%X\r\n", onebyte);

        return 1;
    }

    return 0;
}

static u16 bno055_loop_read_bytes(u8 reg_addr,u8 len,u8 *p_out)
{
    u16 i = 3;
    u16 result = 0;

    while (i--) {
        result = bno055_read_bytes(reg_addr,len,p_out);
        if(!result){
            return 0;
        }
    }

    return 1;
}

// to check chip id
u16 bno055_verify_chip(void)
{
    u8 onebyte = 0;

    onebyte = bno055_read_byte(BNO055_CHIP_ID);
    if (onebyte != 0xA0) {
        DEBUG("verify BNO055_CHIP_ID failed\r\n");
        return 1;
    }

    onebyte = bno055_read_byte(BNO055_ACC_ID);
    if (onebyte != 0xFB) {
        DEBUG("verify BNO055_ACC_ID failed\r\n");
        return 1;
    }

    onebyte = bno055_read_byte(BNO055_MAG_ID);
    if (onebyte != 0x32) {
        DEBUG("verify BNO055_MAG_ID failed\r\n");
        return 1;
    }

    onebyte = bno055_read_byte(BNO055_GYRO_ID);
    if (onebyte != 0x0F) {
        DEBUG("verify BNO055_GYRO_ID failed\r\n");
        return 1;
    }

    return 0;
}

// read the X/Y/Z axis of acceleration
u16 bno055_read_accel(u16 *p_out)
{
    u8 byteData[6] = {0};  // x/y/z accel register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_ACC_DATA_X_LSB, 6, &byteData[0]);  // Read the six raw data registers into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];      // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

// read the X/Y/Z axis of gyroscope
u16 bno055_read_gyro(u16 *p_out)
{
    u8 byteData[6] = {0};  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_GYR_DATA_X_LSB, 6, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

// read the X/Y/Z axis of magnetometer
u16 bno055_read_mag(u16 *p_out)
{
    u8 byteData[6] = {0};  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_MAG_DATA_X_LSB, 6, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

// read the W/X/Y/Z axis of quaternion
u16 bno055_read_quat(u16 *p_out)
{
    u8 byteData[8];  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_QUA_DATA_W_LSB, 8, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
        p_out[3] = ((u16)byteData[7] << 8) | byteData[6];
    }

    return result;
}

// read the heading/roll/pitch of euler
u16 bno055_read_eul(short *p_out)
{
    u8 byteData[6] = {0};  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_EUL_HEADING_LSB, 6, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

// read the X/Y/Z axis of linear acceleration
u16 bno055_read_lia(u16 *p_out)
{
    u8 byteData[6] = {0};  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_LIA_DATA_X_LSB, 6, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

// read the X/Y/Z axis of gravity vector
u16 bno055_read_grv(u16 *p_out)
{
    u8 byteData[6] = {0};  // x/y/z gyro register data stored here

    u16 result = bno055_loop_read_bytes(BNO055_GRV_DATA_X_LSB, 6, &byteData[0]);  // Read the six raw data registers sequentially into data array

    if (!result) {
        p_out[0] = ((u16)byteData[1] << 8) | byteData[0];       // Turn the MSB and LSB into a signed 16-bit value
        p_out[1] = ((u16)byteData[3] << 8) | byteData[2];
        p_out[2] = ((u16)byteData[5] << 8) | byteData[4];
    }

    return result;
}

u16 bno055_read_calibrate_sta(u16 *calib_sta)
{
    u8 temp = 0;

    u16 result = bno055_loop_read_bytes(BNO055_CALIB_STAT, 1, &temp);

    if (!result) {
        DEBUG("calibration sta = 0x%.2X\r\n", temp);
        DEBUG("system calibration %s\r\n", ((3 == (0xC0&temp)>>6))?"done":"not done");
        DEBUG("gyro calibration %s\r\n", ((3 == (0x30&temp)>>4))?"done":"not done");
        DEBUG("accel calibration %s\r\n", ((3 == (0x0C&temp)>>2))?"done":"not done");
        DEBUG("mag calibration %s\r\n", ((3 == (0x03&temp)>>0))?"done":"not done");

        *calib_sta = temp;
    }

    return result;
}

// 1 C = 1 LSB
// 2 F = 1 LSB
u8 bno055_read_temp(void)
{
    u8 temp = 0;

    bno055_loop_read_bytes(BNO055_TEMP, 1, &temp);

    return temp;
}

// read system status & self test result & system error
void bno055_read_status(u8 *sys_stat, u8 *st_ret, u8 * sys_err)
{
    u8 temp = 0;

    if (!sys_stat || !st_ret || !sys_err) {
        return;
    }

    // Select page 0 to read sensors
    // bno055_write_byte(BNO055_PAGE_ID, 0x00);

    /* system status
     0 = idel
     1 = system error
     2 = initializing peripherals
     3 = system initialization
     4 = executing self-test
     5 = sensor fusio algorithm running
     6 = system running without fusion algorithms
    */
    bno055_loop_read_bytes(BNO055_SYS_STATUS, 1, &temp);
    *sys_stat = temp;

    /* self test result
     0 = fail
     1 = pass
     BIT0: accelerometer self test
     BIT1: magnetometer self test
     BIT2: gyroscope self test
     BIT3: mcu self test
    */
    bno055_loop_read_bytes(BNO055_ST_RESULT, 1, &temp);
    *st_ret = temp;

    /*
     0 = no error
     1 = peripheral initialization error
     2 = system initialization error
     3 = self test result failed
     4 = register map value out of range
     5 = register map address out of range
     6 = register map write error
     7 = BNO low power mode not available for selected operation mode
     8 = accelerometer power mode not available
     9 = fusion algorithm configuration error
     A = sensor configuration error
    */
    bno055_loop_read_bytes(BNO055_SYS_ERR, 1, &temp);
    *sys_err = temp;
}

// change the chip's axis remap
u16 bno055_set_axis_remap(u8 mode)
{
    // Select BNO055 config mode
    u16 result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    result = bno055_write_byte(BNO055_AXIS_MAP_CONFIG, mode);
    if (result) {
        return 1;
    }

    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        return 1;
    }

    return 0;
}

// change the chip's axis sign
u16 bno055_set_axis_sign(u8 mode)
{
    u16 result = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    result = bno055_write_byte(BNO055_AXIS_MAP_SIGN, mode);
    if (result) {
        return 1;
    }

    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        return 1;
    }

    return 0;
}

u16 bno055_enter_suspend_mode(void)
{
    u16 result = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    result = bno055_write_byte(BNO055_PWR_MODE, 0x02);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    return 0;
}

u16 bno055_enter_normal_mode(void)
{
    u16 result = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_PAGE_ID, 0x1);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_INT_EN, 0x80);//ACC_AM ACC_NM
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_PAGE_ID, 0x0);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
	
	result = bno055_write_byte(BNO055_PWR_MODE, 0x00);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    return 0;
}

u16 bno055_enter_lower_mode(void)
{
    u16 result = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_PAGE_ID, 0x0);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_PWR_MODE, 0x01);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_PAGE_ID, 0x1);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_INT_EN, 0x40);//ACC_AM ACC_NM
    if (result) {
        return 1;
    }

    bno_055_delay_ms(50);
    result = bno055_write_byte(BNO055_PAGE_ID, 0x0);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    return 0;
}

u8 bno055_get_int_src(void)
{
    u8 onebyte = 0;

    // clear
    // Select BNO055 config mode
    // bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);

    bno_055_delay_ms(50);
    onebyte = bno055_read_byte(BNO055_INT_STATUS);
    bno_055_delay_ms(25);

    return onebyte;
}

u16 bno055_clear_int(void)
{
    // u8 onebyte = 0;
    // clear
    // bno_055_delay_ms(50);
    // onebyte = bno055_read_byte(BNO055_SYS_TRIGGER);

    bno_055_delay_ms(50);
    bno055_write_byte(BNO055_SYS_TRIGGER, 0x40);
    bno_055_delay_ms(50);

    return 0;
}

u16 bno055_initial(void)
{
    u16 result = 0;
    u8 onebyte = 0;
    u8 try_cnt = 5;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    //             bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0 
    // Access       w    w      w          w 
    // Reset        0    0      0          0 
    // Content  CLK_SEL RST_INT RST_SYS    Self_Test 
    // do reset
	result = bno055_write_byte(BNO055_SYS_TRIGGER, 0x20);
    if(result){
        return -1;
    }
	bno_055_delay_ms(1000);

    while (--try_cnt) {
        onebyte = bno055_read_byte(BNO055_CHIP_ID);

        if (0xA0 == onebyte) {
            break;
        }

        bno_055_delay_ms(25);
    }

    // timeout
    if (try_cnt <= 0) {
        return 1;
    }

    result = bno055_write_byte(BNO055_PWR_MODE, Normalpwr);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);

    return 0;
}

u16 bno055_int_config(void )
{
    u16 result = 0;
    u8 onebyte = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    //PAGE1 mode
    result = bno055_write_byte(BNO055_PAGE_ID, 0x1);
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    // AM NM interrupt enable
    result = bno055_write_byte(BNO055_INT_EN, 0xC0);//ACC_AM ACC_NM
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    // interrupt mask
    result = bno055_write_byte(BNO055_INT_MSK, 0xC0);//ACC_AM ACC_NM
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    // 0x14*7.81mg
    result = bno055_write_byte(BNO055_ACC_AM_THRES, 0x14);//1 LSB = 7.81 mg (4-g range)
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    // 0x0A*7.81mg
    result = bno055_write_byte(BNO055_ACC_NM_THRESH, 0x0A);//1 LSB = 7.81 mg (4-g range)
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    result = bno055_write_byte(BNO055_ACC_INT_SETTINGS, 0x1C);//AM/NM_Z_AXIS AM/NM_Y_AXIS AM/NM_X_AXIS 
    if (result) {
        return 1;
    }

    bno_055_delay_ms(25);
    // slo_no_mot_dur=16s
    result = bno055_write_byte(BNO055_ACC_NM_SET, (SLO_NO_MOT_DUR<<1) | SMNM);//SMNM=1
    if (result) {
        return 1;
    }

    // PAGE0
    result = bno055_write_byte(BNO055_PAGE_ID, 0x0);
    if (result) {
        return 1;
    }

    bno055_clear_int();

    onebyte = bno055_get_int_src();

    return 0;
}

u16 bno055_calibrate_demo(void)
{
    u16 result = 0;
    u16 calib_sta = 0;
    u8 byteData[22] = {0};

    // TODO: load offset configs from flash/eeprom into register
    // load_offset_from_flash(&byteData[0]);
    if (0) {
        // Select BNO055 config mode
        result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
        if (result) {
            return 1;
        }

        bno_055_delay_ms(25);
        result = bno055_write_bytes(BNO055_ACC_OFFSET_X_LSB, &byteData[0], 22);
        if (result) {
            return 1;
        }

        bno_055_delay_ms(25);
        // Select BNO055 NDOF mode
        result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
        if (result) {
            return 1;
        }

        bno_055_delay_ms(25);
        /* Crystal must be configured AFTER loading calibration data into BNO055. */
        result = bno055_set_ext_crystal(1);
        if (result) {
            return 1;
        }
    }

    // TODO: if no offset configs saved in flash/eeprom, to do the calibration
    while (1) {
        DEBUG("To Calibrate Accel/Gyro: Put device on a level surface and keep motionless! Wait......\r\n");
        DEBUG("To Calibrate Mag: Wave device in a figure eight until done!\r\n");

        bno055_read_calibrate_sta(&calib_sta);
        if (0xFF == calib_sta) {
            break;
        }

        bno_055_delay_ms(3000);
    }

    result = bno055_loop_read_bytes(BNO055_ACC_OFFSET_X_LSB, 22, &byteData[0]);
    if (result) {
        return 1;
    }

    // TODO: save the calibrated done offset configs into flash
    // save_offset_into_flash(&byteData[0]);

    return 0;
}

u16 bno055_set_ext_crystal(u8 usextal)
{
    u16 result = 0;

    // Select BNO055 config mode
    result = bno055_write_byte(BNO055_OPR_MODE, CONFIGMODE);
    if (result) {
        DEBUG("init failed 1\n");
        return 1;
    }

    bno_055_delay_ms(25);
    // Select page 0 to read sensors
    result = bno055_write_byte(BNO055_PAGE_ID, 0x00);
    if (result) {
        DEBUG("init failed 2\n");
        return 1;
    }

    bno_055_delay_ms(25);
    if (usextal) {
        result = bno055_write_byte(BNO055_SYS_TRIGGER, 0x80);
    } else {
        result = bno055_write_byte(BNO055_SYS_TRIGGER, 0x00);
    }

    if (result) {
        DEBUG("init failed 3\n");
        return 1;
    }

    bno_055_delay_ms(25);
    // Select BNO055 NDOF mode
    result = bno055_write_byte(BNO055_OPR_MODE, NDOF);
    if (result) {
        DEBUG("init failed 4\n");
        return 1;
    }

    bno_055_delay_ms(25);

    return 0;
}

u16 bno055_setup(void)
{
    u16 result = 0;
    // u8 sys_stat, st_ret, sys_err;

    // result = bno055_verify_chip();
    // if (result) {
    //     return 1;
    // }

    result = bno055_initial();
    if (result) {
        DEBUG("init failed \n");
        return 1;
    }

    result = bno055_int_config();
    if (result) {
        DEBUG("ini config failed \n");
        return 1;
    } else {
        DEBUG("bno055 ini config success \n");
    }

    // bno055_read_status(&sys_stat, &st_ret, &sys_err);
    // DEBUG("sys_stat0x%x st_ret0x%x sys_err=0x%x \n",sys_stat, st_ret, sys_err);

    result = bno055_set_ext_crystal(0);
    if (result) {
        DEBUG("set_ext_crystal failed \n");
        return 1;
    }

    return 0;
}

u16 bno055_get_euler(float *cur_pitch, float *cur_yaw, float *cur_roll)
{
    u16 result = 0;
    int delay_time = 100;
    short raw_data[4] = {0};

    float pitch, yaw, roll;// for euler

    bno_055_delay_ms(delay_time);

    // 1 degrees = 16 LSB
    // 1 radians = 900 LSB
    result = bno055_read_eul(raw_data);
    if (result) {
        return 1;
    }

    yaw = (float)raw_data[0] / 16;
    roll = (float)raw_data[1] / 16;
    pitch = (float)raw_data[2] / 16;

    *cur_pitch = pitch;
    *cur_roll  = roll;

    // DEBUG("euler data: yaw=%f, roll=%f, pitch=%f \n", (double)yaw, (double)roll, (double)pitch);
    // bno_055_delay_ms(delay_time);

    return 0;
}

// ret: 0-ok  1-NG  2-err
u16 bno055_euler_check(float init_pitch, float init_yaw, float init_roll)
{
    u16 ret = 0;
    float ch_z = 60;
    float ch_f = 60;

    float cur_pitch = 0;
    float cur_yaw = 0;
    float cur_roll = 0;

    float correct_pitch = 0;
    float correct_yaw = 0;
    float correct_roll = 0;

    ret = bno055_get_euler(&cur_pitch, &cur_yaw, &cur_roll);
    if (ret != 0)
        return ret;// 2 error

    correct_pitch = cur_pitch - init_pitch;
    correct_yaw   = cur_yaw - init_yaw;
    correct_roll  = cur_roll - init_roll;

    // DEBUG("%08f        %08f       %08f \n",cur_pitch, cur_yaw, cur_roll);
    // DEBUG("%08f        %08f       %08f \n",correct_pitch, correct_yaw, correct_roll);
    if ((correct_roll>ch_z) || (correct_roll<ch_f) ||
       (correct_pitch>ch_z) || (correct_pitch<ch_f))
        return 1;
    else
        return 0;
}


//******************************************************************************
// Configure BG96
//******************************************************************************
u16 Configure_BNO055(void)
{
    u16 result = 0;

    result = bno055_setup();
    if (result) {
        DEBUG("bno055 init failure\n");
    } else {
        DEBUG("bno055 init success\n");
    }

    return result;
}

u16 BNO055_init(void)
{
    Configure_BNO055();

    // bno055_clear_int();

    return 0;
}

void BNO055_PowerUp(void)
{    
    GPIOx_Config(BANKC, 13, OUTPUT_DIR);// BNO055 
    GPIOx_Config(BANKC, 14, OUTPUT_DIR);// BNO055
    GPIOx_Output(BANKC, 14, 1);// nTILT_BOOT_LOAD
   
    delay_ms(2000);
    GPIOx_Output(BANKC, 13, 0);// nTILT_RST
    delay_ms(2000);
    GPIOx_Output(BANKC, 13, 1);// nTILT_RST
}

//******************************************************************************
//* END OF FILE
//******************************************************************************

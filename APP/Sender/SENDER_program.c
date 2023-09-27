/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: SENDER driver program file    
*/
#include <stdbool.h>
#include <stdint.h>

#include <HEX1.h>
#include <HEX2.h>

#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "HAL/CANMANAGER/CANMANAGER_interface.h"
#include "HAL/LEDS/LEDS_interface.h"

#include "./APP/SENDER/SENDER_interface.h"
#include "./APP/SENDER/SENDER_private.h"
#include "./APP/SENDER/SENDER_config.h"

static void ResetCallBack(void)
{
    geCurruntState = SenderStateSendStart;
}

static void DataCallBack(void)
{
    gbDataFrameSent = true;
    gu32MsgCount++;
}

static void StartCallBack(void)
{
    geCurruntState = SenderStateSendData;
}

static void EndCallBack(void)
{
    geCurruntState = SenderStateDone;
}

static void CRCCallBack(void)

{
    geCurruntState = SenderStateSendCRC;
}

static uint32_t calculateCRC(uint32_t *data, uint32_t length) {
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i;
    int j ;
    for ( i= 0; i < length; i++) {
        crc ^= data[i];
        for (j= 0; j < 32; j++) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ 0xDEADBEAF;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}


void SENDER_Init(void)
{
    CANMANAGER_Init();
    LEDS_Init();
}
void SENDER_Start(void)
{
    LEDS_ON(RED_LED| BLUE_LED);

    uint32_t u32MsgData;

    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_RESET, SENDER_CAN_MSG_LENGTH_RESET, &u32MsgData, SENDER_CAN_CONTROLLER_ID_RESET, ResetCallBack);
    while(geCurruntState == SenderStateIdle);

    LEDS_OFF(RED_LED| BLUE_LED);
    LEDS_ON(GREEN_LED);
    SysCtlDelay(1000 * 16000 / 3);
    u32MsgData = SENDER_FLASH_TO_BANK_1;                                        /* change this */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_START, SENDER_CAN_MSG_LENGTH_START, &u32MsgData, SENDER_CAN_CONTROLLER_ID_START, StartCallBack);
    while(geCurruntState == SenderStateSendStart);

    LEDS_OFF(RED_LED| BLUE_LED| GREEN_LED);
    LEDS_ON(BLUE_LED);


    while(gu32MsgCount < size_app_1)                                            /* change this */
    {
        u32MsgData = CANProjectToFlash1_image_0[gu32MsgCount];                   /* change this */
        CANMANAGER_Transmit(SENDER_CAN_MSG_ID_DATA , SENDER_CAN_MSG_LENGTH_DATA , &u32MsgData, SENDER_CAN_CONTROLLER_ID_DATA , DataCallBack);
        gbDataFrameSent = false;
    }


    LEDS_OFF(BLUE_LED);
    LEDS_ON(RED_LED);
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_END  , SENDER_CAN_MSG_LENGTH_END  , &u32MsgData, SENDER_CAN_CONTROLLER_ID_END  , EndCallBack);
    while(geCurruntState == SenderStateSendData);

    LEDS_OFF(RED_LED);
    LEDS_ON(GREEN_LED);

    u32MsgData = calculateCRC(CANProjectToFlash1_image_0,size_app_1); /*change this when switching to another app*/
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_CRC  , SENDER_CAN_MSG_LENGTH_CRC  , &u32MsgData, SENDER_CAN_CONTROLLER_ID_CRC  , CRCCallBack);
    while(geCurruntState == SenderStateDone);
    LEDS_OFF(GREEN_LED);
    LEDS_ON(GREEN_LED|RED_LED|BLUE_LED);
    while(1);

}

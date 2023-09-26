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


void SENDER_Init(void)
{
    CANMANAGER_Init();
    LEDS_Init();
}
void SENDER_Start(void)
{
    uint32_t u32MsgData;

    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_RESET, SENDER_CAN_MSG_LENGTH_RESET, &u32MsgData, SENDER_CAN_CONTROLLER_ID_RESET, ResetCallBack);
    while(geCurruntState == SenderStateIdle);

    u32MsgData = SENDER_FLASH_TO_BANK_1;                                        /* change this */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_START, SENDER_CAN_MSG_LENGTH_START, &u32MsgData, SENDER_CAN_CONTROLLER_ID_START, StartCallBack);
    while(geCurruntState == SenderStateSendStart);

    while(gu32MsgCount < size_app_1)                                            /* change this */
    {
        u32MsgData = CANProjectToFlash1_image_0[gu32MsgCount];                   /* change this */
        CANMANAGER_Transmit(SENDER_CAN_MSG_ID_DATA , SENDER_CAN_MSG_LENGTH_DATA , &u32MsgData, SENDER_CAN_CONTROLLER_ID_DATA , DataCallBack);
//        while(!gbDataFrameSent);
        gbDataFrameSent = false;
    }

    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_END  , SENDER_CAN_MSG_LENGTH_END  , &u32MsgData, SENDER_CAN_CONTROLLER_ID_END  , EndCallBack);
    while(geCurruntState == SenderStateSendData);

    while(1);

}

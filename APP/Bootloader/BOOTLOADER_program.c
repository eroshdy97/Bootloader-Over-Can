/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: BOOTLOADER driver program file    
*/
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"

#include "HAL/CANMANAGER/CANMANAGER_interface.h"
#include "HAL/LEDS/LEDS_interface.h"

#include "./APP/BOOTLOADER/BOOTLOADER_interface.h"
#include "./APP/BOOTLOADER/BOOTLOADER_private.h"
#include "./APP/BOOTLOADER/BOOTLOADER_config.h"

static void WriteAppToFlash(uint32_t* pu32Src, uint32_t u32DstAddr, uint32_t u32Count)
{
    int i = 0;
    for (i = 0; i < u32Count; i+=1024)
    {
        FlashErase(u32DstAddr + i);
    }
    FlashProgram(pu32Src, u32DstAddr, u32Count);
}

static void ResetCallBack(void)
{
    SysCtlReset();
}

static void DataCallBack(void)
{
    gbDataFrameReceived = true;
    ++gu32DataReceivedLength;
}

static void StartCallBack(void)
{
    geCurruntState = BLStateReceivingData;
}

static void EndCallBack(void)
{
    geCurruntState = BLStateDone;
}

static void JumpToApp(uint32_t u32Address2jmp)
{
    switch (u32Address2jmp)
    {
        case BOOTLOADER_ADDRESS_BANK_1:
        {
            VTABLE_OFFSET_R |= u32Address2jmp;

            __asm(" mov r0, #0x00005000\n");
            __asm(" ldr r1, [r0, #4]");
            __asm(" bx r1\n");

            break;
        }
        case BOOTLOADER_ADDRESS_BANK_2:
        {
            VTABLE_OFFSET_R |= u32Address2jmp;

            __asm(" mov r0, #0x0001A800\n");
            __asm(" ldr r1, [r0, #4]");
            __asm(" bx r1\n");

            break;
        }
        default:
            break;
    }

}

static void SetAppFlagToRun(uint32_t u32Flag)
{
    FlashErase(BOOTLOADER_ADDRESS_FLAG);
    FlashProgram(&u32Flag, BOOTLOADER_ADDRESS_FLAG, sizeof(u32Flag));

}

void BOOTLOADER_Init(void)
{
    CANMANAGER_Init();
    LEDS_Init();
}

void BOOTLOADER_Start(void)
{

    uint32_t u32CanFrameData;
    uint32_t u32ProgramToRun = PROGRAM_TO_RUN_R;

    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_DATA , BOOTLOADER_CAN_MSG_LENGTH_DATA , &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_DATA , DataCallBack);
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_END  , BOOTLOADER_CAN_MSG_LENGTH_END  , &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_END  , EndCallBack);
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_START, BOOTLOADER_CAN_MSG_LENGTH_START, &gu32FlashToBank, BOOTLOADER_CAN_CONTROLLER_ID_START, StartCallBack);
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_RESET, BOOTLOADER_CAN_MSG_LENGTH_RESET, &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_RESET, ResetCallBack);

    uint32_t u32TimeOutCounter = BOOTLOADER_TIMEOUT_START_MS * 3180;
    while(geCurruntState == BLStateIdle && (u32TimeOutCounter > 0))
        u32TimeOutCounter--;

    if (u32TimeOutCounter == 0 && u32ProgramToRun == 1)
    {
        JumpToApp(BOOTLOADER_ADDRESS_BANK_1);
    }
    else if (u32TimeOutCounter == 0 && u32ProgramToRun == 2)
    {
        JumpToApp(BOOTLOADER_ADDRESS_BANK_2);
    }
    else
    {
        while(geCurruntState != BLStateReceivingData);
        CANMANAGER_ObjReceiveGet(&gu32FlashToBank, BOOTLOADER_CAN_CONTROLLER_ID_START);

        while(geCurruntState == BLStateReceivingData)
        {
            if(gbDataFrameReceived)
            {
                CANMANAGER_ObjReceiveGet(&u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_DATA);
                gu32DataReceived[gu32DataReceivedLength - 1] = u32CanFrameData;
                gbDataFrameReceived = false;
            }
        }
        switch (gu32FlashToBank)
        {
            case 1:
            {
                WriteAppToFlash(gu32DataReceived, BOOTLOADER_ADDRESS_BANK_1, gu32DataReceivedLength * 4);
                gu32DataReceivedLength = 0;

                SetAppFlagToRun(1);

                JumpToApp(BOOTLOADER_ADDRESS_BANK_1);
                break;
            }
            case 2:
            {
                WriteAppToFlash(gu32DataReceived, BOOTLOADER_ADDRESS_BANK_2, gu32DataReceivedLength * 4);
                gu32DataReceivedLength = 0;

                SetAppFlagToRun(2);

                JumpToApp(BOOTLOADER_ADDRESS_BANK_2);
                break;
            }
            default:
                break;
        }
    }
    while(1);
}
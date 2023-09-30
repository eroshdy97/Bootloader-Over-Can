/**
 * @file bootloader.c
 * @brief BOOTLOADER driver program file.
 *
 * This file contains the implementation of the BOOTLOADER driver module for a Tiva-C microcontroller.
 * It is responsible for receiving firmware updates over the CAN bus, validating the received data, and
 * programming it into flash memory.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
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

#include "./APP/BOOTLOADER/BOOTLOADER_config.h"
#include "./APP/BOOTLOADER/BOOTLOADER_private.h"
#include "./APP/BOOTLOADER/BOOTLOADER_interface.h"

/**
 * @brief Initialize the BOOTLOADER module.
 *
 * This function initializes the CAN Manager and LED modules used by the BOOTLOADER module.
 */
void BOOTLOADER_Init(void)
{
    CANMANAGER_Init();
    LEDS_Init();
}

/**
 * @brief Start the BOOTLOADER module.
 *
 * This function is the main entry point for the BOOTLOADER module. It sets up communication via the CAN bus,
 * receives firmware updates, validates them, and programs them into flash memory. After successful programming,
 * it sets a flag to indicate the application to run and performs a reset to start the new application.
 */
void BOOTLOADER_Start(void)
{
    /* Initialize LEDs to show bootloader operation */
    LEDS_ON(RED_LED | GREEN_LED);

    uint32_t u32CanFrameData;
    uint32_t u32ProgramToRun = PROGRAM_TO_RUN_R;

    /* Set up CAN message receive callbacks */
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_DATA , BOOTLOADER_CAN_MSG_LENGTH_DATA , &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_DATA , DataCallBack );
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_END  , BOOTLOADER_CAN_MSG_LENGTH_END  , &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_END  , EndCallBack  );
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_START, BOOTLOADER_CAN_MSG_LENGTH_START, &gu32FlashToBank, BOOTLOADER_CAN_CONTROLLER_ID_START, StartCallBack);
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_RESET, BOOTLOADER_CAN_MSG_LENGTH_RESET, &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_RESET, ResetCallBack);
    CANMANAGER_ObjReceiveSet(BOOTLOADER_CAN_MSG_ID_CRC  , BOOTLOADER_CAN_MSG_LENGTH_CRC  , &u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_CRC  , CRCCallBack  );

    /* Wait for a timeout to start receiving data */
    uint32_t u32TimeOutCounter = BOOTLOADER_TIMEOUT_START_MS * 3180;
    while (geCurruntState == BLStateIdle && (u32TimeOutCounter > 0))
        u32TimeOutCounter--;

    /* If the timeout occurred and a program is marked to run, jump to it */
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
        /* Wait for data reception to start */
        while (geCurruntState != BLStateReceivingData);
        LEDS_OFF(RED_LED | GREEN_LED);
        CANMANAGER_ObjReceiveGet(&gu32FlashToBank, BOOTLOADER_CAN_CONTROLLER_ID_START);
        LEDS_ON(BLUE_LED);

        /* Receive and process data frames */
        while (geCurruntState == BLStateReceivingData)
        {
            if (gbDataFrameReceived)
            {
                CANMANAGER_ObjReceiveGet(&u32CanFrameData, BOOTLOADER_CAN_CONTROLLER_ID_DATA);
                gu32DataReceived[gu32DataReceivedLength - 1] = u32CanFrameData;
                gbDataFrameReceived = false;
            }
        }
        LEDS_OFF(BLUE_LED);
        LEDS_ON(RED_LED);

        /* Wait for the data reception to complete */
        while (geCurruntState == BLStateDone);

        /* Calculate received CRC and compare it with the transmitted CRC */
        uint32_t u32receivedCRC;
        CANMANAGER_ObjReceiveGet(&u32receivedCRC, BOOTLOADER_CAN_CONTROLLER_ID_CRC);
        uint32_t u32calculatedCRC = calculateCRC(gu32DataReceived, gu32DataReceivedLength);
        if (u32calculatedCRC != u32receivedCRC)
        {
            /* CRC check failed, blink the red LED and reset */
            int i;
            for (i = 0; i < 5; ++i)
            {
                LEDS_Blink(RED_LED, 500);
            }
            SysCtlReset();
        }

        /* Write the received data to the appropriate flash bank and set the flag to run it */
        switch (gu32FlashToBank)
        {
            case 1:
            {
                /* Write the received application data to flash memory for Bank 1 */
                WriteAppToFlash(gu32DataReceived, BOOTLOADER_ADDRESS_BANK_1, gu32DataReceivedLength * 4);

                /* Reset the data received length counter */
                gu32DataReceivedLength = 0;

                /* Set the flag to indicate that Bank 1 should be run */
                SetAppFlagToRun(1);

                /* Turn off the red LED and turn on the green LED to indicate success */
                LEDS_OFF(RED_LED);
                LEDS_ON(GREEN_LED);

                /* Jump to the application code in Bank 1 */
                JumpToApp(BOOTLOADER_ADDRESS_BANK_1);
                break;
            }
            case 2:
            {
                /* Write the received application data to flash memory for Bank 2 */
                WriteAppToFlash(gu32DataReceived, BOOTLOADER_ADDRESS_BANK_2, gu32DataReceivedLength * 4);

                /* Reset the data received length counter */
                gu32DataReceivedLength = 0;

                /* Set the flag to indicate that Bank 2 should be run */
                SetAppFlagToRun(2);

                /* Turn off the red LED and turn on the green LED to indicate success */
                LEDS_OFF(RED_LED);
                LEDS_ON(GREEN_LED);

                /* Jump to the application code in Bank 2 */
                JumpToApp(BOOTLOADER_ADDRESS_BANK_2);
                break;
            }
            default:
                /* Invalid bank selection, do nothing */
                break;
        }

    }
    while (1);
}

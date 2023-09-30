/**
 * @file SENDER_program.c
 * @brief SENDER driver program file.
 *
 * This file contains the implementation of the SENDER driver module, which sends data over the CAN bus.
 *
 * @authors Ehab Roushdy
 * @version V01
 * @date 25/09/2023
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

/**
 * @brief Callback function for the reset command.
 *
 * This function is executed when a reset command is transmitted to the BOOTLOADER.
 */
static void ResetCallBack(void)
{
    geCurruntState = SenderStateSendStart;
}

/**
 * @brief Callback function for data frames.
 *
 * This function is executed when a data frame is successfully sent over the CAN bus.
 */
static void DataCallBack(void)
{
    gbDataFrameSent = true;
    gu32MsgCount++;
}

/**
 * @brief Callback function for the start command.
 *
 * This function is executed when a start command is transmitted to the BOOTLOADER.
 */
static void StartCallBack(void)
{
    geCurruntState = SenderStateSendData;
}

/**
 * @brief Callback function for the end command.
 *
 * This function is executed when an end command is transmitted to the BOOTLOADER.
 */
static void EndCallBack(void)
{
    geCurruntState = SenderStateDone;
}

/**
 * @brief Callback function for the CRC validation command.
 *
 * This function is executed when a CRC validation command is transmitted to the BOOTLOADER.
 */
static void CRCCallBack(void)
{
    geCurruntState = SenderStateSendCRC;
}

/**
 * @brief Calculate CRC for data.
 *
 * This function calculates the CRC for a block of data.
 *
 * @param[in] data Pointer to the data.
 * @param[in] length Length of the data in words (32-bit).
 * @return Calculated CRC value.
 */
static uint32_t calculateCRC(uint32_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i;
    int j;
    for (i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 32; j++)
        {
            if (crc & 0x80000000)
            {
                crc = (crc << 1) ^ 0xDEADBEAF;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

/**
 * @brief Initialize the SENDER module.
 *
 * This function initializes the CANMANAGER and LEDS modules.
 */
void SENDER_Init(void)
{
    CANMANAGER_Init();
    LEDS_Init();
}

/**
 * @brief Start the SENDER module.
 *
 * This function initiates the SENDER module's operation.
 */
void SENDER_Start(void)
{
    /* Turn on the RED and BLUE LEDs to indicate the start of the SENDER module. */
    LEDS_ON(RED_LED | BLUE_LED);

    uint32_t u32MsgData;

    /* Send the reset command to the BOOTLOADER to initiate the firmware update process. */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_RESET, SENDER_CAN_MSG_LENGTH_RESET, &u32MsgData, SENDER_CAN_CONTROLLER_ID_RESET, ResetCallBack);

    /* Wait until the BOOTLOADER acknowledges the reset command and changes the state. */
    while (geCurruntState == SenderStateIdle);

    /* Turn off the RED and BLUE LEDs and turn on the GREEN LED to indicate readiness. */
    LEDS_OFF(RED_LED | BLUE_LED);
    LEDS_ON(GREEN_LED);

    /* Delay to ensure a smooth transition before starting the firmware update. */
    SysCtlDelay(1000 * 16000 / 3);

    /* Set the application identifier (e.g., BANK_1) to specify which firmware to update. */
    u32MsgData = SENDER_FLASH_TO_BANK_1; /* Change this to select the appropriate application */

    /* Send the start command to the BOOTLOADER with the selected application identifier. */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_START, SENDER_CAN_MSG_LENGTH_START, &u32MsgData, SENDER_CAN_CONTROLLER_ID_START, StartCallBack);

    /* Wait until the BOOTLOADER acknowledges the start command and changes the state. */
    while (geCurruntState == SenderStateSendStart);

    /* Turn off all LEDs and turn on the BLUE LED to indicate data transmission. */
    LEDS_OFF(RED_LED | BLUE_LED | GREEN_LED);
    LEDS_ON(BLUE_LED);

    /* Transmit data frames until all data is sent. */
    while (gu32MsgCount < size_app_1) /* Change this to the appropriate data size */
    {
        /* Prepare data from the source (e.g., CANProjectToFlash1_image_0) for transmission. */
        u32MsgData = CANProjectToFlash1_image_0[gu32MsgCount]; /* Change this to read data from the appropriate source */

        /* Send a data frame over the CAN bus. */
        CANMANAGER_Transmit(SENDER_CAN_MSG_ID_DATA, SENDER_CAN_MSG_LENGTH_DATA, &u32MsgData, SENDER_CAN_CONTROLLER_ID_DATA, DataCallBack);

        /* Wait for acknowledgment that the data frame has been sent. */
        gbDataFrameSent = false;
    }

    /* Turn off the BLUE LED and turn on the RED LED to indicate the end of data transmission. */
    LEDS_OFF(BLUE_LED);
    LEDS_ON(RED_LED);

    /* Send the end command to the BOOTLOADER to signal the completion of data transmission. */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_END, SENDER_CAN_MSG_LENGTH_END, &u32MsgData, SENDER_CAN_CONTROLLER_ID_END, EndCallBack);

    /* Wait until the BOOTLOADER acknowledges the end command and changes the state. */
    while (geCurruntState == SenderStateSendData);

    /* Turn off the RED LED and turn on the GREEN LED to perform CRC calculation and validation. */
    LEDS_OFF(RED_LED);
    LEDS_ON(GREEN_LED);

    /* Calculate CRC for the transmitted data and send it over the CAN bus for validation. */
    u32MsgData = calculateCRC(CANProjectToFlash1_image_0, size_app_1); /* Change this when switching to another app */
    CANMANAGER_Transmit(SENDER_CAN_MSG_ID_CRC, SENDER_CAN_MSG_LENGTH_CRC, &u32MsgData, SENDER_CAN_CONTROLLER_ID_CRC, CRCCallBack);

    /* Wait for acknowledgment that the CRC calculation and validation have been sent. */
    while (geCurruntState == SenderStateDone);

    /* Turn off the GREEN LED and turn on all LEDs to indicate the end of the update process. */
    LEDS_OFF(GREEN_LED);
    LEDS_ON(GREEN_LED | RED_LED | BLUE_LED);

    while (1);
}

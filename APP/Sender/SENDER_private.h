/**
 * @file SENDER_private.c
 * @brief SENDER driver private file.
 *
 * This file contains private definitions and functions used internally by the SENDER driver module.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _SENDER_PRIVATE_H
#define _SENDER_PRIVATE_H

/**
 * @brief Constant to indicate flashing data to bank 1.
 *
 * This constant is used to indicate that the data should be flashed to bank 1 or 2.
 * It is used as a parameter when transmitting the start command over the CAN bus.
 */
#define SENDER_FLASH_TO_BANK_1                 1
#define SENDER_FLASH_TO_BANK_2                 2

/**
 * @brief Enum for SENDER states.
 *
 * This enumeration defines the possible states of the SENDER module.
 * - SenderStateIdle: The module is in an idle state.
 * - SenderStateSendStart: Sending the start command.
 * - SenderStateSendData: Sending data frames.
 * - SenderStateSendCRC: Sending the CRC command.
 * - SenderStateDone: The data transmission is complete.
 */
typedef enum
{
    SenderStateIdle,
    SenderStateSendStart,
    SenderStateSendData,
    SenderStateSendCRC,
    SenderStateDone,

} SenderStates;

/* Global variables */

/**
 * @brief Current state of the SENDER module.
 *
 * This variable holds the current state of the SENDER module.
 */
static SenderStates geCurruntState = SenderStateIdle;

/**
 * @brief Message count for data frames.
 *
 * This variable keeps track of the number of data frames sent.
 */
static uint32_t gu32MsgCount = 0;

/**
 * @brief Data frame sent flag.
 *
 * This flag is set when a data frame is successfully sent over the CAN bus.
 */
static bool gbDataFrameSent = false;

/* Function prototypes */

/**
 * @brief Calculate CRC for data.
 *
 * This function calculates the CRC for a block of data.
 *
 * @param[in] data Pointer to the data array.
 * @param[in] length Length of the data in words (32-bit).
 * @return Calculated CRC value.
 */
static uint32_t calculateCRC(const uint32_t *pu32data, uint32_t u32length);

/**
 * @brief Send application data to the BOOTLOADER for firmware update.
 *
 * This function sends application data to the BOOTLOADER for a firmware update over the CAN bus.
 * It follows a sequence of commands including reset, start, data transmission, end, and CRC calculation.
 *
 * @param[in] pu32HexArray Pointer to the source data array in hexadecimal format.
 * @param[in] u32AppSize Size of the application data in words (32-bit).
 * @param[in] u8bankToFlash Identifier for the target application bank (e.g., BANK_1 or BANK_2).
 */
static void SendApp(const uint32_t *pu32HexArray, uint32_t u32AppSize, uint8_t u8bankToFlash);

#endif

/**
 * @file bootloader_config.h
 * @brief BOOTLOADER driver configuration file.
 *
 * This file contains configuration settings for the BOOTLOADER driver module,
 * including CAN message IDs, message lengths, CAN controller IDs, memory addresses, and timeouts.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _BOOTLOADER_CONFIG_H
#define _BOOTLOADER_CONFIG_H

/* CAN message IDs */
#define BOOTLOADER_CAN_MSG_ID_RESET             0x00001000 /**< CAN message ID for the reset command. */
#define BOOTLOADER_CAN_MSG_ID_START             0x00001001 /**< CAN message ID for the start command. */
#define BOOTLOADER_CAN_MSG_ID_END               0x00001002 /**< CAN message ID for the end command. */
#define BOOTLOADER_CAN_MSG_ID_DATA              0x00001003 /**< CAN message ID for data frames. */
#define BOOTLOADER_CAN_MSG_ID_CRC               0x00001004 /**< CAN message ID for CRC validation. */

/* CAN message lengths */
#define BOOTLOADER_CAN_MSG_LENGTH_DATA          4 /**< Length of data frames in bytes. */
#define BOOTLOADER_CAN_MSG_LENGTH_END           1 /**< Length of end command messages in bytes. */
#define BOOTLOADER_CAN_MSG_LENGTH_START         4 /**< Length of start command messages in bytes. */
#define BOOTLOADER_CAN_MSG_LENGTH_RESET         1 /**< Length of reset command messages in bytes. */
#define BOOTLOADER_CAN_MSG_LENGTH_CRC           4 /**< Length of CRC command messages in bytes. */

/* CAN controller IDs */
#define BOOTLOADER_CAN_CONTROLLER_ID_RESET      1 /**< CAN controller ID for reset command. */
#define BOOTLOADER_CAN_CONTROLLER_ID_START      2 /**< CAN controller ID for start command. */
#define BOOTLOADER_CAN_CONTROLLER_ID_DATA       3 /**< CAN controller ID for data frames. */
#define BOOTLOADER_CAN_CONTROLLER_ID_END        4 /**< CAN controller ID for end command. */
#define BOOTLOADER_CAN_CONTROLLER_ID_CRC        5 /**< CAN controller ID for CRC validation. */

/* CAN addresses to flash */
#define BOOTLOADER_ADDRESS_FLAG                 0x00004C00 /**< Flash memory address for flag storage (1 KB). */
#define BOOTLOADER_ADDRESS_BANK_1               0x00005000 /**< Flash memory address for the first application bank (86 KB). */
#define BOOTLOADER_ADDRESS_BANK_2               0x0001A800 /**< Flash memory address for the second application bank (150 KB). */

/* Maximum time (in milliseconds) to wait for receiving the START command */
#define BOOTLOADER_TIMEOUT_START_MS             10 /**< Timeout duration for receiving the START message. */

#endif

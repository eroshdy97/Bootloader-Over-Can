/**
 * @file SENDER_config.h
 * @brief SENDER driver configuration file.
 *
 * This file contains configuration constants and macros for the SENDER module.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _SENDER_CONFIG_H
#define _SENDER_CONFIG_H

/* CAN message IDs */
#define SENDER_CAN_MSG_ID_RESET             0x00001000 /**< CAN message ID for the reset command. */
#define SENDER_CAN_MSG_ID_START             0x00001001 /**< CAN message ID for the start command. */
#define SENDER_CAN_MSG_ID_END               0x00001002 /**< CAN message ID for the end command. */
#define SENDER_CAN_MSG_ID_DATA              0x00001003 /**< CAN message ID for data frames. */
#define SENDER_CAN_MSG_ID_CRC               0x00001004 /**< CAN message ID for CRC validation. */

/* CAN message lengths */
#define SENDER_CAN_MSG_LENGTH_DATA          4 /**< Length of data frames in bytes. */
#define SENDER_CAN_MSG_LENGTH_END           1 /**< Length of end command messages in bytes. */
#define SENDER_CAN_MSG_LENGTH_START         4 /**< Length of start command messages in bytes. */
#define SENDER_CAN_MSG_LENGTH_RESET         1 /**< Length of reset command messages in bytes. */
#define SENDER_CAN_MSG_LENGTH_CRC           4 /**< Length of CRC command messages in bytes. */

/* CAN controller IDs */
#define SENDER_CAN_CONTROLLER_ID_RESET      1 /**< CAN controller ID for reset command. */
#define SENDER_CAN_CONTROLLER_ID_START      2 /**< CAN controller ID for start command. */
#define SENDER_CAN_CONTROLLER_ID_DATA       3 /**< CAN controller ID for data frames. */
#define SENDER_CAN_CONTROLLER_ID_END        4 /**< CAN controller ID for end command. */
#define SENDER_CAN_CONTROLLER_ID_CRC        5 /**< CAN controller ID for CRC validation. */

#endif

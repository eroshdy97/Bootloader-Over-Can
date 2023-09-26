
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: BOOTLOADER driver configuration file                                 
*/

#ifndef _BOOTLOADER_CONFIG_H
#define _BOOTLOADER_CONFIG_H

#define BOOTLOADER_CAN_MSG_ID_RESET             0x00001000
#define BOOTLOADER_CAN_MSG_ID_START             0x00001001
#define BOOTLOADER_CAN_MSG_ID_END               0x00001002
#define BOOTLOADER_CAN_MSG_ID_DATA              0x00001003

#define BOOTLOADER_CAN_MSG_LENGTH_DATA          4
#define BOOTLOADER_CAN_MSG_LENGTH_END           1
#define BOOTLOADER_CAN_MSG_LENGTH_START         4
#define BOOTLOADER_CAN_MSG_LENGTH_RESET         1

#define BOOTLOADER_CAN_CONTROLLER_ID_DATA       1
#define BOOTLOADER_CAN_CONTROLLER_ID_END        2
#define BOOTLOADER_CAN_CONTROLLER_ID_START      3
#define BOOTLOADER_CAN_CONTROLLER_ID_RESET      4

#define BOOTLOADER_ADDRESS_FLAG                 0x00004C00      /* 1   KB */
#define BOOTLOADER_ADDRESS_BANK_1               0x00005000      /* 86  KB */
#define BOOTLOADER_ADDRESS_BANK_2               0x0001A800      /* 150 KB */

#define BOOTLOADER_TIMEOUT_START_MS             1

#endif


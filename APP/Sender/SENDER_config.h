
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: SENDER driver configuration file                                 
*/

#ifndef _SENDER_CONFIG_H
#define _SENDER_CONFIG_H

#define SENDER_CAN_MSG_ID_RESET             0x00001000
#define SENDER_CAN_MSG_ID_START             0x00001001
#define SENDER_CAN_MSG_ID_END               0x00001002
#define SENDER_CAN_MSG_ID_DATA              0x00001003

#define SENDER_CAN_MSG_LENGTH_DATA          4
#define SENDER_CAN_MSG_LENGTH_END           1
#define SENDER_CAN_MSG_LENGTH_START         4
#define SENDER_CAN_MSG_LENGTH_RESET         1

#define SENDER_CAN_CONTROLLER_ID_DATA       1
#define SENDER_CAN_CONTROLLER_ID_END        2
#define SENDER_CAN_CONTROLLER_ID_START      3
#define SENDER_CAN_CONTROLLER_ID_RESET      4


#endif



/*Author:      Mohamed Gamal                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: CANMANGER driver private file                                       
*/
#ifndef _CANMANGER_PRIVATE_H
#define _CANMANGER_PRIVATE_H

/*Create a CAN object for transmission*/
tCANMsgObject gCANMsgTobj;

/*Create a CAN object for Receiving*/
tCANMsgObject gCANMsgRobj;

/*Interrupt return status*/
uint32_t gui32Status;

/*Interrupt error flag*/
bool g_bErrFlag = false;

/*Array of pointers to function for each controller ID*/
void(*gpnfs[32])(void);


#endif


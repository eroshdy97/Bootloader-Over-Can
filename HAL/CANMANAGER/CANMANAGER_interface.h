/*Author:      Mohamed Gamal                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: CANMANGER driver interface file      
*/
#ifndef _CANMANGER_INTERFACE_H
#define _CANMANGER_INTERFACE_H

/*Global variables*/

/*Interrupt return status*/
uint32_t gui32Status;
/*Interrupt error flag*/
bool gbErrFlag = false;
/*Create a CAN object for transmission*/
tCANMsgObject gCANMsgTobj;
/*Create a CAN object for Receiving*/
tCANMsgObject gCANMsgRobj;
/*Array of pointers to function for each controller ID*/
void(*gpnfs[32])(void);

/*Function Prototypes*/
void CANMANAGER_Delay_us(uint32_t us);
void CANMANAGER_Init(void);
bool CANMANAGER_Transmit(uint32_t ui32MsgID, uint8_t ui8MsgLength, uint32_t * pui32Data,uint8_t ui8CANControllerId,void(*pFn)(void));
void CANMANAGER_ObjReceiveSet(uint32_t u32MsgID, uint8_t u8MsgLength, uint32_t * pu32Data,uint32_t , uint8_t u8CANControllerId,void(*pFn)(void));
bool CANMANAGER_ObjReceiveGet(uint32_t * pu32Data, uint8_t u8CANControllerId);



#endif

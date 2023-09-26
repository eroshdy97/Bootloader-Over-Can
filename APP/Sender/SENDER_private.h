
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: SENDER driver private file                                       
*/
#ifndef _SENDER_PRIVATE_H
#define _SENDER_PRIVATE_H

#define SENDER_FLASH_TO_BANK_1                 1
#define SENDER_FLASH_TO_BANK_2                 2

#define VTABLE_OFFSET_R                 (*((volatile unsigned long *)0xE000ED08))

typedef enum
{
    SenderStateIdle,
    SenderStateSendStart,
    SenderStateSendData,
    SenderStateDone,

} SenderStates;

static SenderStates geCurruntState = SenderStateIdle;
static uint32_t gu32MsgCount = 0;

#endif


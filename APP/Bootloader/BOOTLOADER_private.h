
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: BOOTLOADER driver private file                                       
*/
#ifndef _BOOTLOADER_PRIVATE_H
#define _BOOTLOADER_PRIVATE_H

#define VTABLE_OFFSET_R                 (*((volatile unsigned long *)0xE000ED08))
#define PROGRAM_TO_RUN_R                (*((volatile unsigned long *)BOOTLOADER_ADDRESS_FLAG))

#define STRINGIFY(x) #x
#define ASM_MOVE_2RO(X)                 " mov r0, #" STRINGIFY(X) "\n"

typedef enum
{
    BLStateIdle,
    BLStateReceivingData,
    BLStateDone,

} BLStates;

static BLStates geCurruntState = BLStateIdle;
static bool gbDataFrameReceived = false;
static uint32_t gu32FlashToBank = 0;
static uint32_t gu32DataReceivedLength = 0;
static uint32_t gu32DataReceived[5000];

static void WriteAppToFlash(uint32_t* src, uint32_t dst, uint32_t count);
static void Jmp2App(uint32_t u32Address2jmp);
#endif


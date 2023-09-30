
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: BOOTLOADER driver private file                                       
*/
#ifndef _BOOTLOADER_PRIVATE_H
#define _BOOTLOADER_PRIVATE_H

/*enum for boot-loader states*/
typedef enum
{
    BLStateIdle,
    BLStateReceivingData,
    BLStateCRC,
    BLStateDone,

} BLStates;


/*Register definitions related to the boot-loader*/
#define VTABLE_OFFSET_R                 (*((volatile unsigned long *)0xE000ED08))
#define PROGRAM_TO_RUN_R                (*((volatile unsigned long *)BOOTLOADER_ADDRESS_FLAG))

/*static global variables*/
static BLStates geCurruntState = BLStateIdle;
static bool gbDataFrameReceived = false;
static uint32_t gu32FlashToBank = 0;
static uint32_t gu32DataReceivedLength = 0;
static uint32_t gu32DataReceived[5000];


/*static function prototypes*/
static void WriteAppToFlash(uint32_t* src, uint32_t dst, uint32_t count);
static void JumpToApp(uint32_t u32Address2jmp);
#endif


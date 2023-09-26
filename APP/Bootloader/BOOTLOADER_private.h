
/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: BOOTLOADER driver private file                                       
*/
#ifndef _BOOTLOADER_PRIVATE_H
#define _BOOTLOADER_PRIVATE_H

/*defines*/
// Define the mv_INST macro to stringify the Address macro
#define STRINGIFY(x) #x
#define ASM_MOVE_2RO(X)    " mov r0, #" STRINGIFY(X) "\n"

/*Registers*/
#define VTABLE_OFFSET_R              (*((volatile unsigned long *)0xE000ED08))
#define PROGRAM_TO_RUN              (*((volatile unsigned long *)ADDRESS_FLAG))


/*Function Prototypes*/
/*Write recieved APP IN Flash*/
static void WriteApp2Flash(uint32_t* src, uint32_t dst, uint32_t count);
static void Jmp2App(uint32_t Address2jmp);
#endif


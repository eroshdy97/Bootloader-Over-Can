#include "def.h"
#if !isTX

#include "RX/RX.h"

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"

#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"

#define CAN_MSG_ID_RESET            0x00001000
#define CAN_MSG_ID_START            0x00001001
#define CAN_MSG_ID_END              0x00001002
#define CAN_MSG_ID_DATA             0x00001003

#define ADDRESS_BOOTLOADER_1        0x00000000
#define ADDRESS_BANK_1              0x00010000
#define ADDRESS_BANK_2              0x00020000
#define ADDRESS_VARS                0x00030000

#define VTABLE_OFFSET               (*((volatile unsigned long *)0xE000ED08))
#define PROGRAM_TO_RUN              (*((volatile unsigned long *)ADDRESS_VARS))

volatile uint32_t flashToBank = 0;
volatile uint32_t dataReceivedLength = 0;
volatile bool dataFrameReceived = false;
volatile bool dataEnd = false;
volatile bool dataStart = false;
volatile bool canErrorFlag = false;
volatile bool resetFrame = false;


void CANIntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        canErrorFlag = true;
    }
    else if(ui32Status == 1) /* data frame */
    {
        CANIntClear(CAN0_BASE, 1);
        dataReceivedLength++;
        dataFrameReceived = true;
        canErrorFlag = false;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
    else if (ui32Status == 2) /* end frame */
    {
        CANIntClear(CAN0_BASE, 2);
        dataFrameReceived = false;
        dataEnd = true;
        canErrorFlag = false;
    }
    else if (ui32Status == 3) /* start frame */
    {
        CANIntClear(CAN0_BASE, 3);
        dataStart = true;
        canErrorFlag = false;
    }
    else if (ui32Status == 4) /* reset frame */
    {
        CANIntClear(CAN0_BASE, 4);
        SysCtlReset();
        canErrorFlag = false;
    }
    else
    {
        // Spurious interrupt handling can go here.
    }
}

void moveApptoRun(uint32_t* src, uint32_t dst, uint32_t count)
{
    int i = 0;
    for (i = 0; i < count; i+=1024)
    {
        FlashErase(dst + i);
    }
    FlashProgram(src, dst, count);
}

uint32_t data[5000];
void RX(void)
{

    tCANMsgObject canMsgStart;
    tCANMsgObject canMsgEnd;
    tCANMsgObject canMsgData;
    tCANMsgObject canMsgReset;
    uint32_t data32bit;
    uint32_t programToRun = PROGRAM_TO_RUN;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    CANInit(CAN0_BASE);

    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);

    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    IntEnable(INT_CAN0);

    CANEnable(CAN0_BASE);

    canMsgData.ui32MsgID = CAN_MSG_ID_DATA;
    canMsgData.ui32MsgIDMask = 0xfffff;
    canMsgData.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    canMsgData.ui32MsgLen = 4;
    CANMessageSet(CAN0_BASE, 1, &canMsgData, MSG_OBJ_TYPE_RX);

    canMsgEnd.ui32MsgID = CAN_MSG_ID_END;
    canMsgEnd.ui32MsgIDMask = 0xfffff;
    canMsgEnd.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    canMsgEnd.ui32MsgLen = 1;
    CANMessageSet(CAN0_BASE, 2, &canMsgEnd, MSG_OBJ_TYPE_RX);

    canMsgStart.ui32MsgID = CAN_MSG_ID_START;
    canMsgStart.ui32MsgIDMask = 0xfffff;
    canMsgStart.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    canMsgStart.ui32MsgLen = 4;
    CANMessageSet(CAN0_BASE, 3, &canMsgStart, MSG_OBJ_TYPE_RX);

    canMsgReset.ui32MsgID = CAN_MSG_ID_RESET;
    canMsgReset.ui32MsgIDMask = 0xfffff;
    canMsgReset.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    canMsgReset.ui32MsgLen = 1;
    CANMessageSet(CAN0_BASE, 4, &canMsgReset, MSG_OBJ_TYPE_RX);

//    SysCtlDelay(5000 * 16000/3);

    uint32_t timeOutCounter = 3180;
    while(!dataStart && (timeOutCounter > 0))
        timeOutCounter--;

    if(programToRun == 1 && !dataStart) /* if the bootloader runs and the last app flashed is in bank1 */
    {
        CANMessageGet(CAN0_BASE, 3, &canMsgStart, 0);
        VTABLE_OFFSET |= ADDRESS_BANK_1;

        __asm(" mov r0, #0x00010000\n");
        __asm(" ldr r1, [r0, #4]");
        __asm(" bx r1\n");
    }
    else if(programToRun == 2 && !dataStart) /* if the bootloader runs and the last app flashed is in bank2 */
    {
        CANMessageGet(CAN0_BASE, 3, &canMsgStart, 0);
        VTABLE_OFFSET |= ADDRESS_BANK_2;

        __asm(" mov r0, #0x00020000\n");
        __asm(" ldr r1, [r0, #4]");
        __asm(" bx r1\n");
    }
    else /* if the bootloader runs and there is no apps in bank1 or bank2 or there is data incoming*/
    {
        canMsgStart.pui8MsgData = (uint8_t *)&flashToBank;
        while(!dataStart);
        CANMessageGet(CAN0_BASE, 3, &canMsgStart, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        while(!dataEnd)
        {
            if(dataFrameReceived)
            {
                canMsgData.pui8MsgData = (uint8_t *)&data32bit;
                CANMessageGet(CAN0_BASE, 1, &canMsgData, 0);
                data[dataReceivedLength - 1] = data32bit;
                dataFrameReceived = 0;
            }
        }
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

        data[55] = &CANIntHandler;

        switch (flashToBank)
        {
            case 2:
            {
//                shiftVTable(data, ADDRESS_BANK_2);
                moveApptoRun(data, ADDRESS_BANK_2, dataReceivedLength * 4);
                dataReceivedLength = 0;

                programToRun = 2;
                FlashErase(ADDRESS_VARS);
                FlashProgram(&programToRun, ADDRESS_VARS, sizeof(programToRun));

                VTABLE_OFFSET |= ADDRESS_BANK_2;

                __asm(" mov r0, #0x00020000\n");
                __asm(" ldr r1, [r0, #4]");
                __asm(" bx r1\n");

                break;
            }
            case 1:
            {
//                shiftVTable(data, ADDRESS_BANK_1);
                moveApptoRun(data, ADDRESS_BANK_1, dataReceivedLength * 4);
                dataReceivedLength = 0;

                programToRun = 1;
                FlashErase(ADDRESS_VARS);
                FlashProgram(&programToRun, ADDRESS_VARS, sizeof(programToRun));

                VTABLE_OFFSET |= ADDRESS_BANK_1;

                __asm(" mov r0, #0x00010000\n");
                __asm(" ldr r1, [r0, #4]");
                __asm(" bx r1\n");

                break;
            }
            default:
                break;
        }

    }





    while(1);

}
#endif

//#include "test/test.h"
//#include "HEX.h"
//
//#include <stdbool.h>
//#include <stdint.h>
//#include "inc/hw_can.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "inc/hw_flash.h"
//
//#include "driverlib/can.h"
//#include "driverlib/gpio.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/flash.h"
//
//#define ADDR_SHIFT          0x00010000
//
//#define x1 (*((volatile unsigned long *)0xE000ED08))
//
////void moveApptoRun(uint32_t* src, uint32_t dst, uint32_t count)
////{
////    int i = 0;
////    for (i = 0; i < count; i+=1024)
////    {
////        FlashErase(dst + i);
////    }
////    FlashProgram(src, dst, count);
////}
////
////void ShiftVTable(uint32_t* app, uint32_t shift)
////{
////    int i = 0;
////    for (i = 1; i < 155; ++i)
////    {
////        if(app[i])
////        {
////            app[i] += shift;
////        }
////    }
////}
//
//void test(void)
//{
//
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//
//    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
//
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
//
//    ShiftVTable(CANProjectToFlash_image_0, ADDR_SHIFT);
//
//    moveApptoRun(CANProjectToFlash_image_0, ADDR_SHIFT, size_array * 4);
//
//    x1 |= 0x10000;
//
//    __asm(" mov r0, #0x00010000\n");
//    __asm(" ldr r1, [r0, #4]");
//    __asm(" bx r1\n");
//
//
//    while(1);
//
//}

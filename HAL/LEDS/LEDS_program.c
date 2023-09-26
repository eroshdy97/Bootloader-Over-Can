/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: LEDS driver program file    
*/
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "./HAL/LEDS/LEDS_interface.h"
#include "./HAL/LEDS/LEDS_private.h"
#include "./HAL/LEDS/LEDS_config.h"


void LEDS_Init(void)
{
    /*Clock gating for PortF*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /*Set all LEDS as output*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void LEDS_ON(uint8_t ui8LEDPin)
{
    if(ui8LEDPin<=GPIO_PIN_4)
    {
    GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, ui8LEDPin);
    }

}

void LEDS_OFF(uint8_t ui8LEDPin)
{
    if(ui8LEDPin<=GPIO_PIN_4)
    {
    GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, 0);
    }

}

void LEDS_Blink(uint8_t ui8LEDPin,uint32_t ui32DelayMs)
{
    if(ui8LEDPin<=GPIO_PIN_4)
    {
    GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin,ui8LEDPin);
    SysCtlDelay(ui32DelayMs*16000/3);
    GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, 0);
    SysCtlDelay(ui32DelayMs*16000/3);
    }

}


/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: LEDS driver interface file      
*/
#ifndef _LEDS_INTERFACE_H
#define _LEDS_INTERFACE_H

/*Defines*/
#define RED_LED             0x00000002
#define BLUE_LED            0x00000004
#define GREEN_LED           0x00000008

/*Function Prototypes*/
void LEDS_Init(void);
void LEDS_ON(uint8_t ui8LEDPin);
void LEDS_OFF(uint8_t ui8LEDPin);

#endif

/*Author:      Mohamed Gamal - Ehab Roushdy                                                                      
  Version:     V01                                                                       
  Date:        25/09/2023                                                                             
  Description: LEDS driver interface file      
*/
#ifndef _LEDS_INTERFACE_H
#define _LEDS_INTERFACE_H

/*Defines*/
#define RED_LED GPIO_PIN_1
#define BLUE_LED GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

/*Function Prototypes*/
void LEDS_Init(void);
void LEDS_ON(uint8_t ui8LEDPin);
void LEDS_OFF(uint8_t ui8LEDPin);

#endif

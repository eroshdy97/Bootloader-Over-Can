/**
 * @file SENDER_interface.h
 * @brief SENDER driver interface file.
 *
 * This file contains the function prototypes for initializing and starting the SENDER module.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _SENDER_INTERFACE_H
#define _SENDER_INTERFACE_H

/**
 * @brief Initialize the SENDER module.
 *
 * This function initializes the SENDER module and related peripherals.
 */
void SENDER_Init(void);

/**
 * @brief Start the SENDER module.
 *
 * This function initiates the SENDER module's operation.
 */
void SENDER_Start(void);

#endif

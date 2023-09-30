/**
 * @file CANMANAGER_program.c
 * @brief CAN Manager driver program file.
 *
 * This file contains the implementation of the CAN Manager module for Tiva-C microcontroller.
 * It provides functions for initializing CAN, transmitting and receiving messages, and handling interrupts.
 *
 * @author Mohamed Gamal
 * @version V01
 * @date 25/09/2023
 */

/* Include necessary libraries and header files */

#include "stdbool.h"
#include "stdint.h"

#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

/* Include CAN Manager specific files */
#include "./HAL/CANMANAGER/CANMANAGER_config.h"
#include "./HAL/CANMANAGER/CANMANAGER_private.h"
#include "./HAL/CANMANAGER/CANMANAGER_interface.h"

/**
 * @brief Delay in microseconds.
 *
 * This function introduces a delay in microseconds using SysCtlDelay.
 *
 * @param[in] us The delay time in microseconds.
 */
void CANMANAGER_Delay_us(uint32_t us)
{
    /* 110-200 us are enough */
    SysCtlDelay(us * 16 / 3);
}

/**
 * @brief CAN interrupt handler.
 *
 * This function is the interrupt handler for CAN. It handles error status and useful message IDs.
 *
 * @note The global variables `gui32Status`, `g_bErrFlag`, `gpnfs` are assumed to be defined elsewhere.
 */
void CANMANAGER_IntHandler(void)
{
    gui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    /* Error status */
    if (gui32Status == CAN_INT_INTID_STATUS)
    {
        gui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = true;
    }
    /* Useful Message ID received */
    else
    {
        CANIntClear(CAN0_BASE, gui32Status);
        /* Call the associated function with the message ID sent */
        gpnfs[gui32Status]();
        g_bErrFlag = false;
    }
}

/**
 * @brief Initialize CAN Manager.
 *
 * This function initializes the CAN Manager by configuring GPIO, CAN, interrupts, and enabling the CAN peripheral.
 */
void CANMANAGER_Init(void)
{
    /* GPIO Inits and Configs for CAN */
    /* Clock gating port B for CAN0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    /* Configure the needed CAN pins */
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* CAN Inits and Configs */

    /* Clock gating for CAN0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    /* Init CAN0 */
    CANInit(CAN0_BASE);

    /* Set CAN Rate */
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);

    /* Enable Interrupts */
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    IntEnable(INT_CAN0);
    IntMasterEnable();

    /* Enable CAN peripheral */
    CANEnable(CAN0_BASE);
}

/**
 * @brief Transmit a CAN message.
 *
 * This function transmits a CAN message with the provided parameters.
 *
 * @param[in] ui32MsgID Message ID.
 * @param[in] ui8MsgLength Message length.
 * @param[in] pui32Data Pointer to message data.
 * @param[in] ui8CANControllerId CAN controller ID.
 * @param[in] pFn Callback function to be associated with this message.
 * @return true if the transmission is successful, false otherwise.
 */
bool CANMANAGER_Transmit(uint32_t ui32MsgID, uint8_t ui8MsgLength, uint32_t *pui32Data, uint8_t ui8CANControllerId, void (*pFn)(void))
{
    /* Fill the needed parameters for the CAN object */
    gCANMsgTobj.ui32MsgID = ui32MsgID;
    gCANMsgTobj.ui32MsgIDMask = 0;
    gCANMsgTobj.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    gCANMsgTobj.ui32MsgLen = ui8MsgLength;
    gCANMsgTobj.pui8MsgData = (uint8_t *)pui32Data;

    /* Set the associated callback function with this object */
    gpnfs[ui8CANControllerId] = pFn;

    CANMessageSet(CAN0_BASE, ui8CANControllerId, &gCANMsgTobj, MSG_OBJ_TYPE_TX);
    CANMANAGER_Delay_us(200);

    if (g_bErrFlag)
    {
        /* Reset error flag for next time */
        g_bErrFlag = false;
        return false;
    }
    else if (gui32Status == ui8CANControllerId)
    {
        gui32Status = 0;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Set up a CAN object to receive messages.
 *
 * This function sets up a CAN object to receive messages with the provided parameters and associates a callback function.
 *
 * @param[in] u32MsgID Message ID.
 * @param[in] u8MsgLength Message length.
 * @param[out] pu32Data Pointer to data storage.
 * @param[in] u8CANControllerId CAN controller ID.
 * @param[in] pFn Callback function to be associated with this object.
 */
void CANMANAGER_ObjReceiveSet(uint32_t u32MsgID, uint8_t u8MsgLength, uint32_t *pu32Data, uint8_t u8CANControllerId, void (*pFn)(void))
{
    /* Fill the needed parameters for the CAN object */
    gCANMsgRobj.ui32MsgID = u32MsgID;
    gCANMsgRobj.ui32MsgIDMask = 0xfffff;
    gCANMsgRobj.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    gCANMsgRobj.ui32MsgLen = u8MsgLength;
    gCANMsgRobj.pui8MsgData = (uint8_t *)pu32Data;

    /* Set the message to the specified object */
    CANMessageSet(CAN0_BASE, u8CANControllerId, &gCANMsgRobj, MSG_OBJ_TYPE_RX);
    /* Set callback */
    gpnfs[u8CANControllerId] = pFn;
}

/**
 * @brief Receive a CAN message.
 *
 * This function receives a CAN message and stores it in the provided data pointer.
 *
 * @param[out] pu32Data Pointer to store the received message data.
 * @param[in] u8CANControllerId CAN controller ID.
 * @return true if the reception is successful, false otherwise.
 */
bool CANMANAGER_ObjReceiveGet(uint32_t *pu32Data, uint8_t u8CANControllerId)
{
    /* Receive the data at the desired variable */
    gCANMsgRobj.pui8MsgData = (uint8_t *)pu32Data;
    /* Receive the message at the object set */
    if (g_bErrFlag)
    {
        /* Reset error flag for next time */
        g_bErrFlag = false;
        return false;
    }
    else if (gui32Status == u8CANControllerId)
    {
        CANMessageGet(CAN0_BASE, u8CANControllerId, &gCANMsgRobj, 0);
        gui32Status = 0;
        return true;
    }
    else
    {
        return false;
    }
}

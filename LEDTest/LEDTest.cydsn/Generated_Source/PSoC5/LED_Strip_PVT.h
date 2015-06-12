/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_LED_Strip_H)
#define CY_SPIM_PVT_LED_Strip_H

#include "LED_Strip.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 LED_Strip_swStatusTx;
extern volatile uint8 LED_Strip_swStatusRx;

#if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 LED_Strip_txBuffer[LED_Strip_TX_BUFFER_SIZE];
    extern volatile uint8 LED_Strip_txBufferRead;
    extern volatile uint8 LED_Strip_txBufferWrite;
    extern volatile uint8 LED_Strip_txBufferFull;
#endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

#if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 LED_Strip_rxBuffer[LED_Strip_RX_BUFFER_SIZE];
    extern volatile uint8 LED_Strip_rxBufferRead;
    extern volatile uint8 LED_Strip_rxBufferWrite;
    extern volatile uint8 LED_Strip_rxBufferFull;
#endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_LED_Strip_H */


/* [] END OF FILE */

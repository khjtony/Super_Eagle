/*******************************************************************************
* File Name: LED_Strip_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LED_Strip_PVT.h"

/* User code required at start of ISR */
/* `#START LED_Strip_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: LED_Strip_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  LED_Strip_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  LED_Strip_txBuffer[LED_Strip_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(LED_Strip_TX_ISR)
{
    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START LED_Strip_TX_ISR_START` */

    /* `#END` */

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(LED_Strip_txBufferRead != LED_Strip_txBufferWrite)
        {
            tmpStatus = LED_Strip_GET_STATUS_TX(LED_Strip_swStatusTx);
            LED_Strip_swStatusTx = tmpStatus;

            if(0u != (LED_Strip_swStatusTx & LED_Strip_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == LED_Strip_txBufferFull)
                {
                   LED_Strip_txBufferRead++;

                    if(LED_Strip_txBufferRead >= LED_Strip_TX_BUFFER_SIZE)
                    {
                        LED_Strip_txBufferRead = 0u;
                    }
                }
                else
                {
                    LED_Strip_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(LED_Strip_TXDATA_PTR, 
                                             LED_Strip_txBuffer[LED_Strip_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(LED_Strip_txBufferRead == LED_Strip_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            LED_Strip_TX_STATUS_MASK_REG &= ((uint8) ~LED_Strip_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START LED_Strip_TX_ISR_END` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: LED_Strip_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  LED_Strip_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  LED_Strip_rxBuffer[LED_Strip_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(LED_Strip_RX_ISR)
{
    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START LED_Strip_RX_ISR_START` */

    /* `#END` */

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = LED_Strip_GET_STATUS_RX(LED_Strip_swStatusRx);
        LED_Strip_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (LED_Strip_swStatusRx & LED_Strip_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(LED_Strip_RXDATA_PTR);

            /* Set next pointer. */
            LED_Strip_rxBufferWrite++;
            if(LED_Strip_rxBufferWrite >= LED_Strip_RX_BUFFER_SIZE)
            {
                LED_Strip_rxBufferWrite = 0u;
            }

            if(LED_Strip_rxBufferWrite == LED_Strip_rxBufferRead)
            {
                LED_Strip_rxBufferRead++;
                if(LED_Strip_rxBufferRead >= LED_Strip_RX_BUFFER_SIZE)
                {
                    LED_Strip_rxBufferRead = 0u;
                }

                LED_Strip_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            LED_Strip_rxBuffer[LED_Strip_rxBufferWrite] = rxData;

            tmpStatus = LED_Strip_GET_STATUS_RX(LED_Strip_swStatusRx);
            LED_Strip_swStatusRx = tmpStatus;
        }

    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START LED_Strip_RX_ISR_END` */

    /* `#END` */
}

/* [] END OF FILE */

/*******************************************************************************
* File Name: LED_Strip.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the SPI Master component.
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

#if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 LED_Strip_txBuffer[LED_Strip_TX_BUFFER_SIZE];
    volatile uint8 LED_Strip_txBufferFull;
    volatile uint8 LED_Strip_txBufferRead;
    volatile uint8 LED_Strip_txBufferWrite;
#endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

#if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 LED_Strip_rxBuffer[LED_Strip_RX_BUFFER_SIZE];
    volatile uint8 LED_Strip_rxBufferFull;
    volatile uint8 LED_Strip_rxBufferRead;
    volatile uint8 LED_Strip_rxBufferWrite;
#endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

uint8 LED_Strip_initVar = 0u;

volatile uint8 LED_Strip_swStatusTx;
volatile uint8 LED_Strip_swStatusRx;


/*******************************************************************************
* Function Name: LED_Strip_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_Init(void) 
{
    /* Initialize the Bit counter */
    LED_Strip_COUNTER_PERIOD_REG = LED_Strip_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != LED_Strip_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (LED_Strip_TX_ISR_NUMBER);
        CyIntSetPriority     (LED_Strip_TX_ISR_NUMBER,  LED_Strip_TX_ISR_PRIORITY);
        (void) CyIntSetVector(LED_Strip_TX_ISR_NUMBER, &LED_Strip_TX_ISR);
    #endif /* (0u != LED_Strip_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != LED_Strip_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (LED_Strip_RX_ISR_NUMBER);
        CyIntSetPriority     (LED_Strip_RX_ISR_NUMBER,  LED_Strip_RX_ISR_PRIORITY);
        (void) CyIntSetVector(LED_Strip_RX_ISR_NUMBER, &LED_Strip_RX_ISR);
    #endif /* (0u != LED_Strip_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    LED_Strip_ClearFIFO();

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
        LED_Strip_rxBufferFull  = 0u;
        LED_Strip_rxBufferRead  = 0u;
        LED_Strip_rxBufferWrite = 0u;
    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        LED_Strip_txBufferFull  = 0u;
        LED_Strip_txBufferRead  = 0u;
        LED_Strip_txBufferWrite = 0u;
    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    (void) LED_Strip_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) LED_Strip_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    LED_Strip_TX_STATUS_MASK_REG = LED_Strip_TX_INIT_INTERRUPTS_MASK;
    LED_Strip_RX_STATUS_MASK_REG = LED_Strip_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: LED_Strip_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LED_Strip_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    LED_Strip_COUNTER_CONTROL_REG |= LED_Strip_CNTR_ENABLE;
    LED_Strip_TX_STATUS_ACTL_REG  |= LED_Strip_INT_ENABLE;
    LED_Strip_RX_STATUS_ACTL_REG  |= LED_Strip_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != LED_Strip_INTERNAL_CLOCK)
        LED_Strip_IntClock_Enable();
    #endif /* (0u != LED_Strip_INTERNAL_CLOCK) */

    LED_Strip_EnableTxInt();
    LED_Strip_EnableRxInt();
}


/*******************************************************************************
* Function Name: LED_Strip_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_Start(void) 
{
    if(0u == LED_Strip_initVar)
    {
        LED_Strip_Init();
        LED_Strip_initVar = 1u;
    }

    LED_Strip_Enable();
}


/*******************************************************************************
* Function Name: LED_Strip_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void LED_Strip_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    LED_Strip_TX_STATUS_ACTL_REG &= ((uint8) ~LED_Strip_INT_ENABLE);
    LED_Strip_RX_STATUS_ACTL_REG &= ((uint8) ~LED_Strip_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != LED_Strip_INTERNAL_CLOCK)
        LED_Strip_IntClock_Disable();
    #endif /* (0u != LED_Strip_INTERNAL_CLOCK) */

    LED_Strip_DisableTxInt();
    LED_Strip_DisableRxInt();
}


/*******************************************************************************
* Function Name: LED_Strip_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_EnableTxInt(void) 
{
    #if(0u != LED_Strip_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(LED_Strip_TX_ISR_NUMBER);
    #endif /* (0u != LED_Strip_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_EnableRxInt(void) 
{
    #if(0u != LED_Strip_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(LED_Strip_RX_ISR_NUMBER);
    #endif /* (0u != LED_Strip_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_DisableTxInt(void) 
{
    #if(0u != LED_Strip_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(LED_Strip_TX_ISR_NUMBER);
    #endif /* (0u != LED_Strip_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_DisableRxInt(void) 
{
    #if(0u != LED_Strip_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(LED_Strip_RX_ISR_NUMBER);
    #endif /* (0u != LED_Strip_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LED_Strip_SetTxInterruptMode(uint8 intSrc) 
{
    LED_Strip_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: LED_Strip_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LED_Strip_SetRxInterruptMode(uint8 intSrc) 
{
    LED_Strip_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: LED_Strip_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  LED_Strip_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LED_Strip_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LED_Strip_DisableTxInt();

        tmpStatus = LED_Strip_GET_STATUS_TX(LED_Strip_swStatusTx);
        LED_Strip_swStatusTx = 0u;

        LED_Strip_EnableTxInt();

    #else

        tmpStatus = LED_Strip_TX_STATUS_REG;

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: LED_Strip_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  LED_Strip_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LED_Strip_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        LED_Strip_DisableRxInt();

        tmpStatus = LED_Strip_GET_STATUS_RX(LED_Strip_swStatusRx);
        LED_Strip_swStatusRx = 0u;

        LED_Strip_EnableRxInt();

    #else

        tmpStatus = LED_Strip_RX_STATUS_REG;

    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: LED_Strip_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  LED_Strip_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  LED_Strip_txBuffer[LED_Strip_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_WriteTxData(uint8 txData) 
{
    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = LED_Strip_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (LED_Strip_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == LED_Strip_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        LED_Strip_DisableTxInt();

        tempStatus = LED_Strip_GET_STATUS_TX(LED_Strip_swStatusTx);
        LED_Strip_swStatusTx = tempStatus;


        if((LED_Strip_txBufferRead == LED_Strip_txBufferWrite) &&
           (0u != (LED_Strip_swStatusTx & LED_Strip_STS_TX_FIFO_NOT_FULL)))
        {
            /* Put data element into the TX FIFO */
            CY_SET_REG8(LED_Strip_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            LED_Strip_txBufferWrite++;
            if(LED_Strip_txBufferWrite >= LED_Strip_TX_BUFFER_SIZE)
            {
                LED_Strip_txBufferWrite = 0u;
            }

            if(LED_Strip_txBufferWrite == LED_Strip_txBufferRead)
            {
                LED_Strip_txBufferRead++;
                if(LED_Strip_txBufferRead >= LED_Strip_TX_BUFFER_SIZE)
                {
                    LED_Strip_txBufferRead = 0u;
                }
                LED_Strip_txBufferFull = 1u;
            }

            LED_Strip_txBuffer[LED_Strip_txBufferWrite] = txData;

            LED_Strip_TX_STATUS_MASK_REG |= LED_Strip_STS_TX_FIFO_NOT_FULL;
        }

        LED_Strip_EnableTxInt();

    #else
        /* Wait until TX FIFO has a place */
        while(0u == (LED_Strip_TX_STATUS_REG & LED_Strip_STS_TX_FIFO_NOT_FULL))
        {
        }

        /* Put data element into the TX FIFO */
        CY_SET_REG8(LED_Strip_TXDATA_PTR, txData);

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  LED_Strip_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  LED_Strip_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  LED_Strip_rxBuffer[LED_Strip_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LED_Strip_ReadRxData(void) 
{
    uint8 rxData;

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        LED_Strip_DisableRxInt();

        if(LED_Strip_rxBufferRead != LED_Strip_rxBufferWrite)
        {
            if(0u == LED_Strip_rxBufferFull)
            {
                LED_Strip_rxBufferRead++;
                if(LED_Strip_rxBufferRead >= LED_Strip_RX_BUFFER_SIZE)
                {
                    LED_Strip_rxBufferRead = 0u;
                }
            }
            else
            {
                LED_Strip_rxBufferFull = 0u;
            }
        }

        rxData = LED_Strip_rxBuffer[LED_Strip_rxBufferRead];

        LED_Strip_EnableRxInt();

    #else

        rxData = CY_GET_REG8(LED_Strip_RXDATA_PTR);

    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: LED_Strip_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  LED_Strip_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  LED_Strip_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 LED_Strip_GetRxBufferSize(void) 
{
    uint8 size;

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        LED_Strip_DisableRxInt();

        if(LED_Strip_rxBufferRead == LED_Strip_rxBufferWrite)
        {
            size = 0u;
        }
        else if(LED_Strip_rxBufferRead < LED_Strip_rxBufferWrite)
        {
            size = (LED_Strip_rxBufferWrite - LED_Strip_rxBufferRead);
        }
        else
        {
            size = (LED_Strip_RX_BUFFER_SIZE - LED_Strip_rxBufferRead) + LED_Strip_rxBufferWrite;
        }

        LED_Strip_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (LED_Strip_RX_STATUS_REG & LED_Strip_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: LED_Strip_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  LED_Strip_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  LED_Strip_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  LED_Strip_GetTxBufferSize(void) 
{
    uint8 size;

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LED_Strip_DisableTxInt();

        if(LED_Strip_txBufferRead == LED_Strip_txBufferWrite)
        {
            size = 0u;
        }
        else if(LED_Strip_txBufferRead < LED_Strip_txBufferWrite)
        {
            size = (LED_Strip_txBufferWrite - LED_Strip_txBufferRead);
        }
        else
        {
            size = (LED_Strip_TX_BUFFER_SIZE - LED_Strip_txBufferRead) + LED_Strip_txBufferWrite;
        }

        LED_Strip_EnableTxInt();

    #else

        size = LED_Strip_TX_STATUS_REG;

        if(0u != (size & LED_Strip_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & LED_Strip_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = LED_Strip_FIFO_SIZE;
        }

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: LED_Strip_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  LED_Strip_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(LED_Strip_RX_STATUS_REG & LED_Strip_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(LED_Strip_RXDATA_PTR);
    }

    #if(LED_Strip_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        LED_Strip_DisableRxInt();

        LED_Strip_rxBufferFull  = 0u;
        LED_Strip_rxBufferRead  = 0u;
        LED_Strip_rxBufferWrite = 0u;

        LED_Strip_EnableRxInt();
    #endif /* (LED_Strip_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: LED_Strip_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LED_Strip_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  LED_Strip_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    LED_Strip_AUX_CONTROL_DP0_REG |= ((uint8)  LED_Strip_TX_FIFO_CLR);
    LED_Strip_AUX_CONTROL_DP0_REG &= ((uint8) ~LED_Strip_TX_FIFO_CLR);

    #if(LED_Strip_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        LED_Strip_AUX_CONTROL_DP1_REG |= ((uint8)  LED_Strip_TX_FIFO_CLR);
        LED_Strip_AUX_CONTROL_DP1_REG &= ((uint8) ~LED_Strip_TX_FIFO_CLR);
    #endif /* (LED_Strip_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LED_Strip_DisableTxInt();

        LED_Strip_txBufferFull  = 0u;
        LED_Strip_txBufferRead  = 0u;
        LED_Strip_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        LED_Strip_TX_STATUS_MASK_REG &= ((uint8) ~LED_Strip_STS_TX_FIFO_NOT_FULL);

        LED_Strip_EnableTxInt();
    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != LED_Strip_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: LED_Strip_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void LED_Strip_TxEnable(void) 
    {
        LED_Strip_CONTROL_REG |= LED_Strip_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: LED_Strip_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void LED_Strip_TxDisable(void) 
    {
        LED_Strip_CONTROL_REG &= ((uint8) ~LED_Strip_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != LED_Strip_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: LED_Strip_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        LED_Strip_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: LED_Strip_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void LED_Strip_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(LED_Strip_RX_STATUS_REG & LED_Strip_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(LED_Strip_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    LED_Strip_AUX_CONTROL_DP0_REG |= ((uint8)  LED_Strip_TX_FIFO_CLR);
    LED_Strip_AUX_CONTROL_DP0_REG &= ((uint8) ~LED_Strip_TX_FIFO_CLR);

    #if(LED_Strip_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        LED_Strip_AUX_CONTROL_DP1_REG |= ((uint8)  LED_Strip_TX_FIFO_CLR);
        LED_Strip_AUX_CONTROL_DP1_REG &= ((uint8) ~LED_Strip_TX_FIFO_CLR);
    #endif /* (LED_Strip_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: LED_Strip_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_EnableInt(void) 
{
    LED_Strip_EnableRxInt();
    LED_Strip_EnableTxInt();
}


/*******************************************************************************
* Function Name: LED_Strip_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LED_Strip_DisableInt(void) 
{
    LED_Strip_DisableTxInt();
    LED_Strip_DisableRxInt();
}


/*******************************************************************************
* Function Name: LED_Strip_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LED_Strip_SetInterruptMode(uint8 intSrc) 
{
    LED_Strip_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~LED_Strip_STS_SPI_IDLE));
    LED_Strip_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: LED_Strip_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  LED_Strip_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LED_Strip_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(LED_Strip_TX_SOFTWARE_BUF_ENABLED || LED_Strip_RX_SOFTWARE_BUF_ENABLED)

        LED_Strip_DisableInt();

        tmpStatus  = LED_Strip_GET_STATUS_RX(LED_Strip_swStatusRx);
        tmpStatus |= LED_Strip_GET_STATUS_TX(LED_Strip_swStatusTx);
        tmpStatus &= ((uint8) ~LED_Strip_STS_SPI_IDLE);

        LED_Strip_swStatusTx = 0u;
        LED_Strip_swStatusRx = 0u;

        LED_Strip_EnableInt();

    #else

        tmpStatus  = LED_Strip_RX_STATUS_REG;
        tmpStatus |= LED_Strip_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~LED_Strip_STS_SPI_IDLE);

    #endif /* (LED_Strip_TX_SOFTWARE_BUF_ENABLED || LED_Strip_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */

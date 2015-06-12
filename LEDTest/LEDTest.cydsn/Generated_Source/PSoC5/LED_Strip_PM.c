/*******************************************************************************
* File Name: LED_Strip_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LED_Strip_PVT.h"

static LED_Strip_BACKUP_STRUCT LED_Strip_backup =
{
    LED_Strip_DISABLED,
    LED_Strip_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: LED_Strip_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LED_Strip_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: LED_Strip_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LED_Strip_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: LED_Strip_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LED_Strip_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_Sleep(void) 
{
    /* Save components enable state */
    LED_Strip_backup.enableState = ((uint8) LED_Strip_IS_ENABLED);

    LED_Strip_Stop();
}


/*******************************************************************************
* Function Name: LED_Strip_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LED_Strip_backup - used when non-retention registers are restored.
*  LED_Strip_txBufferWrite - modified every function call - resets to
*  zero.
*  LED_Strip_txBufferRead - modified every function call - resets to
*  zero.
*  LED_Strip_rxBufferWrite - modified every function call - resets to
*  zero.
*  LED_Strip_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LED_Strip_Wakeup(void) 
{
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

    /* Clear any data from the RX and TX FIFO */
    LED_Strip_ClearFIFO();

    /* Restore components block enable state */
    if(0u != LED_Strip_backup.enableState)
    {
        LED_Strip_Enable();
    }
}


/* [] END OF FILE */

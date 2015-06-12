/*******************************************************************************
* File Name: WDT_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "WDT.h"

static WDT_backupStruct WDT_backup;


/*******************************************************************************
* Function Name: WDT_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void WDT_SaveConfig(void) 
{
    #if (!WDT_UsingFixedFunction)
        WDT_backup.TimerUdb = WDT_ReadCounter();
        WDT_backup.InterruptMaskValue = WDT_STATUS_MASK;
        #if (WDT_UsingHWCaptureCounter)
            WDT_backup.TimerCaptureCounter = WDT_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!WDT_UDB_CONTROL_REG_REMOVED)
            WDT_backup.TimerControlRegister = WDT_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: WDT_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void WDT_RestoreConfig(void) 
{   
    #if (!WDT_UsingFixedFunction)

        WDT_WriteCounter(WDT_backup.TimerUdb);
        WDT_STATUS_MASK =WDT_backup.InterruptMaskValue;
        #if (WDT_UsingHWCaptureCounter)
            WDT_SetCaptureCount(WDT_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!WDT_UDB_CONTROL_REG_REMOVED)
            WDT_WriteControlRegister(WDT_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: WDT_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void WDT_Sleep(void) 
{
    #if(!WDT_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(WDT_CTRL_ENABLE == (WDT_CONTROL & WDT_CTRL_ENABLE))
        {
            /* Timer is enabled */
            WDT_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            WDT_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    WDT_Stop();
    WDT_SaveConfig();
}


/*******************************************************************************
* Function Name: WDT_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void WDT_Wakeup(void) 
{
    WDT_RestoreConfig();
    #if(!WDT_UDB_CONTROL_REG_REMOVED)
        if(WDT_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                WDT_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

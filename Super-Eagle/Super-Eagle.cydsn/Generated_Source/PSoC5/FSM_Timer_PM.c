/*******************************************************************************
* File Name: FSM_Timer_PM.c
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

#include "FSM_Timer.h"

static FSM_Timer_backupStruct FSM_Timer_backup;


/*******************************************************************************
* Function Name: FSM_Timer_SaveConfig
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
*  FSM_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void FSM_Timer_SaveConfig(void) 
{
    #if (!FSM_Timer_UsingFixedFunction)
        FSM_Timer_backup.TimerUdb = FSM_Timer_ReadCounter();
        FSM_Timer_backup.InterruptMaskValue = FSM_Timer_STATUS_MASK;
        #if (FSM_Timer_UsingHWCaptureCounter)
            FSM_Timer_backup.TimerCaptureCounter = FSM_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!FSM_Timer_UDB_CONTROL_REG_REMOVED)
            FSM_Timer_backup.TimerControlRegister = FSM_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: FSM_Timer_RestoreConfig
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
*  FSM_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void FSM_Timer_RestoreConfig(void) 
{   
    #if (!FSM_Timer_UsingFixedFunction)

        FSM_Timer_WriteCounter(FSM_Timer_backup.TimerUdb);
        FSM_Timer_STATUS_MASK =FSM_Timer_backup.InterruptMaskValue;
        #if (FSM_Timer_UsingHWCaptureCounter)
            FSM_Timer_SetCaptureCount(FSM_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!FSM_Timer_UDB_CONTROL_REG_REMOVED)
            FSM_Timer_WriteControlRegister(FSM_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: FSM_Timer_Sleep
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
*  FSM_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void FSM_Timer_Sleep(void) 
{
    #if(!FSM_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(FSM_Timer_CTRL_ENABLE == (FSM_Timer_CONTROL & FSM_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            FSM_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            FSM_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    FSM_Timer_Stop();
    FSM_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: FSM_Timer_Wakeup
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
*  FSM_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void FSM_Timer_Wakeup(void) 
{
    FSM_Timer_RestoreConfig();
    #if(!FSM_Timer_UDB_CONTROL_REG_REMOVED)
        if(FSM_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                FSM_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

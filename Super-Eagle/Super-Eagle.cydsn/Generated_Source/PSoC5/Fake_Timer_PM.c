/*******************************************************************************
* File Name: Fake_Timer_PM.c
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

#include "Fake_Timer.h"

static Fake_Timer_backupStruct Fake_Timer_backup;


/*******************************************************************************
* Function Name: Fake_Timer_SaveConfig
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
*  Fake_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Fake_Timer_SaveConfig(void) 
{
    #if (!Fake_Timer_UsingFixedFunction)
        Fake_Timer_backup.TimerUdb = Fake_Timer_ReadCounter();
        Fake_Timer_backup.InterruptMaskValue = Fake_Timer_STATUS_MASK;
        #if (Fake_Timer_UsingHWCaptureCounter)
            Fake_Timer_backup.TimerCaptureCounter = Fake_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Fake_Timer_UDB_CONTROL_REG_REMOVED)
            Fake_Timer_backup.TimerControlRegister = Fake_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Fake_Timer_RestoreConfig
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
*  Fake_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Fake_Timer_RestoreConfig(void) 
{   
    #if (!Fake_Timer_UsingFixedFunction)

        Fake_Timer_WriteCounter(Fake_Timer_backup.TimerUdb);
        Fake_Timer_STATUS_MASK =Fake_Timer_backup.InterruptMaskValue;
        #if (Fake_Timer_UsingHWCaptureCounter)
            Fake_Timer_SetCaptureCount(Fake_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Fake_Timer_UDB_CONTROL_REG_REMOVED)
            Fake_Timer_WriteControlRegister(Fake_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Fake_Timer_Sleep
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
*  Fake_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Fake_Timer_Sleep(void) 
{
    #if(!Fake_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Fake_Timer_CTRL_ENABLE == (Fake_Timer_CONTROL & Fake_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Fake_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Fake_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Fake_Timer_Stop();
    Fake_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Fake_Timer_Wakeup
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
*  Fake_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Fake_Timer_Wakeup(void) 
{
    Fake_Timer_RestoreConfig();
    #if(!Fake_Timer_UDB_CONTROL_REG_REMOVED)
        if(Fake_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Fake_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

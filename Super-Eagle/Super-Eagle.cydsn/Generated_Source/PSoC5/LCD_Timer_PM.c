/*******************************************************************************
* File Name: LCD_Timer_PM.c
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

#include "LCD_Timer.h"

static LCD_Timer_backupStruct LCD_Timer_backup;


/*******************************************************************************
* Function Name: LCD_Timer_SaveConfig
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
*  LCD_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void LCD_Timer_SaveConfig(void) 
{
    #if (!LCD_Timer_UsingFixedFunction)
        LCD_Timer_backup.TimerUdb = LCD_Timer_ReadCounter();
        LCD_Timer_backup.InterruptMaskValue = LCD_Timer_STATUS_MASK;
        #if (LCD_Timer_UsingHWCaptureCounter)
            LCD_Timer_backup.TimerCaptureCounter = LCD_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!LCD_Timer_UDB_CONTROL_REG_REMOVED)
            LCD_Timer_backup.TimerControlRegister = LCD_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: LCD_Timer_RestoreConfig
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
*  LCD_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void LCD_Timer_RestoreConfig(void) 
{   
    #if (!LCD_Timer_UsingFixedFunction)

        LCD_Timer_WriteCounter(LCD_Timer_backup.TimerUdb);
        LCD_Timer_STATUS_MASK =LCD_Timer_backup.InterruptMaskValue;
        #if (LCD_Timer_UsingHWCaptureCounter)
            LCD_Timer_SetCaptureCount(LCD_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!LCD_Timer_UDB_CONTROL_REG_REMOVED)
            LCD_Timer_WriteControlRegister(LCD_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: LCD_Timer_Sleep
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
*  LCD_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void LCD_Timer_Sleep(void) 
{
    #if(!LCD_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(LCD_Timer_CTRL_ENABLE == (LCD_Timer_CONTROL & LCD_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            LCD_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            LCD_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    LCD_Timer_Stop();
    LCD_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: LCD_Timer_Wakeup
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
*  LCD_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void LCD_Timer_Wakeup(void) 
{
    LCD_Timer_RestoreConfig();
    #if(!LCD_Timer_UDB_CONTROL_REG_REMOVED)
        if(LCD_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                LCD_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: CAN_PM.c
* Version 2.30
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CAN.h"

static CAN_BACKUP_STRUCT CAN_backup = 
{ 0u,
  0u,
  CAN_INIT_INTERRUPT_MASK,
  CAN_MODE_MASK,
  (((uint32) ((uint32) CAN_SYNC_EDGE     << CAN_EDGE_MODE_SHIFT))     | 
   ((uint32) ((uint32) CAN_SAMPLING_MODE << CAN_SAMPLE_MODE_SHIFT))   |
   ((uint32) ((uint32) CAN_CFG_REG_SJW   << CAN_CFG_REG_SJW_SHIFT))   | 
   ((uint32) ((uint32) CAN_RESET_TYPE    << CAN_RESET_SHIFT))         |
   ((uint32) ((uint32) CAN_CFG_REG_TSEG2 << CAN_CFG_REG_TSEG2_SHIFT)) |
   ((uint32) ((uint32) CAN_CFG_REG_TSEG1 << CAN_CFG_REG_TSEG1_SHIFT)) |
   ((uint32) ((uint32) CAN_ARBITER       << CAN_ARBITER_SHIFT))       |
   ((uint32) ((uint32) CAN_BITRATE       << CAN_BITRATE_SHIFT))) };


/*******************************************************************************
* Function Name: CAN_SaveConfig
********************************************************************************
*
* Summary:
*  Save CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
* 
* Global Variables:
*  CAN_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_SaveConfig(void) 
{
    CAN_backup.intSr = (CY_GET_REG32(CAN_INT_SR_PTR));
    CAN_backup.intEn = (CY_GET_REG32(CAN_INT_EN_PTR));
    CAN_backup.cmd = (CY_GET_REG32(CAN_CMD_PTR));
    CAN_backup.cfg = (CY_GET_REG32(CAN_CFG_PTR));
}


/*******************************************************************************
* Function Name: CAN_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  CAN_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer: CAN_INT_SR, 
*  CAN_INT_EN, CAN_CMD, CAN_CFG.
*
*******************************************************************************/
void CAN_RestoreConfig(void) 
{
    CY_SET_REG32(CAN_INT_SR_PTR, CAN_backup.intSr);
    CY_SET_REG32(CAN_INT_EN_PTR, CAN_backup.intEn);
    CY_SET_REG32(CAN_CMD_PTR, CAN_backup.cmd);
    CY_SET_REG32(CAN_CFG_PTR, CAN_backup.cfg);
}


/*******************************************************************************
* Function Name: CAN_Sleep
********************************************************************************
* 
* Summary:
*  Prepare CAN Component goes to sleep.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  CAN_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_Sleep(void) 
{   
    if (0u != (CY_GET_REG32(CAN_CMD_PTR) & CAN_MODE_MASK))
    {
        CAN_backup.enableState = 1u;
    }
    else /* The CAN block is disabled */
    {
        CAN_backup.enableState = 0u;
    }
    
    CAN_SaveConfig();
    (void) CAN_Stop();
}


/*******************************************************************************
* Function Name: CAN_Wakeup
********************************************************************************
* 
* Summary:
*  Prepare CAN Component to wake up.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  CAN_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_Wakeup(void) 
{           
    /* Enable power to CAN */
    CAN_PM_ACT_CFG_REG |= CAN_ACT_PWR_EN;
    
    CAN_RestoreConfig();
    
    if (CAN_backup.enableState != 0u)
    {
        /* Enable component's operation */
        (void) CAN_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

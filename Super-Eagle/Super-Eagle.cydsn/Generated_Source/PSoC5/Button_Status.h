/*******************************************************************************
* File Name: Button_Status.h  
* Version 1.80
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_Button_Status_H) /* CY_STATUS_REG_Button_Status_H */
#define CY_STATUS_REG_Button_Status_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Button_Status_Read(void) ;
void Button_Status_InterruptEnable(void) ;
void Button_Status_InterruptDisable(void) ;
void Button_Status_WriteMask(uint8 mask) ;
uint8 Button_Status_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define Button_Status_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define Button_Status_INPUTS              2


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Button_Status_Status             (* (reg8 *) Button_Status_sts_sts_reg__STATUS_REG )
#define Button_Status_Status_PTR         (  (reg8 *) Button_Status_sts_sts_reg__STATUS_REG )
#define Button_Status_Status_Mask        (* (reg8 *) Button_Status_sts_sts_reg__MASK_REG )
#define Button_Status_Status_Aux_Ctrl    (* (reg8 *) Button_Status_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_Button_Status_H */


/* [] END OF FILE */

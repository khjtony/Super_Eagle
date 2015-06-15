/*******************************************************************************
* File Name: BackLit.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BackLit_H) /* Pins BackLit_H */
#define CY_PINS_BackLit_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BackLit_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BackLit__PORT == 15 && ((BackLit__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    BackLit_Write(uint8 value) ;
void    BackLit_SetDriveMode(uint8 mode) ;
uint8   BackLit_ReadDataReg(void) ;
uint8   BackLit_Read(void) ;
uint8   BackLit_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BackLit_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define BackLit_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define BackLit_DM_RES_UP          PIN_DM_RES_UP
#define BackLit_DM_RES_DWN         PIN_DM_RES_DWN
#define BackLit_DM_OD_LO           PIN_DM_OD_LO
#define BackLit_DM_OD_HI           PIN_DM_OD_HI
#define BackLit_DM_STRONG          PIN_DM_STRONG
#define BackLit_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define BackLit_MASK               BackLit__MASK
#define BackLit_SHIFT              BackLit__SHIFT
#define BackLit_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BackLit_PS                     (* (reg8 *) BackLit__PS)
/* Data Register */
#define BackLit_DR                     (* (reg8 *) BackLit__DR)
/* Port Number */
#define BackLit_PRT_NUM                (* (reg8 *) BackLit__PRT) 
/* Connect to Analog Globals */                                                  
#define BackLit_AG                     (* (reg8 *) BackLit__AG)                       
/* Analog MUX bux enable */
#define BackLit_AMUX                   (* (reg8 *) BackLit__AMUX) 
/* Bidirectional Enable */                                                        
#define BackLit_BIE                    (* (reg8 *) BackLit__BIE)
/* Bit-mask for Aliased Register Access */
#define BackLit_BIT_MASK               (* (reg8 *) BackLit__BIT_MASK)
/* Bypass Enable */
#define BackLit_BYP                    (* (reg8 *) BackLit__BYP)
/* Port wide control signals */                                                   
#define BackLit_CTL                    (* (reg8 *) BackLit__CTL)
/* Drive Modes */
#define BackLit_DM0                    (* (reg8 *) BackLit__DM0) 
#define BackLit_DM1                    (* (reg8 *) BackLit__DM1)
#define BackLit_DM2                    (* (reg8 *) BackLit__DM2) 
/* Input Buffer Disable Override */
#define BackLit_INP_DIS                (* (reg8 *) BackLit__INP_DIS)
/* LCD Common or Segment Drive */
#define BackLit_LCD_COM_SEG            (* (reg8 *) BackLit__LCD_COM_SEG)
/* Enable Segment LCD */
#define BackLit_LCD_EN                 (* (reg8 *) BackLit__LCD_EN)
/* Slew Rate Control */
#define BackLit_SLW                    (* (reg8 *) BackLit__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BackLit_PRTDSI__CAPS_SEL       (* (reg8 *) BackLit__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BackLit_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BackLit__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BackLit_PRTDSI__OE_SEL0        (* (reg8 *) BackLit__PRTDSI__OE_SEL0) 
#define BackLit_PRTDSI__OE_SEL1        (* (reg8 *) BackLit__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BackLit_PRTDSI__OUT_SEL0       (* (reg8 *) BackLit__PRTDSI__OUT_SEL0) 
#define BackLit_PRTDSI__OUT_SEL1       (* (reg8 *) BackLit__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BackLit_PRTDSI__SYNC_OUT       (* (reg8 *) BackLit__PRTDSI__SYNC_OUT) 


#if defined(BackLit__INTSTAT)  /* Interrupt Registers */

    #define BackLit_INTSTAT                (* (reg8 *) BackLit__INTSTAT)
    #define BackLit_SNAP                   (* (reg8 *) BackLit__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BackLit_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: Rotary.h  
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

#if !defined(CY_PINS_Rotary_H) /* Pins Rotary_H */
#define CY_PINS_Rotary_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Rotary_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Rotary__PORT == 15 && ((Rotary__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Rotary_Write(uint8 value) ;
void    Rotary_SetDriveMode(uint8 mode) ;
uint8   Rotary_ReadDataReg(void) ;
uint8   Rotary_Read(void) ;
uint8   Rotary_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Rotary_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Rotary_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Rotary_DM_RES_UP          PIN_DM_RES_UP
#define Rotary_DM_RES_DWN         PIN_DM_RES_DWN
#define Rotary_DM_OD_LO           PIN_DM_OD_LO
#define Rotary_DM_OD_HI           PIN_DM_OD_HI
#define Rotary_DM_STRONG          PIN_DM_STRONG
#define Rotary_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Rotary_MASK               Rotary__MASK
#define Rotary_SHIFT              Rotary__SHIFT
#define Rotary_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Rotary_PS                     (* (reg8 *) Rotary__PS)
/* Data Register */
#define Rotary_DR                     (* (reg8 *) Rotary__DR)
/* Port Number */
#define Rotary_PRT_NUM                (* (reg8 *) Rotary__PRT) 
/* Connect to Analog Globals */                                                  
#define Rotary_AG                     (* (reg8 *) Rotary__AG)                       
/* Analog MUX bux enable */
#define Rotary_AMUX                   (* (reg8 *) Rotary__AMUX) 
/* Bidirectional Enable */                                                        
#define Rotary_BIE                    (* (reg8 *) Rotary__BIE)
/* Bit-mask for Aliased Register Access */
#define Rotary_BIT_MASK               (* (reg8 *) Rotary__BIT_MASK)
/* Bypass Enable */
#define Rotary_BYP                    (* (reg8 *) Rotary__BYP)
/* Port wide control signals */                                                   
#define Rotary_CTL                    (* (reg8 *) Rotary__CTL)
/* Drive Modes */
#define Rotary_DM0                    (* (reg8 *) Rotary__DM0) 
#define Rotary_DM1                    (* (reg8 *) Rotary__DM1)
#define Rotary_DM2                    (* (reg8 *) Rotary__DM2) 
/* Input Buffer Disable Override */
#define Rotary_INP_DIS                (* (reg8 *) Rotary__INP_DIS)
/* LCD Common or Segment Drive */
#define Rotary_LCD_COM_SEG            (* (reg8 *) Rotary__LCD_COM_SEG)
/* Enable Segment LCD */
#define Rotary_LCD_EN                 (* (reg8 *) Rotary__LCD_EN)
/* Slew Rate Control */
#define Rotary_SLW                    (* (reg8 *) Rotary__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Rotary_PRTDSI__CAPS_SEL       (* (reg8 *) Rotary__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Rotary_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Rotary__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Rotary_PRTDSI__OE_SEL0        (* (reg8 *) Rotary__PRTDSI__OE_SEL0) 
#define Rotary_PRTDSI__OE_SEL1        (* (reg8 *) Rotary__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Rotary_PRTDSI__OUT_SEL0       (* (reg8 *) Rotary__PRTDSI__OUT_SEL0) 
#define Rotary_PRTDSI__OUT_SEL1       (* (reg8 *) Rotary__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Rotary_PRTDSI__SYNC_OUT       (* (reg8 *) Rotary__PRTDSI__SYNC_OUT) 


#if defined(Rotary__INTSTAT)  /* Interrupt Registers */

    #define Rotary_INTSTAT                (* (reg8 *) Rotary__INTSTAT)
    #define Rotary_SNAP                   (* (reg8 *) Rotary__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Rotary_H */


/* [] END OF FILE */

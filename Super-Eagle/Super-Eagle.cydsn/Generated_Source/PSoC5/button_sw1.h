/*******************************************************************************
* File Name: button_sw1.h  
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

#if !defined(CY_PINS_button_sw1_H) /* Pins button_sw1_H */
#define CY_PINS_button_sw1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "button_sw1_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 button_sw1__PORT == 15 && ((button_sw1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    button_sw1_Write(uint8 value) ;
void    button_sw1_SetDriveMode(uint8 mode) ;
uint8   button_sw1_ReadDataReg(void) ;
uint8   button_sw1_Read(void) ;
uint8   button_sw1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define button_sw1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define button_sw1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define button_sw1_DM_RES_UP          PIN_DM_RES_UP
#define button_sw1_DM_RES_DWN         PIN_DM_RES_DWN
#define button_sw1_DM_OD_LO           PIN_DM_OD_LO
#define button_sw1_DM_OD_HI           PIN_DM_OD_HI
#define button_sw1_DM_STRONG          PIN_DM_STRONG
#define button_sw1_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define button_sw1_MASK               button_sw1__MASK
#define button_sw1_SHIFT              button_sw1__SHIFT
#define button_sw1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define button_sw1_PS                     (* (reg8 *) button_sw1__PS)
/* Data Register */
#define button_sw1_DR                     (* (reg8 *) button_sw1__DR)
/* Port Number */
#define button_sw1_PRT_NUM                (* (reg8 *) button_sw1__PRT) 
/* Connect to Analog Globals */                                                  
#define button_sw1_AG                     (* (reg8 *) button_sw1__AG)                       
/* Analog MUX bux enable */
#define button_sw1_AMUX                   (* (reg8 *) button_sw1__AMUX) 
/* Bidirectional Enable */                                                        
#define button_sw1_BIE                    (* (reg8 *) button_sw1__BIE)
/* Bit-mask for Aliased Register Access */
#define button_sw1_BIT_MASK               (* (reg8 *) button_sw1__BIT_MASK)
/* Bypass Enable */
#define button_sw1_BYP                    (* (reg8 *) button_sw1__BYP)
/* Port wide control signals */                                                   
#define button_sw1_CTL                    (* (reg8 *) button_sw1__CTL)
/* Drive Modes */
#define button_sw1_DM0                    (* (reg8 *) button_sw1__DM0) 
#define button_sw1_DM1                    (* (reg8 *) button_sw1__DM1)
#define button_sw1_DM2                    (* (reg8 *) button_sw1__DM2) 
/* Input Buffer Disable Override */
#define button_sw1_INP_DIS                (* (reg8 *) button_sw1__INP_DIS)
/* LCD Common or Segment Drive */
#define button_sw1_LCD_COM_SEG            (* (reg8 *) button_sw1__LCD_COM_SEG)
/* Enable Segment LCD */
#define button_sw1_LCD_EN                 (* (reg8 *) button_sw1__LCD_EN)
/* Slew Rate Control */
#define button_sw1_SLW                    (* (reg8 *) button_sw1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define button_sw1_PRTDSI__CAPS_SEL       (* (reg8 *) button_sw1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define button_sw1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) button_sw1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define button_sw1_PRTDSI__OE_SEL0        (* (reg8 *) button_sw1__PRTDSI__OE_SEL0) 
#define button_sw1_PRTDSI__OE_SEL1        (* (reg8 *) button_sw1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define button_sw1_PRTDSI__OUT_SEL0       (* (reg8 *) button_sw1__PRTDSI__OUT_SEL0) 
#define button_sw1_PRTDSI__OUT_SEL1       (* (reg8 *) button_sw1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define button_sw1_PRTDSI__SYNC_OUT       (* (reg8 *) button_sw1__PRTDSI__SYNC_OUT) 


#if defined(button_sw1__INTSTAT)  /* Interrupt Registers */

    #define button_sw1_INTSTAT                (* (reg8 *) button_sw1__INTSTAT)
    #define button_sw1_SNAP                   (* (reg8 *) button_sw1__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_button_sw1_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: BTN_Back.h  
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

#if !defined(CY_PINS_BTN_Back_H) /* Pins BTN_Back_H */
#define CY_PINS_BTN_Back_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BTN_Back_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BTN_Back__PORT == 15 && ((BTN_Back__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    BTN_Back_Write(uint8 value) ;
void    BTN_Back_SetDriveMode(uint8 mode) ;
uint8   BTN_Back_ReadDataReg(void) ;
uint8   BTN_Back_Read(void) ;
uint8   BTN_Back_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BTN_Back_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define BTN_Back_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define BTN_Back_DM_RES_UP          PIN_DM_RES_UP
#define BTN_Back_DM_RES_DWN         PIN_DM_RES_DWN
#define BTN_Back_DM_OD_LO           PIN_DM_OD_LO
#define BTN_Back_DM_OD_HI           PIN_DM_OD_HI
#define BTN_Back_DM_STRONG          PIN_DM_STRONG
#define BTN_Back_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define BTN_Back_MASK               BTN_Back__MASK
#define BTN_Back_SHIFT              BTN_Back__SHIFT
#define BTN_Back_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BTN_Back_PS                     (* (reg8 *) BTN_Back__PS)
/* Data Register */
#define BTN_Back_DR                     (* (reg8 *) BTN_Back__DR)
/* Port Number */
#define BTN_Back_PRT_NUM                (* (reg8 *) BTN_Back__PRT) 
/* Connect to Analog Globals */                                                  
#define BTN_Back_AG                     (* (reg8 *) BTN_Back__AG)                       
/* Analog MUX bux enable */
#define BTN_Back_AMUX                   (* (reg8 *) BTN_Back__AMUX) 
/* Bidirectional Enable */                                                        
#define BTN_Back_BIE                    (* (reg8 *) BTN_Back__BIE)
/* Bit-mask for Aliased Register Access */
#define BTN_Back_BIT_MASK               (* (reg8 *) BTN_Back__BIT_MASK)
/* Bypass Enable */
#define BTN_Back_BYP                    (* (reg8 *) BTN_Back__BYP)
/* Port wide control signals */                                                   
#define BTN_Back_CTL                    (* (reg8 *) BTN_Back__CTL)
/* Drive Modes */
#define BTN_Back_DM0                    (* (reg8 *) BTN_Back__DM0) 
#define BTN_Back_DM1                    (* (reg8 *) BTN_Back__DM1)
#define BTN_Back_DM2                    (* (reg8 *) BTN_Back__DM2) 
/* Input Buffer Disable Override */
#define BTN_Back_INP_DIS                (* (reg8 *) BTN_Back__INP_DIS)
/* LCD Common or Segment Drive */
#define BTN_Back_LCD_COM_SEG            (* (reg8 *) BTN_Back__LCD_COM_SEG)
/* Enable Segment LCD */
#define BTN_Back_LCD_EN                 (* (reg8 *) BTN_Back__LCD_EN)
/* Slew Rate Control */
#define BTN_Back_SLW                    (* (reg8 *) BTN_Back__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BTN_Back_PRTDSI__CAPS_SEL       (* (reg8 *) BTN_Back__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BTN_Back_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BTN_Back__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BTN_Back_PRTDSI__OE_SEL0        (* (reg8 *) BTN_Back__PRTDSI__OE_SEL0) 
#define BTN_Back_PRTDSI__OE_SEL1        (* (reg8 *) BTN_Back__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BTN_Back_PRTDSI__OUT_SEL0       (* (reg8 *) BTN_Back__PRTDSI__OUT_SEL0) 
#define BTN_Back_PRTDSI__OUT_SEL1       (* (reg8 *) BTN_Back__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BTN_Back_PRTDSI__SYNC_OUT       (* (reg8 *) BTN_Back__PRTDSI__SYNC_OUT) 


#if defined(BTN_Back__INTSTAT)  /* Interrupt Registers */

    #define BTN_Back_INTSTAT                (* (reg8 *) BTN_Back__INTSTAT)
    #define BTN_Back_SNAP                   (* (reg8 *) BTN_Back__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BTN_Back_H */


/* [] END OF FILE */

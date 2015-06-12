/*******************************************************************************
* File Name: clk.h  
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

#if !defined(CY_PINS_clk_H) /* Pins clk_H */
#define CY_PINS_clk_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "clk_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 clk__PORT == 15 && ((clk__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    clk_Write(uint8 value) ;
void    clk_SetDriveMode(uint8 mode) ;
uint8   clk_ReadDataReg(void) ;
uint8   clk_Read(void) ;
uint8   clk_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define clk_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define clk_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define clk_DM_RES_UP          PIN_DM_RES_UP
#define clk_DM_RES_DWN         PIN_DM_RES_DWN
#define clk_DM_OD_LO           PIN_DM_OD_LO
#define clk_DM_OD_HI           PIN_DM_OD_HI
#define clk_DM_STRONG          PIN_DM_STRONG
#define clk_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define clk_MASK               clk__MASK
#define clk_SHIFT              clk__SHIFT
#define clk_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define clk_PS                     (* (reg8 *) clk__PS)
/* Data Register */
#define clk_DR                     (* (reg8 *) clk__DR)
/* Port Number */
#define clk_PRT_NUM                (* (reg8 *) clk__PRT) 
/* Connect to Analog Globals */                                                  
#define clk_AG                     (* (reg8 *) clk__AG)                       
/* Analog MUX bux enable */
#define clk_AMUX                   (* (reg8 *) clk__AMUX) 
/* Bidirectional Enable */                                                        
#define clk_BIE                    (* (reg8 *) clk__BIE)
/* Bit-mask for Aliased Register Access */
#define clk_BIT_MASK               (* (reg8 *) clk__BIT_MASK)
/* Bypass Enable */
#define clk_BYP                    (* (reg8 *) clk__BYP)
/* Port wide control signals */                                                   
#define clk_CTL                    (* (reg8 *) clk__CTL)
/* Drive Modes */
#define clk_DM0                    (* (reg8 *) clk__DM0) 
#define clk_DM1                    (* (reg8 *) clk__DM1)
#define clk_DM2                    (* (reg8 *) clk__DM2) 
/* Input Buffer Disable Override */
#define clk_INP_DIS                (* (reg8 *) clk__INP_DIS)
/* LCD Common or Segment Drive */
#define clk_LCD_COM_SEG            (* (reg8 *) clk__LCD_COM_SEG)
/* Enable Segment LCD */
#define clk_LCD_EN                 (* (reg8 *) clk__LCD_EN)
/* Slew Rate Control */
#define clk_SLW                    (* (reg8 *) clk__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define clk_PRTDSI__CAPS_SEL       (* (reg8 *) clk__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define clk_PRTDSI__DBL_SYNC_IN    (* (reg8 *) clk__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define clk_PRTDSI__OE_SEL0        (* (reg8 *) clk__PRTDSI__OE_SEL0) 
#define clk_PRTDSI__OE_SEL1        (* (reg8 *) clk__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define clk_PRTDSI__OUT_SEL0       (* (reg8 *) clk__PRTDSI__OUT_SEL0) 
#define clk_PRTDSI__OUT_SEL1       (* (reg8 *) clk__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define clk_PRTDSI__SYNC_OUT       (* (reg8 *) clk__PRTDSI__SYNC_OUT) 


#if defined(clk__INTSTAT)  /* Interrupt Registers */

    #define clk_INTSTAT                (* (reg8 *) clk__INTSTAT)
    #define clk_SNAP                   (* (reg8 *) clk__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_clk_H */


/* [] END OF FILE */

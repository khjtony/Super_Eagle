/*******************************************************************************
* File Name: BTN_Back.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "BTN_Back.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 BTN_Back__PORT == 15 && ((BTN_Back__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: BTN_Back_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void BTN_Back_Write(uint8 value) 
{
    uint8 staticBits = (BTN_Back_DR & (uint8)(~BTN_Back_MASK));
    BTN_Back_DR = staticBits | ((uint8)(value << BTN_Back_SHIFT) & BTN_Back_MASK);
}


/*******************************************************************************
* Function Name: BTN_Back_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  BTN_Back_DM_STRONG     Strong Drive 
*  BTN_Back_DM_OD_HI      Open Drain, Drives High 
*  BTN_Back_DM_OD_LO      Open Drain, Drives Low 
*  BTN_Back_DM_RES_UP     Resistive Pull Up 
*  BTN_Back_DM_RES_DWN    Resistive Pull Down 
*  BTN_Back_DM_RES_UPDWN  Resistive Pull Up/Down 
*  BTN_Back_DM_DIG_HIZ    High Impedance Digital 
*  BTN_Back_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void BTN_Back_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(BTN_Back_0, mode);
}


/*******************************************************************************
* Function Name: BTN_Back_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro BTN_Back_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 BTN_Back_Read(void) 
{
    return (BTN_Back_PS & BTN_Back_MASK) >> BTN_Back_SHIFT;
}


/*******************************************************************************
* Function Name: BTN_Back_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 BTN_Back_ReadDataReg(void) 
{
    return (BTN_Back_DR & BTN_Back_MASK) >> BTN_Back_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(BTN_Back_INTSTAT) 

    /*******************************************************************************
    * Function Name: BTN_Back_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 BTN_Back_ClearInterrupt(void) 
    {
        return (BTN_Back_INTSTAT & BTN_Back_MASK) >> BTN_Back_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

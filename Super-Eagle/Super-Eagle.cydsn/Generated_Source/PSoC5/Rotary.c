/*******************************************************************************
* File Name: Rotary.c  
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
#include "Rotary.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Rotary__PORT == 15 && ((Rotary__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Rotary_Write
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
void Rotary_Write(uint8 value) 
{
    uint8 staticBits = (Rotary_DR & (uint8)(~Rotary_MASK));
    Rotary_DR = staticBits | ((uint8)(value << Rotary_SHIFT) & Rotary_MASK);
}


/*******************************************************************************
* Function Name: Rotary_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Rotary_DM_STRONG     Strong Drive 
*  Rotary_DM_OD_HI      Open Drain, Drives High 
*  Rotary_DM_OD_LO      Open Drain, Drives Low 
*  Rotary_DM_RES_UP     Resistive Pull Up 
*  Rotary_DM_RES_DWN    Resistive Pull Down 
*  Rotary_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Rotary_DM_DIG_HIZ    High Impedance Digital 
*  Rotary_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Rotary_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Rotary_0, mode);
}


/*******************************************************************************
* Function Name: Rotary_Read
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
*  Macro Rotary_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Rotary_Read(void) 
{
    return (Rotary_PS & Rotary_MASK) >> Rotary_SHIFT;
}


/*******************************************************************************
* Function Name: Rotary_ReadDataReg
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
uint8 Rotary_ReadDataReg(void) 
{
    return (Rotary_DR & Rotary_MASK) >> Rotary_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Rotary_INTSTAT) 

    /*******************************************************************************
    * Function Name: Rotary_ClearInterrupt
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
    uint8 Rotary_ClearInterrupt(void) 
    {
        return (Rotary_INTSTAT & Rotary_MASK) >> Rotary_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

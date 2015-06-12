/*******************************************************************************
* File Name: OK.c  
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
#include "OK.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 OK__PORT == 15 && ((OK__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: OK_Write
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
void OK_Write(uint8 value) 
{
    uint8 staticBits = (OK_DR & (uint8)(~OK_MASK));
    OK_DR = staticBits | ((uint8)(value << OK_SHIFT) & OK_MASK);
}


/*******************************************************************************
* Function Name: OK_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  OK_DM_STRONG     Strong Drive 
*  OK_DM_OD_HI      Open Drain, Drives High 
*  OK_DM_OD_LO      Open Drain, Drives Low 
*  OK_DM_RES_UP     Resistive Pull Up 
*  OK_DM_RES_DWN    Resistive Pull Down 
*  OK_DM_RES_UPDWN  Resistive Pull Up/Down 
*  OK_DM_DIG_HIZ    High Impedance Digital 
*  OK_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void OK_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(OK_0, mode);
}


/*******************************************************************************
* Function Name: OK_Read
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
*  Macro OK_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 OK_Read(void) 
{
    return (OK_PS & OK_MASK) >> OK_SHIFT;
}


/*******************************************************************************
* Function Name: OK_ReadDataReg
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
uint8 OK_ReadDataReg(void) 
{
    return (OK_DR & OK_MASK) >> OK_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(OK_INTSTAT) 

    /*******************************************************************************
    * Function Name: OK_ClearInterrupt
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
    uint8 OK_ClearInterrupt(void) 
    {
        return (OK_INTSTAT & OK_MASK) >> OK_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

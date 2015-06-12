/*******************************************************************************
* File Name: clk.c  
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
#include "clk.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 clk__PORT == 15 && ((clk__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: clk_Write
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
void clk_Write(uint8 value) 
{
    uint8 staticBits = (clk_DR & (uint8)(~clk_MASK));
    clk_DR = staticBits | ((uint8)(value << clk_SHIFT) & clk_MASK);
}


/*******************************************************************************
* Function Name: clk_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  clk_DM_STRONG     Strong Drive 
*  clk_DM_OD_HI      Open Drain, Drives High 
*  clk_DM_OD_LO      Open Drain, Drives Low 
*  clk_DM_RES_UP     Resistive Pull Up 
*  clk_DM_RES_DWN    Resistive Pull Down 
*  clk_DM_RES_UPDWN  Resistive Pull Up/Down 
*  clk_DM_DIG_HIZ    High Impedance Digital 
*  clk_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void clk_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(clk_0, mode);
}


/*******************************************************************************
* Function Name: clk_Read
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
*  Macro clk_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 clk_Read(void) 
{
    return (clk_PS & clk_MASK) >> clk_SHIFT;
}


/*******************************************************************************
* Function Name: clk_ReadDataReg
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
uint8 clk_ReadDataReg(void) 
{
    return (clk_DR & clk_MASK) >> clk_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(clk_INTSTAT) 

    /*******************************************************************************
    * Function Name: clk_ClearInterrupt
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
    uint8 clk_ClearInterrupt(void) 
    {
        return (clk_INTSTAT & clk_MASK) >> clk_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

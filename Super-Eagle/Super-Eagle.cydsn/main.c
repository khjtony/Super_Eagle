/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*  Series of horizontal bar graphs are drawn, left to right. First, row 0 is 
*  filled followed by a clear display. The bars move towards right, becoming 
*  shorter and shorter. The same happens in row 1. Finally a text on the 
*  display indicates completion of the demo. 
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <device.h>
#include "LCD_Bar.h"
#include "state_event.h"
#include "dash_board.h"

extern uint8 const CYCODE LCD_customFonts[];
volatile uint16_t event = 0x00;
volatile uint8_t state = 0x0;
volatile uint8_t current_page=0x0;
volatile uint8_t throttle;
volatile uint8_t battery=100;
volatile uint8_t brake;
volatile uint8_t current_page;


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main program does the following
*  1. Initializes the LCD.
*  2. Loads custom fonts. This step is required to create a bar graph.
*  3. Prints start of a demo string on the LCD.
*  4. For each row, displays a bar graph from left to right with the initial position 
*     moving towards right. After each graph the display is cleared.
*  5. Displays completion of the demo.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

/******************************************************************************
//volatile list
*******************************************************************************/
void fatal_fault(){
    CyGlobalIntDisable;
    for (;;){
    }
    return;
}

CY_ISR(ISR_SW1)
{
    button_sw1_ClearInterrupt();    
    //throttle+=1;
    event |= 1<<HV_UP;
    return; 
}

CY_ISR(ISR_SW2)
{
    button_sw2_ClearInterrupt();    
    event |= 1 << DRIVE_UP;
    return;
}


CY_ISR(LCD_Handler)
{
    LCD_Timer_STATUS;
    Dash_display_State(state);
    return;
}

CY_ISR(Fake_Handler)
{
    Fake_Timer_STATUS;
    throttle+=1;
    if (throttle>=255){
        throttle=0;
    }
    brake+=1;
    if (brake>=255){
        brake=0;
    }
    return;
}


CY_ISR(FSM_Handler){
    
    
    FSM_Timer_STATUS;
    switch(state)// FSM, should be in the highest int priority
		{
			case STARTUP:
				action_startup();
				if(event&(1<<HV_UP)) state = NEUTRAL;
				break;
			case NEUTRAL:
				action_neutral();
				if(event&(1<<DRIVE_UP)) state = DRIVE;
				if(event&(1<<CHARGE_UP)) state = PRECHARGE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case DRIVE:
				action_drive();
				if(event&(1<<NEUTRAL_UP) || event&(1<<CHARGE_UP)) state = NEUTRAL;
				if(event&(1<<SOFT_FAULT_SIG)) state = SOFT_FAULT;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				if(state != DRIVE) reset_drive_sound(); 
				break;
			case PRECHARGE:
				action_precharge();
				if(event&(1<<PRECHARGE_DONE)) state = CHARGING;
				if(event&(1<<CHARGE_DOWN)) state = NEUTRAL;
				if(state != PRECHARGE) reset_precharge_timer();
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case CHARGING:
				action_charging();
				//if(event&(1<<CHARGE_DOWN)) state = PRECHARGE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case SOFT_FAULT:
				action_soft_fault();
				if(event&(1<<SOFT_FAULT_REMEDIED)) state = DRIVE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				if(state != SOFT_FAULT) reset_fault_sound();
				break;
			case HARD_FAULT:
				action_hard_fault();
				for(;;);	// trap fault
				break;
			default: // INVALID STATE
				fatal_fault();
				break;
        }; // switch state
        event=0x0; //wipe the event signal
}





void main()
{   
    /* Enable the global interrupt */
    CyGlobalIntEnable;
    
    //setup startup state
    state = STARTUP;
    
    /* Enable the Interrupt component connected to Timer interrupt */
    FSM_ISR_StartEx(FSM_Handler);
    LCD_ISR_StartEx(LCD_Handler);
    isr_sw1_StartEx(ISR_SW1);
    isr_sw2_StartEx(ISR_SW2);
    Fake_ISR_StartEx(Fake_Handler);

	/* Start the components */
    FSM_Timer_Start();
    LCD_Timer_Start();
    Fake_Timer_Start();
    Throttle_ADC_Start();
    Brake_ADC_Start();
    
    
    
    /* Start LCD */
    LCD_Start();
    
    for(;;)
	{
      
        CyDelay(10u);
	} // main loop
	return;
    
}


/* [] END OF FILE */

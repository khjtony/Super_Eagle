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
#include "can_manager.h"

extern uint8 const CYCODE LCD_customFonts[];
volatile uint16_t event = 0x00;
volatile uint8_t state = 0x0;
volatile uint8_t current_page=0x0;
volatile uint8_t throttle;
volatile uint8_t battery=100;
volatile uint8_t brake;
volatile uint8_t current_page;
volatile Dash_Mode mode = COMMON;
volatile RGB LED_rgb;
volatile uint8_t LED_len;
volatile RGB LED_test;
extern uint8_t wdt_battery;
extern uint8_t wdt_padel;
extern uint8_t wdt_voltage;
extern uint8_t wdt_current;
extern uint8_t error_battery;
extern uint8_t error_padel;
extern uint32_t info_battery_voltage;
extern uint16_t info_battery_current;




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

CY_ISR(isr_rotary)
{
    if (mode == INFO){
        current_page += 1;
        if (current_page > MAX_PAGE){
            current_page = 1;
        }
    }
    return; 
}

CY_ISR(isr_OK)
{   
    if (mode == COMMON){
        mode = INFO;
    }else{
        mode = COMMON;
    }
    return; 
}

CY_ISR(isr_back)
{   
    mode = COMMON;
    return;
}


CY_ISR(LCD_Handler)
{

    
    LCD_Timer_STATUS;
    LCD_ClearDisplay();
    Dash_display_Mode(mode);
    Dash_display_State(state);
    Dash_display_Value();
    return;
}

CY_ISR(Fake_Handler)
{
    Fake_Timer_STATUS;
    return;
}


CY_ISR(FSM_Handler){
    FSM_Timer_STATUS;
        if (wdt_voltage>1){
        wdt_voltage -= 1;
    }else{
            info_battery_voltage = 0;
    }
    
    if (wdt_current>1){
        wdt_current -= 1;
    }else{
            info_battery_current = 0;
    }
    
    
    
/*
    switch(state)// FSM, should be in the highest int priority
		{
			case STARTUP:
				break;
			case NEUTRAL:
				break;
			case DRIVE:
				break;
			case PRECHARGE:
				break;
			case CHARGING:
				break;
			case SOFT_FAULT:
				break;
			case HARD_FAULT:
				for(;;);	// trap fault
				break;
			default: // INVALID STATE
				fatal_fault();
				break;
        }; // switch state
        event=0x0; //wipe the event signal
}
*/

}



int main()
{   
    /* Enable the global interrupt */
    CyGlobalIntEnable;
    
    //setup startup state
    //state = STARTUP;
    state = DRIVE;
    mode = COMMON;
    
    /* Enable the Interrupt component connected to Timer interrupt */
    //FSM_ISR_StartEx(FSM_Handler);
    LCD_ISR_StartEx(LCD_Handler);
    FSM_ISR_StartEx(FSM_Handler);
    isr_OK_StartEx(isr_OK);
    isr_back_StartEx(isr_back);
    isr_rotary_StartEx(isr_rotary);
    Fake_ISR_StartEx(Fake_Handler);

	
    
    /* Start the components */
    FSM_Timer_Start();
    LCD_Timer_Start();
    Fake_Timer_Start();
    can_init();

    
    
    /* Start LCD and initial others */
    LCD_Start();
    BackLit_Write(1);
    LCD_ClearDisplay();
    wdt_battery = 255;
    wdt_voltage = 255;
    wdt_padel = 255;
    
    LED_test.R=0x4;
    LED_test.G=0xa;
    LED_test.B=0x4;
    int8_t stamp=15;
    int8_t direction=-1;
    
    for(;;)
	{
        if (stamp<=0 || stamp >= 16){
            direction = 0-direction;
            stamp += direction;
        }else{
            stamp += direction;
        }
        //LED_update(LED_test, stamp);
        CyDelay(100u);
	} // main loop
	return 0;
    
}


/* [] END OF FILE */

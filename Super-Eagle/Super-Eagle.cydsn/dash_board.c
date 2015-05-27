/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "dash_board.h"
#include "LCD_Bar.h"

void _dash_state_drive(uint8_t);
void _dash_state_charging(uint8_t);

void Dash_display_Mode(Dash_Mode mode){
    LCD_Position(0u,0u);
    switch(mode){
        case COMMON:
            LCD_PrintString("MAIN: "); 
            break;
	    case INFO:
            LCD_PrintString("INFO:"); 
            break;
	    default:
            LCD_PrintString("Err: "); 
            break;
    }
}

void Dash_display_State(States newState){
    LCD_Position(0u,6u);
    switch(newState){
        case STARTUP:
            LCD_PrintString("START UP"); 
            break;
	    case NEUTRAL:
            LCD_PrintString("NEUTRAL"); 
            break;
	    case DRIVE:
            LCD_PrintString("DRIVING"); 
            break;
	    case PRECHARGE:
            LCD_PrintString("PRECHARGE"); 
            break;
	    case CHARGING:
            LCD_PrintString("CHARGING"); 
            break;
	    case SOFT_FAULT:
            LCD_PrintString("SOFT FAULT"); 
            break;
        case HARD_FAULT:
            LCD_PrintString("HARD FAULT"); 
            break;
    }
    return;
}


void Dash_display_Event(Events newEvent){
    switch(newEvent){
        case HV_UP:
            break;
	    case DRIVE_UP:
            break;
	    case NEUTRAL_UP:
            break;
	    case CHARGE_UP:
            break;
	    case PRECHARGE_DONE:
            break;
	    case CHARGE_DOWN:
            break;
        case SOFT_FAULT_SIG:
            break;
        case SOFT_FAULT_REMEDIED:
            break;
        case HARD_FAULT_SIG:
            break;
        case NONE:
            break;
    }
    return;
}

void Dash_display_Value(){
    //display two values, msg, or err_msg

}

/* [] END OF FILE */

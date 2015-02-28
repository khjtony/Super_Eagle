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


void Dash_display_State(States newState){
    LCD_ClearDisplay();
    switch(newState){
        case STARTUP:
            LCD_Position(0u, 4u);
            LCD_PrintString("START UP"); 
            break;
	    case NEUTRAL:
            LCD_Position(0u, 3u);
            LCD_PrintString("NEUTRAL"); 
            LCD_Position(1u, 3u);
            LCD_PrintString("System OK!"); 
            break;
	    case DRIVE:
            _dash_state_drive(0);
            break;
	    case PRECHARGE:
            LCD_Position(0u, 3u);
            LCD_PrintString("PRECHARGE"); 
            break;
	    case CHARGING:
            LCD_Position(0u, 3u);
            LCD_PrintString("CHARGING"); 
            _dash_state_charging(0);
            break;
	    case SOFT_FAULT:
            LCD_Position(0u, 3u);
            LCD_PrintString("SOFT FAULT"); 
            break;
        case HARD_FAULT:
            LCD_Position(0u, 3u);
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

void _dash_state_drive(uint8_t frame){
    uint8_t local_throttle=throttle;
    uint8_t local_brake=brake;
    switch(frame){
        case 0:
            
            //quickBar(0,LCD_MID,RIGHT,Throttle_ADC_GetResult8());
            quickBar(0,8,LEFT,local_brake);
            quickBar(0,8,RIGHT,local_throttle);
            

            //quickBar(0,LCD_MID-brake,LEFT,Throttle_ADC_GetResult8());
           // quickBar(0,LCD_MID,LEFT,25);
            
            
            quickBar(1,0,RIGHT,Throttle_ADC_GetResult8());
            break;
        case 1:

            quickBar(0,LCD_MID,RIGHT,throttle);
            break;
        case 2:
            LCD_Position(0u, 3u);
            LCD_PrintString("DRIVING!");
            break;
        case 3:
            quickBar(0,0,RIGHT,50);
            break;
    }
    return;       
}

void _dash_state_charging(uint8_t frame){
    return;
}

/* [] END OF FILE */

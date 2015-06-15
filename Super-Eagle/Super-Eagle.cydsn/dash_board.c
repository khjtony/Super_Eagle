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
#include "can_manager.h"
#include "LCD_Bar.h"

uint8_t STARTING_FRAME[4]={0x00,0x00,0x00,0x00};
uint8_t ENDING_FRAME[4]={0xff,0xff,0xff,0xff};

void _dash_state_drive(uint8_t);
void _dash_state_charging(uint8_t);
extern volatile Dash_Mode mode;


extern uint32_t info_battery_voltage;
extern int16_t info_battery_current;
extern uint16_t info_battery_temp;
extern uint16_t info_battery_status;
extern uint16_t info_car_speed;
extern uint16_t info_common_time;
extern uint8_t error_battery;
extern uint8_t error_padel;
extern uint8_t wdt_battery;
extern uint8_t wdt_padel;








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
    LCD_Position(0u,0u);
    
    if (error_battery || error_padel){
        LCD_Position(0u,8u);
        LCD_PrintString("ERR:");
        LCD_PrintHexUint16(error_battery<<8 | error_padel);
    }
    
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
    if (mode == COMMON){
        //left voltage right current
        if (info_battery_voltage<1*10){     //0~0.9V
            LCD_Position(1u,4u);
            LCD_PrintNumber(0);
        }else if(info_battery_voltage<10*10){       //1~9.9V
            LCD_Position(1u,4u);
            LCD_PrintNumber(info_battery_voltage/10);
        }else if(info_battery_voltage<100*10){      //10~99.9V
            LCD_Position(1u,3u);
            LCD_PrintNumber(info_battery_voltage/10);
        }else{
            LCD_Position(1u,2u);
            LCD_PrintNumber(info_battery_voltage/10);
        }
        
        
        LCD_Position(1u,5u);
        LCD_PrintString(".");
        LCD_Position(1u,6u);
        LCD_PrintNumber(info_battery_voltage%10);
        LCD_Position(1u,7u);
        LCD_PrintString("V");
        
        if ((info_battery_current)<1*10 | (info_battery_current)>-1*10){     //0~0.9A
            LCD_Position(1u,12u);
            LCD_PrintNumber(0);
        }else if(info_battery_current<10*10 | info_battery_current>-10*10){       //1~9.9A
            LCD_Position(1u,12u);
            LCD_PrintNumber(info_battery_current/10);
        }else if(info_battery_current<100*10 | info_battery_current>100*10){      //10~99.9A
            LCD_Position(1u,11u);
            LCD_PrintNumber(info_battery_current/10);
        }else{
            LCD_Position(1u,10u);
            LCD_PrintNumber(info_battery_current/10);
        }
        
        LCD_Position(1u,13u);
        LCD_PrintString(".");
        LCD_Position(1u,14u);
        LCD_PrintNumber(info_battery_current%10);
        LCD_Position(1u,15u);
        LCD_PrintString("A");
    }

}

void LED_update(RGB LED_rgb,uint8_t LED_len){
    uint8_t i=0;
    uint8_t single_frame[4];
    //send starting frame
    LED_Strip_PutArray(STARTING_FRAME,4);
    //send N LED frame
    single_frame[0]=0xff;
    single_frame[1]=LED_rgb.B;
    single_frame[2]=LED_rgb.G;
    single_frame[3]=LED_rgb.R;
    for (i=0;i<LED_len;i++){
        LED_Strip_PutArray(single_frame,4);
    }
    //send ending frame
    LED_Strip_PutArray(ENDING_FRAME,4);
}

/* [] END OF FILE */

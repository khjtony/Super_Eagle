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
#ifndef DASH_BOARD_H
#define DASH_BOARD_H

#define MAX_PAGE (5u)
    
#include <project.h>
#include "state_event.h"
    #define BAT_NO_ERROR (0x0)
    #define BAT_NO_CONNECTION (0x1)
    #define BAT_OVER_VOLTAGE (0x1<<1)
    #define BAT_UNDER_VOLTAGE (0x1<<2)
    #define BAT_OVER_TEMP (0x1<<3)
    #define BAT_UNDER_TEMP (0x1<<4)
    
    #define PADEL_NO_ERROR (0x0)
    #define PADEL_NO_CONNECTION (0x1)
    #define PADEL_THROTTLE_ERROR (0x1<<1)
    #define PADEL_BRAKE_ERROR (0x1<<2)
    
    
#define TOTAL_LED (16u)
    
typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB;


    
void Dash_display_Mode(Dash_Mode mode);
void Dash_display_State(States newState);
void Dash_display_Event(Events newEvent);
void Dash_display_Value();
void LED_update(RGB LED_rgb,uint8_t LED_len);

#endif
/* [] END OF FILE */

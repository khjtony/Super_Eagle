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
    

    
void Dash_display_Mode(Dash_Mode mode);
void Dash_display_State(States newState);
void Dash_display_Event(Events newEvent);
void Dash_display_Value();

#endif
/* [] END OF FILE */

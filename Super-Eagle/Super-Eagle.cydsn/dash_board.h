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
#ifndef _DASH_BOARD_H_
    #define _DASH_BOARD_H_
    
    #include "state_event.h"
    
    void Dash_display_State(States);
    void Dash_display_Event(Events);
    
    extern volatile uint8_t throttle;
    extern volatile uint8_t battery;
    extern volatile uint8_t brake;
    extern volatile uint8_t current_page;
    
#endif
/* [] END OF FILE */

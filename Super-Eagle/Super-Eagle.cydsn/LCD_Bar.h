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

#define LCD_ROWS 2u
#define LCD_MID 8u
#define LCD_COLUMNS 16u
#define LCD_WIDTH (16u*5)

#ifndef _LCD_BAR_H_
    #define _LCD_BAR_H_
    
    #include "device.h"
    
    typedef enum {
        LEFT=0,
        RIGHT=1
    } direction;
    
    void quickBar(uint8_t row, uint8_t colume, direction dir, uint8_t len);
    void ThrottleBar(uint8_t throttle);
    void BrakeBar(uint8_t brake);
    
    
#endif

/* [] END OF FILE */

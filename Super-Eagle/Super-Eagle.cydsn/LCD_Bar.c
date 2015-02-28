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
#include "LCD_Bar.h"

void quickBar(uint8_t row, uint8_t colume, direction dir, uint8_t len){
    uint8_t norLen=(uint8_t)(len*LCD_WIDTH/256/2);
    if (dir == RIGHT){
        LCD_DrawHorizontalBG(row, colume, (LCD_COLUMNS - LCD_ROWS), norLen);
    }
    else{
        LCD_DrawHorizontalBG(row, colume-norLen/5, (LCD_COLUMNS - LCD_ROWS), norLen);
    }
    return;
}




/* [] END OF FILE */

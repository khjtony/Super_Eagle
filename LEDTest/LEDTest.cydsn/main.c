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
#include <project.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    LED_Strip_Start();
    uint8_t i=0;
    uint8_t single_frame[4];
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint8_t STARTING_FRAME[4]={0x0,0x0,0x0,0x0};
    uint8_t ENDING_FRAME[4]={0x1,0x1,0x1,0x1};
    int8_t stamp=15;
    int8_t direction=-1;
    for(;;)
    {
         
    //send starting frame
    LED_Strip_PutArray(STARTING_FRAME,4);
    //send N LED frame
    single_frame[0]=0xff;
    single_frame[1]=0x4;
    single_frame[2]=0xa;
    single_frame[3]=0x4;
    for (i=0;i<stamp;i++){
        LED_Strip_PutArray(single_frame,4);
    }
    //send ending frame
    LED_Strip_PutArray(ENDING_FRAME,4);
        /* Place your application code here. */
    
    
    if (stamp<=0 || stamp >= 16){
        direction = 0-direction;
        stamp += direction;
    }else{
        stamp += direction;
    }
    CyDelay(40);
    }
}

/* [] END OF FILE */

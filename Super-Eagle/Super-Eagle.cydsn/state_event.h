#ifndef STATE_EVENT_H
#define STATE_EVENT_H

#include <device.h>
    
typedef enum
    {  
        COMMON,
        INFO
    }Dash_Mode;
    

typedef enum 
{
	STARTUP,
	NEUTRAL,
	DRIVE,
	PRECHARGE,
	CHARGING,
	SOFT_FAULT,
	HARD_FAULT
} States; // enum States



typedef enum 
{
	HV_UP,
	DRIVE_UP,
	NEUTRAL_UP,
	CHARGE_UP,
	PRECHARGE_DONE,
	CHARGE_DOWN,
	SOFT_FAULT_SIG,
	SOFT_FAULT_REMEDIED,
	HARD_FAULT_SIG,
    NONE
} Events ; // enum Events


void action_startup();
void action_neutral();
void action_drive();
void action_precharge();
void action_charging();
void action_soft_fault();
void action_hard_fault();
void fatal_fault();

void reset_drive_sound(); 
void reset_precharge_timer();
void reset_fault_sound();


#endif

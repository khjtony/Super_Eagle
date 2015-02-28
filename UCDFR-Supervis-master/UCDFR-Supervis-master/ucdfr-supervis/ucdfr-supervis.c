#include <stdint.h>
#include "hardware.h"
//#include "interface.h"
#include "state_event.h"



void fatal_fault()
{
	for(;;)
		action_fatal();
} // fatal_fault()



int main()
{
	uint8_t state = STARTUP;
	uint16_t event = 0x00;
	//interface_init();	// USB interface
	hard_init();	// initialize hardware

	for(;;)
	{
		event = 0x00; // wipe events
		get_inputs(&event);
		//usb_terminal(&event, &state);

		// FSM
		switch(state)
		{
			case STARTUP:
				action_startup();
				if(event&(1<<HV_UP)) state = NEUTRAL;
				break;
			case NEUTRAL:
				action_neutral();
				if(event&(1<<DRIVE_UP)) state = DRIVE;
				if(event&(1<<CHARGE_UP)) state = PRECHARGE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case DRIVE:
				action_drive();
				if(event&(1<<NEUTRAL_UP) || event&(1<<CHARGE_UP)) state = NEUTRAL;
				if(event&(1<<SOFT_FAULT_SIG)) state = SOFT_FAULT;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				if(state != DRIVE) reset_drive_sound(); 
				break;
			case PRECHARGE:
				action_precharge();
				if(event&(1<<PRECHARGE_DONE)) state = CHARGING;
				if(event&(1<<CHARGE_DOWN)) state = NEUTRAL;
				if(state != PRECHARGE) reset_precharge_timer();
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case CHARGING:
				action_charging();
				//if(event&(1<<CHARGE_DOWN)) state = PRECHARGE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				break;
			case SOFT_FAULT:
				action_soft_fault();
				if(event&(1<<SOFT_FAULT_REMEDIED)) state = DRIVE;
				if(event&(1<<HARD_FAULT_SIG)) state = HARD_FAULT;
				if(state != SOFT_FAULT) reset_fault_sound();
				break;
			case HARD_FAULT:
				action_hard_fault();
				for(;;);	// trap fault
				break;
			default: // INVALID STATE
				fatal_fault();
				break;
		}; // switch state
	} // main loop

	fatal_fault();
	return 0;
} // main()

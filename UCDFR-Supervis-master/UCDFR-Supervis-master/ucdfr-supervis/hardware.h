#ifndef	HARDWARE_H 
#define HARDWARE_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "state_event.h"

#define F_CPU 16000000UL // 16 MHz
#define CTC_COUNT (((F_CPU/1000)/8) - 1) // 1 millisecond CTC
#define INTERNAL_REF 1085	// internal 1.1V ref actual voltage

#define LED_MASK 0x8F	// masks PORTC for leds
#define BUZZER_MAX 1500	// beep duration
#define BUZZER_TRIG 50	// toggle beep every x ms
#define PRECHARGE_MAX 6000	// 6 second precharge
#define FAULT_MAX 3000
#define FAULT_TRIG 25

//#define BRAKE_FIFTEEN_PERCENT 400
#define BRAKE_FIFTEEN_PERCENT 575

//#define PEDAL2_BEGIN_IN_PEDAL1 1000 // Places of pedal1 vs pedal2 discontinuity
//#define PEDAL2_END_IN_PEDAL1 5000

// Express pedal 2 in terms of pedal 1
#define PEDAL_SLOPE_NOMIN 1672//2056L
#define PEDAL_SLOPE_DENOM 1000//1000L
#define PEDAL_INTERCEPT 166L

#define PEDAL1_DROPOUT 444 // 414
#define PEDAL2_DROPOUT 269 // 239

#define PEDAL1_TEN_PERCENT 200//400
#define PEDAL1_TWENTYFIVE_PERCENT 1025
#define BRAKE_MIN_MARGIN 575 // calibrate this
#define PEDAL_MIN_MARGIN 910//765 //910

/*
// Express controller's throttle as pedal1 travel
#define THRTTL_SLOPE 1
#define THRTTL_INTERCEPT 0
*/

/* Port A
A1	In	HV request
A2	In	Drive request
A3	In	Neutral request
*/

/* Port B
B0	Out Precharge Relay
B1	Out	PEE Power
B2	Out	ACUES Power
B3	Out	Drive Enable
B4	Out	Keyswitch Enable
B5	Out	AIR 2
B6	Out	AIR 1
B7	Out	Charger Enable
*/

/* Port C
C0	In	Left Estop (Active Low)
C1	In	Brake Overtravel (Active Low)
C2	In	Interlock Loop (Active Low)
C3	In	Inertia Switch (Active Low)
C4	Out	Red LED
C5	Out	Green LED
C6	Out	Blue LED
*/

/* Port D
D0	Out RTD Buzzer
D1	Out	Dash/TQV
D2	Out	Reserved 1 (Unused)
D3	Out Reserved 2 (Unused)
D4	In	Charger Detect
D5	In	Inv. 1 Contact Request
D6	In	Inv. 2 Contact Request
D7	In	Master Switch (Active Low)
*/

/* Port E
E0	In	Cockpit Estop (Active Low)
E1	In	Right Estop (Active Low)
E6	In	Powerstage IMD Error (Active Low)
E7	In	Powerstage BPS Error (Active Low)
*/

/* Port F
F0	In	GLV Sys Voltage
F1	In	DC Link Voltage (Unused)
F2	In	Brake Sensor
F3	In	Traction Sys Current (Unused)
F4	In	Throttle to Ctrlr 1 (Unused)
F5	In	Throttle to Ctrlr 2 (Throttle Sensor 1 without RIO)
F6	In	Throttle Sensor 2 
F7	In	Throttle Sensor 1 (Unused without RIO)
*/

/* State Color
Pin 4 R
Pin 5 G
Pin 6 B

B	G	R

B		Neutral
G		Drive
GB	Charging
R		Hard Fault
RB	Fatal	
RG	Soft Fault
RGB	Startup
*/


enum Color
{
	NO_COLOR,	// 000
	RED,			// 00R
	GREEN,		// 0G0
	YELLOW,		// 0GR
	BLUE,			// B00
	MAGENTA,	// B0R
	CYAN,			// BG0
	WHITE			// BGR
};


void hard_init();
uint32_t analog_vcc();
uint32_t analog_read(uint32_t *vcc, uint8_t f_pin);
void get_inputs(uint16_t *events);

void action_startup();
void action_neutral();
void action_precharge();
void action_charging();
void action_drive();
void action_soft_fault();
void action_hard_fault();
void action_fatal();

void reset_drive_sound();
void reset_precharge_timer();
void reset_fault_sound();
void set_led(uint8_t color);

#endif

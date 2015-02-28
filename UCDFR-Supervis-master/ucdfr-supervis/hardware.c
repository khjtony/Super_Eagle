#include "hardware.h"


volatile uint8_t buzz_now = 0;
volatile uint16_t buzzer_time = BUZZER_MAX;

volatile uint8_t precharge_now = 0;
volatile uint16_t precharge_time = PRECHARGE_MAX;
volatile uint8_t precharging_done = 0;

volatile uint8_t fault_now = 0;
volatile uint16_t fault_time = FAULT_MAX;


static uint8_t glv_sys_voltage_low = 0;



ISR(TIMER1_COMPA_vect)
{
	if(buzz_now)
	{
		if(buzzer_time > 0)
		{
			if(!(buzzer_time%BUZZER_TRIG))
				PORTD ^= (1<<0); // TOGGLE Buzzer 
			buzzer_time--;
		} // buzz
		else
		{
			PORTD &= ~(1<<0); // OFF Buzzer 
			buzz_now = 0;
		} // stop buzzer
	}

	if(precharge_now)
	{
		if(precharge_time > 0)
		{
			//PORTD |= (1<<0);	// ON Buzzer
			precharge_time--;
		} // delay contactor
		else
		{
			PORTD &= ~(1<<0);	// OFF Buzzer
			precharge_now = 0;
			precharging_done = 1;
		} // end delay
	} // if currently precharging

	if(fault_now)
	{
		if(fault_time > 0)
		{
			if(!(fault_time%FAULT_TRIG))
				PORTD ^= (1<<0); // TOGGLE Buzzer 
			fault_time--;
		} // buzz
		else
		{
			PORTD &= ~(1<<0); // OFF Buzzer 
			fault_now = 0;
		} // stop buzzer
	}

} // ISR(TIMER1)



void hard_init()
{
	CLKPR = 0x80, CLKPR = 0; // zero prescaler

	DDRA = (0x00);	// A1-A3 In
	DDRB = (0xFF);	// B0-B7 Out
	DDRC = (0x70);	// C0-C3 In, C4-C6 Out
	DDRD = (0x0F);	// D0-D3 Out, D4-D7 In
	DDRE = (0x00);	// E0-E7 In
	DDRF = (0x00);	// F0-F7 In

	PORTA = (0x0E);	// Pullup on A1-A3
	PORTC = (0x0F);	// Pullup on C0-C3
	PORTD = (0x80);	// Pullup on D7
	PORTE	= (0xC3);	// Pullup on E0-E1
	//PORTF = (0xFF);	// Pullup on F0-F7

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Prepare ADC
	ADCSRB = 0x80;	// "High Speed" ADC

	TCCR1B |= (1 << WGM12) | (1 << CS11); // Timer Prescale 8
	OCR1A = CTC_COUNT;										// Timer to CTC mode
	TIMSK1 |= (1 << OCIE1A);							// 1 ms
	sei();																// Enable Interrupts
} // inputs_init()



uint32_t analog_vcc()
{
	ADMUX = (1<<REFS0) | 0x1E;				// comapare vcc against MUX 1110 1.1V
	_delay_ms(2);											// VCC to settle
	ADCSRA |= (1<<ADSC); 							// begin conversion
	while(ADCSRA & (1<<ADSC)); 				// wait for the conversion to happen
	uint32_t vcc_value = ADCW;
	vcc_value = (INTERNAL_REF * (1023L)) / vcc_value;	// find Vcc in mV
	return vcc_value;									// Return VCC in mV
} // analog_vcc()



uint32_t analog_read(uint32_t *vcc, uint8_t f_pin)
{
	ADMUX = (1<<REFS0) | f_pin;					// MUX to f_pin. Compare voltage with AVCC
	ADCSRA |= (1<<ADSC); 								// begin conversion
	while(ADCSRA & (1<<ADSC)); 					// wait for the conversion to happen
	uint32_t f_value = ADCW;						// Read it. 10 bit precision
	f_value = (*vcc * f_value) / 1023;	// correct vcc reference
	return f_value;											// return read value in mV
} // analog_read()



void get_inputs(uint16_t *events)
{
	uint32_t vcc = analog_vcc();
	uint32_t brake_value = analog_read(&vcc, 3);
	uint32_t pedal1_value = analog_read(&vcc, 5);
	uint32_t pedal2_value = analog_read(&vcc, 6);
	//uint32_t controller_throttle1 = analog_read(&vcc, 4);
	//uint32_t controller_throttle2 = analog_read(&vcc, 5);

	int pedal2_in_pedal1 =
		((long)pedal2_value * PEDAL_SLOPE_NOMIN) / PEDAL_SLOPE_DENOM +
		PEDAL_INTERCEPT;

	int pedal_difference = (int)pedal1_value - (int)pedal2_in_pedal1;

	if(pedal_difference < 0)
		pedal_difference = -pedal_difference;
/*
	int throttle1_transform =
		THRTTL_SLOPE * controller_throttle1 + THRTTL_INTERCEPT;

	int throttle2_transform =
		THRTTL_SLOPE * controller_throttle2 + THRTTL_INTERCEPT;
*/
	// HV_UP
	if(!(PINA&(1<<1)))
		*events |= (1<<HV_UP);

	// DRIVE_UP
	if(!(PINA&(1<<2)) && PIND&(1<<5) && PIND&(1<<6)
		&& brake_value > BRAKE_FIFTEEN_PERCENT)
		*events |= (1<<DRIVE_UP);

	// NEUTRAL_UP
	if(!(PINA&(1<<3)))
		*events |= (1<<NEUTRAL_UP);

	// PRECHARGE_DONE
	if(precharging_done)
		*events |= (1<<PRECHARGE_DONE);

	// CHARGE_UP
	if(PIND&(1<<4))
		*events |= (1<<CHARGE_UP);

	// CHARGE_DOWN
	if(!(PIND&(1<<4)))
		*events |= (1<<CHARGE_DOWN);

	// SOFT_FAULT_SIG
	
	if(pedal_difference > PEDAL1_TEN_PERCENT)
		*events |= (1<<SOFT_FAULT_SIG);	// Pedal sensor comparison

	if(pedal1_value < PEDAL1_DROPOUT || pedal2_value < PEDAL2_DROPOUT)
		*events |= (1<<SOFT_FAULT_SIG);	// Pedal sensor disconnect
	
	if(brake_value > BRAKE_MIN_MARGIN && pedal1_value > PEDAL1_TWENTYFIVE_PERCENT)
		*events |= (1<<SOFT_FAULT_SIG); // Pedal and brake
		
/*
	if(pedal1_value < controller_throttle1 || pedal1_value > controller_throttle2)
		*events |= (1<<SOFT_FAULT_SIG); // Pedal vs controller's throttle
*/
	// SOFT_FAULT_REMEDIED
	if(pedal_difference <= PEDAL1_TEN_PERCENT &&
		pedal1_value < PEDAL_MIN_MARGIN &&
		pedal1_value >= PEDAL1_DROPOUT && pedal2_value >= PEDAL2_DROPOUT)
			*events |= (1<<SOFT_FAULT_REMEDIED);

	// HARD_FAULT_SIG
	uint8_t portc_triggers =
		!(PINC&(1<<0) && PINC&(1<<1) && PINC&(1<<2) && PINC&(1<<3));
	uint8_t portd_triggers = !(PIND&(1<<7));
	uint8_t porte_triggers =
		!(PINE&(1<<0) && PINE&(1<<1) && PINE&(1<<6) && PINE&(1<<7));

	if(portc_triggers || portd_triggers || porte_triggers)
		*events |= (1<<HARD_FAULT_SIG);

	// GLV Voltage Low
/*	
	if(!(PINF&(1<<0)))
		glv_sys_voltage_low = 1;
	else
		glv_sys_voltage_low = 0;
	*/	
} // get_inputs()



void action_startup()
{
	PORTD |= (1<<1);	// ON Dash/TQV
	PORTB |= (1<<1);	// ON PEE Power
	PORTB |= (1<<2);	// ON ACUES POWER
	set_led(WHITE);
} // action_startup()



void action_neutral()
{
	PORTB &= ~(1<<7);	// OFF Charge Enable
	PORTB |= (1<<5);	// ON AIR 2
	PORTB |= (1<<6);	// ON AIR 1
	PORTB |= (1<<4);	// ON Keyswitch Enable
	PORTB &= ~(1<<3);	// OFF Drive Enable
	set_led(BLUE);
} // action_neutral()



void action_precharge()
{
	precharge_now = 1;
	PORTB &= ~(1<<4);	// OFF Keyswitch Enable
	PORTB |= (1<<7);	// ON Charge Enable
	PORTB &= ~(1<<0);	// OFF Precharge
} // action_precharge()



void action_charging()
{
	PORTB |= (1<<0);	// ON Precharge
	set_led(CYAN);
} // action_charging()



void action_drive()
{
	buzz_now = 1;
	PORTB |= (1<<3);	// ON Drive Enable
	set_led(GREEN);
} // action_drive()



void action_soft_fault()
{
	fault_now = 1;
	PORTB &= ~(1<<3);	// OFF Drive Enable
	set_led(YELLOW);
} // action_soft_fault()



void action_hard_fault()
{
	fault_now = 1;
	PORTB &= ~(1<<7);	// OFF Charge Enable
	PORTD &= ~(1<<1);	// OFF Dash/TQV
	PORTB &= ~(1<<5);	// OFF AIR 1
	PORTB &= ~(1<<6);	// OFF AIR 2
	PORTB &= ~(1<<4);	// OFF Keyswitch Enable
	PORTB &= ~(1<<3);	// OFF Drive Enable
	set_led(RED);

	for(;;); // HOLD PROGRAM
} // action_hard_fault()



void action_fatal()
{
	set_led(MAGENTA);
} // action_fatal()



void reset_drive_sound()
{
	buzz_now = 0;
	buzzer_time = BUZZER_MAX;
} // reset_drive_sound()



void reset_precharge_timer()
{
	precharge_now = 0;
	precharge_time = PRECHARGE_MAX;
	precharging_done = 0;
} // reset_precharge_timer()



void reset_fault_sound()
{
	fault_now = 0;
	fault_time = FAULT_MAX;
} // reset_fault_sound()



void set_led(uint8_t color)
{
	if(glv_sys_voltage_low)
		PORTC = (MAGENTA << 4) | (PORTC & LED_MASK);
	else
		PORTC = (color << 4) | (PORTC & LED_MASK);
} // set_led()

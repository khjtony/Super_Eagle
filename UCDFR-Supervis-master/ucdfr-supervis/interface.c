#include "interface.h"



void interface_init()
{
	usb_init();
} // interface_init()



//send a string from the Teensy to PC
void send_str(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}


uint8_t recv_str(char *buf, uint8_t size)
{
	int16_t r;
	uint8_t count=0;

	while (count < size) {
		r = usb_serial_getchar();
		if (r != -1) {
			if (r == '\r' || r == '\n') return count;
			if (r >= ' ' && r <= '~') {
				*buf++ = r;
				usb_serial_putchar(r);
				count++;
			}
		} else {
			if (!usb_configured() ||
			  !(usb_serial_get_control() & USB_SERIAL_DTR)) {
				// user no longer connected
				return 255;
			}
			// just a normal timeout, keep waiting
		}
	}
	return count;
}
//function call from main, 
void usb_terminal(uint16_t *events, uint8_t *state)
{
	if (usb_configured() &
	    usb_serial_get_control() &
	    USB_SERIAL_DTR){ 	//only initiate usage of serial terminal if  
							//PC is connected and ready to receive.
		
		usb_serial_flush_input(); //discard anything received prior
		get_inputs(events); //get snapshot of events
		
		send_str(PSTR("\r\nSupervisory Computer Serial"
					  "Terminal Interface\r\n\r\n"
					  "state : print current vehicle state\r\n"
					  "event=HV_UP : set event to HV_UP\r\n"));
		
	}
}

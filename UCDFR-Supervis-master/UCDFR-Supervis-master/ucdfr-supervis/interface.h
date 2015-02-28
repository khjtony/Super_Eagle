#ifndef INTERFACE_H
#define INTERFACE_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdint.h>

#include "usb_serial.h"
#include "hardware.h"

void interface_init();
void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void usb_terminal(uint16_t *events, uint8_t *state);


#endif

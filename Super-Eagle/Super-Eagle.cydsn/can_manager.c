#include "can_manager.h"

volatile uint8_t can_buffer[8];


/* Data Frame format for Voltage and Temperature
The datatype consists of three bytes:
1. Identifying Number (eg cell #1)
2. upper byte of data
3. lower byte of data
*/



                    

                    
uint8_t can_test_send()
{
    uint16_t ID=0x205;
    uint8_t msg[8];
    //msg[0~3] is left
    msg[0]=0x0f;
    msg[1]=0xf0;
    msg[2]=0x0f;
    msg[3]=0xf0;
    //msg[4~7] is right
    msg[4]=0x9a;
    msg[5]=0xbc;
    msg[6]=0xde;
    msg[7]=0xff;
    
    
    
	uint8_t i, state;
	CAN_1_TX_MSG message;
	CAN_1_DATA_BYTES_MSG payload;

	message.id = ID; // edit for testing
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 0x08;
	message.irq = 1;
	message.msg = &payload;

	for(i=0;i<8;i++)
		payload.byte[i] = msg[i];

	state = CAN_1_SendMsg(&message);

/*
	if(state != CYRET_SUCCESS)
	{
		LED_Write(1);
		return CAN_1_FAIL;
	}

	LED_Write(0);
	return CYRET_SUCCESS;
*/
	
	return state;
} // can_test_send()


void can_init()
{
	CAN_1_GlobalIntEnable(); // CAN Initialization
	CAN_1_Init();
	CAN_1_Start();
} // can_init()
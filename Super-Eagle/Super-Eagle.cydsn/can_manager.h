#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include "data.h"


typedef enum{
    NO_ERROR=0x0000,
    CHARGEMODE=0x0001,
    PACK_TEMP_OVER=0x0002,
    PACK_TEMP_CLOSE=0x0004,
    PACK_TEMP_UNDER=0x0008,
    LOW_SOC = 0x0010,
    CRITICAL_SOC = 0x0020,
    IMBALANCE = 0x0040,
    COM_FAILURE = 0x0080,
    NEG_CONT_CLOSED = 0x0100,
    POS_CONT_CLOSED = 0x0200,
    ISO_FAULT = 0x0400,
    CELL_VOLT_OVER = 0x0800,
    CELL_VOLT_UNDER = 0x1000,
    CHARGE_HAULT = 0x2000,
    FULL = 0x4000,
    PRECHARGE_CLOSED = 0x8000
}BMS_STATUS;

    
    
uint8_t can_test_send();
void can_init();

#endif // CAN_MANAGER_H

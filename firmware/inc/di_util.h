#ifndef _UTIL_H_
#define _UTIL_H_

#include "stdint.h"
#include "stdbool.h"
#include "types.h"



//ignores heartbeat inputs and sets their values to 0.
HEARTBEAT_DATA *ignore_heartbeat_data(){}

//
HEARTBEAT_DATA *process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *heartbeat_data, uint32_t msTicks){}

#endif

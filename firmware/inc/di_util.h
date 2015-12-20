#ifndef _UTIL_H_
#define _UTIL_H_

#include "stdint.h"
#include "stdbool.h"
#include "types.h"

// Returns heartbeat state object with values to 0.
HEARTBEAT_DATA *initialize_heartbeat_data(void);

// Process input messages to return updated heartbeat message state
HEARTBEAT_DATA *process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *heartbeat_data, uint32_t msTicks);

// Converts the (requested) state of input accessories to a request for the corresponding hardware accessories state
ACCESSORIES_OUTPUT_REQUEST_T *convert_acc(ACCESSORIES_INPUT_STATE_T *acc_in);

// Creates a hardware accessories request that turns off all accessories
ACCESSORIES_OUTPUT_REQUEST_T *turn_all_off(void);
#endif

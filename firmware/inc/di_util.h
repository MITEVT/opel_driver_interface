#ifndef _UTIL_H_
#define _UTIL_H_

#include "stdint.h"
#include "stdbool.h"
#include "types.h"

// Returns heartbeat state object with values to 0.
void initialize_heartbeat_data(HEARTBEAT_DATA *hb_data);

// Process input messages to return updated heartbeat message state
void process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *hb_data, uint32_t msTicks);

// Converts the (requested) state of input accessories to a request for the corresponding hardware accessories state
void convert_acc(ACCESSORIES_INPUT_STATE *acc_in, bool brakes_on, ACCESSORIES_OUTPUT_REQUEST *out_req);

// Creates a hardware accessories request that turns off all accessories
void turn_all_off(ACCESSORIES_OUTPUT_REQUEST *out_req);
#endif

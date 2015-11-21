#include <stdio.h>
#include "types.h"

ACCESSORIES_OUTPUT_REQUEST_T *convert_acc(ACCESSORIES_INPUT_STATE_T *acc_in) {
    return NULL;
}

HEARTBEAT_DATA *process_input_message(INPUT_MESSAGES *input_messages) {
    return NULL;
}

ERROR_T AccStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){

    switch(mode_request) {
        case REQ_OFF:
            return ERROR_ILLEGAL_STATE_REQUEST;

        case REQ_ACCESSORIES: {
            // Process input: convert acc input requested to acc output request
            ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
            // Process input: convert received heartbeat to update last received 
            HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);
            //Update output
            output->close_contactors = false;
            output->acc_output = acc_out;
            OUTPUT_MESSAGES out_messages = {.test = false, .command_shutdown = false, .send_heartbeat = false};
            output->messages = &out_messages;

            //Update state
            state->heartbeat_data = heartbeat_data;
            state->DSM_modes = MODE_ACCESSORIES;
	        return ERROR_NONE;

        } case REQ_DRIVE: {
            // Process input: convert acc input requested to acc output request
            ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
            // Process input: convert received heartbeat to update last received 
            HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);

            //Update output
            output->close_contactors = true;
            output->acc_output = acc_out;
            OUTPUT_MESSAGES out_messages = {.test = false, .command_shutdown = false, .send_heartbeat = true};
            output->messages = &out_messages;

            //Update state
            state->heartbeat_data = heartbeat_data;
            state->DSM_modes = MODE_DRIVE;
	        return ERROR_NONE;

        } case REQ_INIT:
            return ERROR_ILLEGAL_STATE_REQUEST;

        case REQ_SHUTDOWN:
            break;

        case REQ_CHARGE:
            break;

        case REQ_NONE:
	        return ERROR_NONE;
    }
	return ERROR_NONE;
}

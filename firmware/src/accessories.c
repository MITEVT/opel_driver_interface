#include <stdio.h> 
#include "types.h"
#include "util.h"

ACCESSORIES_OUTPUT_REQUEST_T *convert_acc(ACCESSORIES_INPUT_STATE_T *acc_in) {
    return NULL;
}

ACCESSORIES_OUTPUT_REQUEST_T *turn_all_off() {
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
            output->close_contactors = true;
            output->acc_output = acc_out;
            //TODO: should send heartbeat when not driving, but still want low power line?
            OUTPUT_MESSAGES out_messages = {.test = false, .command_shutdown = false, .send_heartbeat = false};
            output->messages = &out_messages;

            //Update state
            //TODO: update state mode here
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

        case REQ_SHUTDOWN: {
            ACCESSORIES_OUTPUT_REQUEST_T *acc_out = turn_all_off();
            HEARTBEAT_DATA *heartbeat_data = reset_heartbeat_data();

            //Update output
            output->close_contactors = false;
            output->acc_output = acc_out;
            OUTPUT_MESSAGES out_messages = {.test = false, .command_shutdown = true, .send_heartbeat = false};
            output->messages = &out_messages;

            //Update state
            state->heartbeat_data = heartbeat_data;
            state->DSM_modes = MODE_SHUTDOWN;
	        return ERROR_NONE;

        } case REQ_CHARGE:
            // Process input: convert acc input requested to acc output request
            ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
            // Process input: convert received heartbeat to update last received 
            HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);

            //Update output
            output->close_contactors = true;
            output->acc_output = acc_out;
            OUTPUT_MESSAGES out_messages = {.test = false, .command_shutdown = false, .send_heartbeat = false};
            output->messages = &out_messages;

            //Update state
            state->heartbeat_data = heartbeat_data;
            state->DSM_modes = MODE_CHARGE;
	        return ERROR_NONE;
    }
	return ERROR_NONE;
}

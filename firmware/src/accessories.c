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
            break;
        case REQ_ACCESSORIES: {
            //Process input
            ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
            HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);
            //Update output
            output->close_contactors = false;
            output->acc_output = acc_out;
            OUTPUT_MESSAGES out_messages = {.req_to_test = false, .req_to_shutdown = false, .req_to_send_heartbeat = false};
            output->messages = &out_messages;

            //Update state
            state->heartbeat_data = heartbeat_data;
            state->DSM_modes = MODE_ACCESSORIES;
	        return ERROR_NONE;
            break;
        } case REQ_DRIVE:
            break;
        case REQ_INIT:
            break;
        case REQ_SHUTDOWN:
            break;
        case REQ_CHARGE:
            break;
        case REQ_NONE:
            break;
    }



	return ERROR_NONE;
}

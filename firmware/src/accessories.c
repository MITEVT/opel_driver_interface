#include <stdio.h> 
#include "types.h"
#include "util.h"

ACCESSORIES_OUTPUT_REQUEST_T *convert_acc(ACCESSORIES_INPUT_STATE_T *acc_in) {
    ACCESSORIES_OUTPUT_REQUEST_T out_req;
    out_req.wipers_on = acc_in->wipers_on;
    out_req.brake_lights_on = acc_in->brake_lights_on;
    out_req.turn_blinker = acc_in->turn_blinker_switches;
    out_req.headlight_state = acc_in->headlight_switches;	
    return &out_req;
}

ACCESSORIES_OUTPUT_REQUEST_T *turn_all_off() {
    ACCESSORIES_OUTPUT_REQUEST_T out_req;
    out_req.wipers_on = false;
    out_req.brake_lights_on = false;
    out_req.turn_blinker = BLINKER_OFF;
    out_req.headlight_state = HEADLIGHT_OFF;
    return &out_req;
}

ERROR_T AccStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);

    if(mode_request == REQ_ACCESSORIES) {
        output->close_contactors = true;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        out_messages.drive_mode = MESSAGE_PARKED_AUX;
        output->messages = &out_messages;
        
        state->dsm_mode = MODE_ACCESSORIES;
	    return ERROR_NONE;

    } else if(mode_request == REQ_CHARGE) {
        output->close_contactors = true;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        out_messages.drive_mode = MESSAGE_CHARGE;
        output->messages = &out_messages;

        state->dsm_mode = MESSAGE_CHARGE;
	    return ERROR_NONE;

    } else if(mode_request == REQ_DRIVE) {
        output->close_contactors = true;
        output->acc_output = acc_out;

        if(input->direction == DRIVE_FORWARD) {
            out_messages.drive_mode = MESSAGE_DRIVE_FORWARD;
        } else {
            out_messages.drive_mode = MESSAGE_DRIVE_REVERSE;
        }

        OUTPUT_MESSAGES out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        output->messages = &out_messages;

        state->dsm_mode = MESSAGE_CHARGE;
	    return ERROR_NONE;

    } else if(mode_request == REQ_SHUTDOWN) {
        output->close_contactors = true;
        output->acc_output = turn_all_off();

        OUTPUT_MESSAGES out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        output->messages = &out_messages;

        state->dsm_mode = MESSAGE_CHARGE;
	    return ERROR_NONE;


    } else if(mode_request == REQ_OFF) {
        return ERROR_ILLEGAL_STATE_REQUEST;

    } else if(mode_request == REQ_INIT) {
        return ERROR_ILLEGAL_STATE_REQUEST;
    } 
}

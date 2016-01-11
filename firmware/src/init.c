#include "board.h"
#include <stdio.h> 
#include "types.h"
#include "di_util.h"
#include <stdbool.h>

    //TODO: returns true if we failed our tests
bool checkTestValidity(INPUT_T *input, STATE_T *state){
    if(input->messages->init_test){
	return false;
    }else if(){ //condition that determines if we failed our tests.
        return true;
    }else{
        return false;
    }
}

ERROR_T Init_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
    passedInitTests = input->messages->init_test;
    // TODO Change the below case details to correspond to init state
    //      (everything currently copied from Accessories SM)
    //      most important thing: need to run tests
    
    //chose 2 seconds arbitrarily as the time limit

    if(checkTestValidity(input, state)){
        return ERROR_TESTS_FAILED;
    }

    if(mode_request == REQ_INIT) {
        output->close_contactors = false;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = true;
        out_messages.send_heartbeat = false;
        out_messages.drive_mode = MESSAGE_PARKED_AUX;
        output->messages = &out_messages;
        
	state->testing_mode = INIT_TESTING;
        state->dsm_mode = MODE_INIT;
	    return ERROR_NONE;

    } else if(mode_request == REQ_CHARGE) {
        output->close_contactors = passedInitTests;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = !passedInitTests;
        out_messages.send_heartbeat = passedInitTests;
        out_messages.drive_mode = MESSAGE_CHARGE;
        output->messages = &out_messages;
	
        state->dsm_mode = (passedInitTests) ? MODE_CHARGE : MODE_INIT;
	    return ERROR_NONE;

    } else if(mode_request == REQ_DRIVE) {
        output->close_contactors = passedInitTests;
        output->acc_output = acc_out;

        if(input->direction == DRIVE_FORWARD) {
            out_messages.drive_mode = MESSAGE_DRIVE_FORWARD;
        } else {
            out_messages.drive_mode = MESSAGE_DRIVE_REVERSE;
        }

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = !passedInitTests;
        out_messages.send_heartbeat = passedInitTests;
        output->messages = &out_messages;

        state->dsm_mode = (passedInitTests) ? MODE_DRIVE : MODE_INIT;
	    return ERROR_NONE;

    } else if(mode_request == REQ_ACCESSORIES) {
        output->close_contactors = passedInitTests;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = !passedInitTests;
        out_messages.send_heartbeat = passedInitTests;
        out_messages.drive_mode = MESSAGE_PARKED_AUX;
        output->messages = &out_messages;

        state->dsm_mode = (passedInitTests) ? MODE_ACCESSORIES : MODE_INIT;
	    return ERROR_NONE;

    } else if(mode_request == REQ_OFF) {
	output->close_contactors = false;
        output->acc_output = turn_all_off();

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = false;
        out_messages.drive_mode = MESSAGE_PARKED_AUX;
        output->messages = &out_messages;

        state->dsm_mode = MODE_OFF;
	    return ERROR_NONE;


    } else if(mode_request == REQ_SHUTDOWN) {
        return ERROR_ILLEGAL_STATE_REQUEST;

    } else {
        return ERROR_ILLEGAL_STATE_REQUEST;
    }
}

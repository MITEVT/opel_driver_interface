#include "board.h"
#include <stdio.h> 
#include "types.h"
#include "di_util.h"
#include <stdbool.h>

    //returns true if we timed out on our tests (could possibly be simplified
    //to look much cleaner).
bool checkTimeout(INPUT_T *input, STATE_T *state){
    if(input->messages->init_test){
	return false;
    }else if(state->testing_mode == INIT_TESTING){
        if(state->counter == 0){
	    state->time_since_test_start = msTicks;
	    state->counter++;
	    return false;
	//arbirarily chose 5 seconds as the timeout time.
        }else if(msTicks - state->time_since_test_start > 5000){
	    return true;
  	}else{
	    return false
	}
    }else{
        return false;
    }
}

ERROR_T Init_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
    passedInitTests = input->messages->init_test;

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

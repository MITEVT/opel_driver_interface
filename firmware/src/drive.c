#include <stdio.h> 
#include "types.h"
#include "di_util.h"

ERROR_T Drive_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
    DRIVE_DIRECTION_T dir = input->direction;
    uint16_t vel = input->messages->velocity;
    // TODO Change the below case details to correspond to drive state
    //      (everything currently copied from Accessories SM)


    //shouldn't this be untouched? when it comes to the DI, everything in 
    //we can do in accessories should also be able to be done in drive.

    if(mode_request == REQ_ACCESSORIES) {
        output->close_contactors = true;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        out_messages.drive_mode = MESSAGE_PARKED_AUX;
        output->messages = &out_messages;
        
        state->dsm_mode = MODE_ACCESSORIES;
	    return ERROR_NONE;

    } else if(mode_request == REQ_CHARGE) {
        output->close_contactors = true;
        output->acc_output = acc_out;

        OUTPUT_MESSAGES_T out_messages;
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

	    if(state->direction != DRIVE_FORWARD){
		if(vel == 0){
		    out_messages.drive_mode = MESSAGE_DRIVE_FORWARD;
		    state->direction = DRIVE_FORWARD;
		}else{
		    return ERROR_VELOCITY_NOT_ZERO;
		}
	    }else{
 		out_messages.drive_mode = MESSAGE_DRIVE_FORWARD;
		state->direction = DRIVE_FORWARD;	    
	    }
           
        } else {
 
	    if(state->direction != DRIVE_REVERSE){
		if(vel == 0){
		    out_messages.drive_mode = MESSAGE_DRIVE_REVERSE;
		    state->direction = DRIVE_REVERSE;
		}else{
		    return ERROR_VELOCITY_NOT_ZERO;
		}
	    }else{
 		out_messages.drive_mode = MESSAGE_DRIVE_REVERSE;
		state->direction = DRIVE_REVERSE;	    
	    }
           
           
        }

        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;
        output->messages = &out_messages;

        state->dsm_mode = MODE_DRIVE;
	    return ERROR_NONE;

    } else if(mode_request == REQ_SHUTDOWN) {

        output->close_contactors = true;
        output->acc_output = (vel == 0) ? turn_all_off() : acc_out;
		
        OUTPUT_MESSAGES_T out_messages;
        out_messages.test = false;
        out_messages.send_heartbeat = true;

	MESSAGE_DRIVE_MODE_T current_drive_mode = (vel > 0) ? MESSAGE_DRIVE_FORWARD : MESSAGE_DRIVE_REVERSE;
        out_messages.drive_mode = (vel == 0) ? MESSAGE_SHUTDOWN_IMPENDING : current_drive_mode;
        output->messages = &out_messages;

        state->dsm_mode = (vel == 0) ? MODE_SHUTDOWN : MODE_DRIVE;
	    return ERROR_NONE;

    } else if(mode_request == REQ_OFF) {
        return ERROR_ILLEGAL_STATE_REQUEST;

    } else if(mode_request == REQ_INIT) {
        return ERROR_ILLEGAL_STATE_REQUEST;

    } else {
        return ERROR_ILLEGAL_STATE_REQUEST;
    }
}

#include "types.h"
#include "shutdown.h"
#include "init.h"
#include "accessories.h"

ERROR_T check_heartbeat_validity(INPUT_T *input, STATE_T *state) {
    // Check that the heartbeats in *state aren't stale
    return NULL;
}

void DSM_Init(void){
    // Return init state struct
    return NULL;
}

ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output){

    HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);
    state->heartbeat_data = heartbeat_data;
    ERROR_T error = check_heartbeat_validity(input);
    if(error != ERROR_NONE) {
        return error;
    }

    MODE_T mode = state->dsm_mode;
    keymode_req = input->keymodes;

	if(keymode_req == KEYMODE_ACCESSORIES) {
		if(mode != MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_ACCESSORIES);
		
        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_ACCESSORIES);				
        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_ACCESSORIES);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_ACCESSORIES);

        } else if(mode == MODE_OFF || mode == MODE_INIT) {
            return InitStep(input, output, state, REQ_ACCESSORIES);

        } else if(mode == MODE_FAIL) {
            return FailStep(input, output, state, REQ_OFF);	
        } 

	} else if(keymode_req == KEYMODE_CHARGE) {
		if(mode != MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_CHARGE);
		
        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_CHARGE);				

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_CHARGE);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_CHARGE);

        } else if(mode == MODE_OFF || mode == MODE_INIT) {
            return InitStep(input, output, state, REQ_CHARGE);

        } else if(mode == MODE_FAIL) {
            return FailStep(input, output, state, REQ_CHARGE);	
        } 

	} else if(keymode_req == KEYMODE_DRIVE) {
		if(mode != MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_DRIVE);
		
        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_DRIVE);				

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_DRIVE);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_DRIVE);

        } else if(mode == MODE_OFF || mode == MODE_INIT) {
            return InitStep(input, output, state, REQ_DRIVE);

        } else if(mode == MODE_FAIL) {
            return FailStep(input, output, state, REQ_DRIVE);	
        } 
    } else if(keymode_req == KEYMODE_OFF) {
        if(mode == MODE_ACCESSORIES) {
            return AccStep(input, output, state, REQ_SHUTDOWN);

        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_SHUTDOWN);

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_SHUTDOWN);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_OFF);

        } else if(mode == MODE_INIT) {
            return InitStep(input, output, state, REQ_SHUTDOWN);
		
        } else if(mode == MODE_OFF) {
			return ERROR_NONE;

        } else if(mode == MODE_FAIL) {
            return FailStep(input, output, state, REQ_OFF);
        }
    }
}


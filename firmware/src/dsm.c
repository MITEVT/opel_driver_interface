#include "types.h"
#include "shutdown.h"
#include "init.h"
#include "frequencies.h"
#include "di_util.h"
#include "board.h"

ERROR_T check_heartbeat_validity(STATE_T *state) {
    // Check that the heartbeats in *state aren't stale
    if(state->heartbeat_data->time_since_BMS_heartbeat > 1000.0/BMS_heartbeat_frequency){
        return ERROR_LOST_HEARTBEAT;
    }else if(state->heartbeat_data->time_since_throttle_heartbeat > 1000.0/throttle_heartbeat_frequency){
        return ERROR_LOST_HEARTBEAT;
    }else if(state->heartbeat_data->time_since_PDM_heartbeat > 1000.0/PDM_heartbeat_frequency){
        return ERROR_LOST_HEARTBEAT;
    }else if(state->heartbeat_data->time_since_velocity_heartbeat > 1000.0/velocity_heartbeat_frequency){
	return ERROR_LOST_HEARTBEAT;
    }else{
	return ERROR_NONE;
    }
}

void DSM_Init(void){
    // Return init state struct
    return NULL;
}

ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output){
    
    //Process the input heartbeats, then check to see if the heartbeats are stale.
    state->heartbeat_data = process_input_message(input->messages,state->heartbeat_data,msTicks);
    ERROR_T error = check_heartbeat_validity(state);
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


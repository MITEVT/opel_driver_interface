#include "board.h"

#include "types.h"
#include "di_util.h"
#include "frequencies.h"

#include "accessories.h"
#include "drive.h"
#include "charge.h"
#include "shutdown.h"
#include "init.h"

ERROR_T check_heartbeat_validity(STATE_T *state) {
    // Check that the heartbeats in *state aren't stale
    uint32_t heartbeat_time = state->heartbeat_data->time_since_BMS_heartbeat;
    uint32_t throttle_time = state->heartbeat_data->time_since_BMS_heartbeat;
    uint32_t pdm_time = state->heartbeat_data->time_since_BMS_heartbeat;
    uint32_t velocity_time = state->heartbeat_data->time_since_BMS_heartbeat;

    if(heartbeat_time > 1000.0/BMS_HEARTBEAT_FREQUENCY) {
        return ERROR_LOST_BMS_HEARTBEAT;

    } else if(throttle_time > 1000.0/THROTTLE_HEARTBEAT_FREQUENCY) {
        return ERROR_LOST_THROTTLE_HEARTBEAT;

    } else if(pdm_time > 1000.0/PDM_HEARTBEAT_FREQUENCY) {
        return ERROR_LOST_PDM_HEARTBEAT;

    } else if(velocity_time > 1000.0/VELOCITY_HEARTBEAT_FREQUENCY){
	    return ERROR_LOST_VELOCITY_HEARTBEAT;
    }
	return ERROR_NONE;
}

STATE_T *DSM_Init(void){
    STATE_T init_state;
    init_state.dsm_mode = MODE_OFF;
    init_state.heartbeat_data = initialize_heartbeat_data();
    init_state.testing_mode = INIT_OFF;
    init_state.time_since_test_start = NULL;
    init_state.counter = 0;
    return &init_state;
}

ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output){

    // Process input heartbeats, then check to see if the heartbeats are stale
    HEARTBEAT_DATA *heartbeat_data = process_input_message(input->messages);
    state->heartbeat_data = heartbeat_data;
    ERROR_T error = check_heartbeat_validity(input);
    if(error != ERROR_NONE) {
        return error
    }

    MODE_T mode = state->dsm_mode;
    keymode_req = input->keymodes;

	if(keymode_req == KEYMODE_ACCESSORIES) {
		if(mode == MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_ACCESSORIES);
		
        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_ACCESSORIES);				

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_ACCESSORIES);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_OFF);

        } else if(mode == MODE_OFF) {
            return InitStep(input, output, state, REQ_INIT);

        } else(mode == MODE_INIT){
	    return InitStep(input, output, state, REQ_ACCESSORIES);
	}

	} else if(keymode_req == KEYMODE_CHARGE) {
		if(mode == MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_CHARGE);
		
       	 } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_CHARGE);				

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_CHARGE);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_OFF);

        } else if(mode == MODE_OFF) {
            return InitStep(input, output, state, REQ_INIT);

        } else(mode == MODE_INIT){
	    return InitStep(input, output, state, REQ_CHARGE)
	}

	} else if(keymode_req == KEYMODE_DRIVE) {
		if(mode != MODE_ACCESSORIES) {
			return AccStep(input, output, state, REQ_DRIVE);
		
        } else if(mode == MODE_CHARGE) {
            return ChargeStep(input, output, state, REQ_DRIVE);				

        } else if(mode == MODE_DRIVE) {
            return DriveStep(input, output, state, REQ_DRIVE);

        } else if(mode == MODE_SHUTDOWN) {
            return ShutdownStep(input, output, state, REQ_OFF);

        } else if(mode == MODE_OFF) {
            return InitStep(input, output, state, REQ_INIT);

        } else(mode == MODE_INIT) {
            return InitStep(input, output, state, REQ_DRIVE);	
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
            return InitStep(input, output, state, REQ_OFF);
		
        } else(mode == MODE_OFF) {
   	    return ERROR_NONE;
        }
    }
}


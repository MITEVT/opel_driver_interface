#include <stdio.h> 
#include "types.h"
#include "di_util.h"

ERROR_T Shutdown_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
        if(mode_request == REQ_OFF) {
	INPUT_MESSAGES inp_messages = input->messages;
	
	bool BMS_hb = inp_messages->BMS_heartbeat;
	bool PDM_hb = inp_messages->PDM_heartbeat;
	bool throttle_hb = inp_messages->throttle_heartbeat;
	uint16_t vel  = inp_messages->velocity;
 	
	//returns error when velocity is not zero & shutdown is attempted
	if(vel != 0){
	    return ERROR_VELOCITY_NOT_ZERO;
	}
	
	//follow the shutdown sequence
	output->close_contactors = false;
	output->acc_output = turn_all_off();

	OUTPUT_MESSAGES_T out_messages;
	out_messages.test = false;
	out_messages.send_heartbeat = false;
	out_messages.drive_mode = (!BMS_hb && !PDM_hb && !throttle_hb) ? MESSAGE_PARKED_AUX : MESSAGE_SHUTDOWN_IMPENDING;
	output->messages = &out_messages;
	
	//If adding FAIL state mode, then add a possibility of entering the
	//fail state mode here.
	state->dsm_mode = (!BMS_hb && !PDM_hb && !throttle_hb) ? MODE_OFF : MODE_SHUTDOWN;
	return ERROR_NONE;
    }else{
	//shouldn't get a request other than to turn off the car.
        return ERROR_ILLEGAL_STATE_REQUEST;
    }
}

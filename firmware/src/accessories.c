#include "types.h"

ERROR_T AccStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
	ACCESSORIES_INPUT_STATE_T inp = input->acc_input;
	ACCESSORY_OUTPUT_REQUEST_T out_req = output->acc_output;	

	//headlights
	HEADLIGHT_SWITCH_STATE hlswitches = inp->headlight_switches;
	//switch1 low switch2 low
	if(!hlswitches->switch1 && !hlswitches->switch2){
		out_req->headlight_state = HEADLIGHT_OFF;
	//switch1 high switch2 low 	
	}else if(hlswitches->switch1 && !hlswitches->switch2){
		out_req->headlight_state = HEADLIGHT_ON;
	//switch1 high switch2 low
	}else if(hlswitches->switch1 && hlswitches->switch2){
		out_req->headlight_state = HIGHBEAM_ON;
	//unused
	}else{
		out_req->headlight_state = HEADLIGHT_OFF;
	}

	//blinkers
	BLINKER_SWITCH_STATE blswitches = inp->turn_blinker_switches;
	//switch1 low switch2 low
	if(!blswitches->switch1 && !blswitches->switch2){
		out_req->turn_blinker = BLINKER_OFF;
	//switch1 high switch2 low
	}else if(blswitches->switch1 && !blswitches->switch2){
		out_req->turn_blinker = LEFT_BLINKER;
	//switch1 low switch2 high
	}else if(!blswitches->switch1 && blswitches->switch2){
		out_req->turn_blinker = RIGHT_BLINKER;
	//unused
	}else{
		out_req->turn_blinker = BLINKER_OFF;
	}

	return ERROR_NONE;
}

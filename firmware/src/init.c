#include "init.h"

INIT_MODES_T init_mode;

// TODO: keep init_done state (maybe ^?) and respond to both curr MODE_OFF -> other MODE and curr MODE_INIT -> other MODE

INIT_Init(void){
	init_mode = INIT_OFF;
}

ERROR_T InitStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_req)}
	switch(mode_req){
		case REQ_OFF:
			init_mode = INIT_OFF;
			break;

		case REQ_INIT:
			init_mode = INIT_TESTING;
			output->messages->req_to_test = true;
			break;	

		default:
			init_mode = INIT_OFF;
			break;
				
	}	
}

INIT_MODES_T Init_GetMode(void){
	return init_mode;
}

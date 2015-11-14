#include "types.h"

MODES_T mode;

void DSM_Init(void){
	mode = OFF;
}


ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output, MODE_REQUEST_T mode_request){
	if(mode_request == REQ_OFF){
		if(mode != OFF){
			//Move to OFF
			if(mode == ACCESSORIES){
				mode = SHUTDOWN;
				return AccStep(input,output);
			}else if(mode == CHARGE){
				mode = SHUTDOWN;
				return ERROR_NONE;
				
			}
		}
	}
		

}


}

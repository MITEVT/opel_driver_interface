#include "types.h"

MODES_T mode;

void DSM_Init(void){
                    //Check Skanda's edit of types.h
	mode = MODE_OFF;
}


ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output){
	if(input->keymodes == KEYMODE_OFF){
		if(mode != MODE_OFF){
			//Move to OFF
			if(mode == MODE_ACCESSORIES){
				mode = MODE_SHUTDOWN;
				return AccStep(input, output, state, REQ_SHUTDOWN);
			}else if(mode == MODE_CHARGE){
				mode = MODE_SHUTDOWN;
				return ChargeStep(input, output, state, REQ_SHUTDOWN);
			}else if(mode == MODE_DRIVE){
				mode = MODE_SHUTDOWN;
				return DriveStep(input, output, state, REQ_SHUTDOWN);
			}else if(mode == MODE_INIT){
				mode = MODE_OFF;
				return ERROR_NONE;
			}else if(mode == MODE_SHUTDOWN){
				mode = MODE_OFF;
				return ShutDownStep(input, output, state, REQ_OFF);
			}else{
				mode = MODE_OFF;
				return FailStep(input, output, state, REQ_OFF);
			}
		}
	}else if(input->keymodes == KEYMODE_ACCESSORIES)
		

}


}

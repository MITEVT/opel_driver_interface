#include "types.h"
#include "shutdown.h"
#include "init.h"


//MAKE SURE TO CHECK BMS HEARTBEAT

MODES_T mode;

void DSM_Init(void){
                    //Check Skanda's edit of types.h
	mode = MODE_OFF;
}


ERROR_T DSM_Step(INPUT_T *input, STATE_T *state, OUTPUT_T *output){
	//keyswitch is OFF
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
				return InitStep(input, output, state, REQ_OFF);

			}else if(mode == MODE_SHUTDOWN){
				if(Shutdown_GetMode() == SHUTDOWN_DONE){ //ShutDown OFF? or ShutDown Done?
					mode == MODE_OFF;
					return ERROR_NONE;
				}
				return ShutDownStep(input, output, state, REQ_OFF);

			}else{
				mode = MODE_OFF;
				return FailStep(input, output, state, REQ_OFF);

			}
		}else{
			return ERROR_NONE;
		}
	}else if(input->keymodes == KEYMODE_ACCESSORIES){
		if(mode != MODE_ACCESSORIES){
			if(mode == MODE_OFF){
				mode = MODE_INIT;
				return ERROR_NONE;

			}else if(mode == MODE_INIT){
				if(Init_GetMode() == INIT_DONE){
					mode = MODE_ACCESSORIES;
					return ERROR_NONE;
				}
				return InitStep(input, output, state, REQ_ACCESSORIES);

			}else if(mode == MODE_CHARGE){
				mode = MODE_ACCESSORIES;
				return ChargeStep(input, output, state, REQ_ACCESSORIES);				
			}else if(mode == MODE_DRIVE){
				mode = MODE_ACCESSORIES;
				return DriveStep(input, output, state, REQ_ACCESSORIES);

			}else if(mode == MODE_FAIL){
				return FailStep(input, output, state, REQ_OFF);	
			}
			else{
				return ERROR_INCOMPATIBLE_MODE; //need to add to types.h
			}
		}else{
			return AccStep(input, output, state, REQ_NONE);
		}
	}else if(input->keymodes == KEYMODE_DRIVE){
		if(mode != MODE_DRIVE){
			if(mode == MODE_OFF){
				mode = MODE_INIT;
				return ERROR_NONE; 
			}else if(mode == MODE_INIT){
				if(Init_GetMode() == INIT_DONE){
					mode = MODE_DRIVE;
					return ERROR_NONE
				}
			}
		}else{
			return DriveStep(input, output, state, REQ_NONE);
		}
	}
}


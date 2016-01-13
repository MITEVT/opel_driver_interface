#include <stdio.h> 
#include "types.h"
#include "di_util.h"

ERROR_T Shutdown_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request){
    
    ACCESSORIES_OUTPUT_REQUEST_T *acc_out = convert_acc(input->acc_input);
    // TODO Change the below case details to correspond to shutdown state
    //      (everything currently copied from Accessories SM)
    //      Make sure to return error when velocity is not zero and shutdown is attempted

    if(mode_request == REQ_OFF) {
        //follow the shutdown sequence
    }else{
	//shouldn't get a request other than to turn off the car.
        return ERROR_ILLEGAL_STATE_REQUEST;
    }
}

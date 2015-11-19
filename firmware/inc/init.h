#ifndef _INIT_H_
#define _INIT_H_
#include "types.h"

typedef enum {
	INIT_OFF = 0, //OFF Mode
	INIT_TESTING = 1, //Testing Mode
	INIT_DONE = 2, //Tests Done
} INIT_MODES_T;

ERROR_T InitStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_req);

INIT_MODES_T Init_GetMode(void);


#endif

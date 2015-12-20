#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "types.h"

ERROR_T Drive_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_req);

#endif

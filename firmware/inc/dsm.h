#ifndef _DSM_H_
#define _DSM_H_

#include "types.h"

STATE_T *DSM_Init(void);

ERROR_T DSM_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state);

#endif

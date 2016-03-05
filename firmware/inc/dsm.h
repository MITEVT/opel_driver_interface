/**
 * @file dsm.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 20 February 2016
 * @brief Defines large driver state machine
 */

#ifndef _DSM_H_
#define _DSM_H_

#include "chip.h"
#include "types.h"

typedef struct {} DSM_Config_T;

void DSM_Config(DSM_Config_T *DSM_config);

STATE *DSM_Init(void);

DI_ERROR DSM_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);

#endif

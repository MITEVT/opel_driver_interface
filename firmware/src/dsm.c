#include "types.h"
#include "di_util.h"
#include "dsm.h"
#include "chip.h"

void DSM_Config(DSM_Config_T *dsm_config) {}

STATE *DSM_Init(void) {}

DI_ERROR DSM_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    return ERROR_NONE;
}

#include "types.h"
#include "di_util.h"
#include "charge.h"
#include "chip.h"

void Charge_Config(Charge_Config_T *charge_config) {}

void Charge_Cleanup(STATE *state) {}

DI_ERROR check_brusa(STATE *state) {
    return 0;
}

DI_ERROR Charge_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    return 0;
}

#include "types.h"
#include "di_util.h"
#include "charge.h"
#include "chip.h"

void Charge_Config(Charge_Config_T *charge_config) {}

void Charge_Cleanup(STATE *state) {}

DI_ERROR check_brusa(STATE *state) {
    HEARTBEAT_DATA *hb_data = state->heartbeat_data;
    bool hardware_on = hb_data->brusa_status->hardware_on;
    bool overall_error = hb_data->brusa_status->overall_error_reported;
    bool specific_error = hb_data->brusa_status->specific_error_reported;
    if(!hardware_on) {
        return ERROR_BRUSA_NOT_ON;
    } else if(overall_error) {
        return ERROR_BRUSA_OVERALL_ERROR;
    } else if(specific_error) {
        return ERROR_BRUSA_SPECIFIC_ERROR;
    } else {
        return ERROR_NONE;
    }
}

DI_ERROR charge_checks(STATE *state) {
    HEARTBEAT_DATA *hb_data = state->heartbeat_data;
    if(check_velocity_zero(state)) {
        return ERROR_VELOCITY_NOT_ZERO;
    } else {
        return check_brusa(state);
    }
}

DI_ERROR Charge_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    DI_ERROR hb_error_existence = all_hb_exist(state, msTicks);
    if(hb_error_existence != ERROR_NONE) {
        return hb_error_existence;
    }

    DI_ERROR hb_error_content = no_heartbeat_errors(state, true);
    if (hb_error_content != ERROR_NONE) {
        return hb_error_content;
    }

    DI_ERROR charge_error = charge_checks(state);
    if(charge_error != ERROR_NONE) {
        // TODO If MODE_REQUEST IS NOT CHARGE, switch state
        // else return error
        return charge_error;
    }

    return ERROR_NONE;
}

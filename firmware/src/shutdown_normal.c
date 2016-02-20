#include "types.h"
#include "di_util.h"
#include "shutdown_normal.h"
#include "chip.h"

static uint32_t max_wt_module_shutdown_ms;
static uint32_t max_wt_velocity_check_ms;
static uint32_t max_wt_contactors_open_ms;

void Shutdown_Normal_Config(Shutdown_Normal_Config_T *shutdown_config) {
    max_wt_module_shutdown_ms = shutdown_config->max_wait_modules_shutdown_ms;
    max_wt_velocity_check_ms = shutdown_config->max_wait_velocity_check_ms;
    max_wt_contactors_open_ms = shutdown_config->max_wait_contactors_open_ms;
}

void Shutdown_Normal_Cleanup(STATE *state) {
    state->time_started_module_shutdown_ms = 0;
    state->time_started_velocity_zero_ms = 0;
    state->time_started_open_contactors_ms = 0;
}

DI_ERROR check_open_ctcr(STATE *state) {
   HEARTBEAT_DATA *hb_data = state->heartbeat_data;
   BMS_PRECHARGE_STATUS *s = hb_data->bms_precharge_status;
   if(s->contactor_output[0] || s->contactor_output[1] || s->contactor_output[2]) {
       return ERROR_OPEN_CONTACTOR;
   } else {
       return ERROR_NONE;
   }
}

DI_ERROR check_ctcr_state(STATE *state, OUTPUT *output, uint32_t msTicks) {
    uint32_t time_open_ctcr = state->time_started_open_contactors_ms;
    if(time_open_ctcr != 0) {
        // To check if PDM results are OK; tests all heartbeats too!
        DI_ERROR open_error = check_open_ctcr(state); 
        
        if(open_error == ERROR_NONE) {
            Shutdown_Normal_Cleanup(state);
            output->messages->shutdown_vel_zero_req = false;
            state->dsm_mode = MODE_OFF;
            output->messages->di_packet->mode = OUT_OFF;
            output->critical_systems_relay_on = false;
            output->low_voltage_relay_on = false;
            return ERROR_NONE;

        } else if (msTicks - time_open_ctcr > max_wt_contactors_open_ms) {
            return open_error;

        } else {
            return ERROR_NONE;

        }
    } else {
        output->messages->di_packet->ignition = DI_START;
        state->time_started_open_contactors_ms = msTicks;
        return ERROR_NONE;
    }
}

DI_ERROR no_shutdown_error(STATE *state) {
    bool ui_check = !state->heartbeat_data->ui_status->rasp_pi_on;
    bool mi_check = state->heartbeat_data->mi_status->shutdown_okay;
    if(ui_check) {
        return ERROR_NONE;
    } else if (mi_check) {
        return ERROR_MI_SHUTDOWN;
    } else {
        return ERROR_NONE;
    }
}

DI_ERROR shutdown_ctcr_check(STATE *state, OUTPUT *output, uint32_t msTicks) {
    uint32_t tsms = state->time_started_module_shutdown_ms;

    if(tsms != 0) {
        DI_ERROR module_shutdown_error = no_shutdown_error(state);
        if(module_shutdown_error == ERROR_NONE) {
            return check_ctcr_state(state, output, msTicks);

        } else if(msTicks - tsms > max_wt_module_shutdown_ms) {
            return module_shutdown_error;

        } else {
            return ERROR_NONE;
        }

    } else {
        state->time_started_module_shutdown_ms = msTicks;
        return ERROR_NONE;
    }
}

DI_ERROR no_velocity_error(STATE *state) {
    if(check_velocity_zero(state)) {
        uint32_t velocity1_rpm = state->heartbeat_data->wv1_status->velocity_rpm;
        uint32_t velocity2_rpm = state->heartbeat_data->wv2_status->velocity_rpm;
        DI_ERROR error = check_velocity_in_range(velocity1_rpm, velocity2_rpm);
        if(error == ERROR_NONE) {
            return check_velocity_diff(state);
        } else {
            return error;
        }
    } else {
        return ERROR_VELOCITY_NOT_ZERO;
    }
}

DI_ERROR Shutdown_Normal_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    uint32_t tests_started_time = state->time_started_velocity_zero_ms;

    if(tests_started_time != 0) {
        // We started shutdown procedure
        
        DI_ERROR velocity_error = no_velocity_error(state);
        if(velocity_error == ERROR_NONE) {
            return shutdown_ctcr_check(state, output, msTicks);

        } else if(msTicks - tests_started_time > max_wt_velocity_check_ms) {
            return velocity_error;

        } else {
            return ERROR_NONE;
        }

    } else {
        // We haven't started shutdown sequence
        output->messages->di_packet->mode = OUT_SHUTDOWN_IMPENDING;
        output->messages->shutdown_vel_zero_req = true;
        state->time_started_velocity_zero_ms = msTicks;
        return ERROR_NONE;
    }
}

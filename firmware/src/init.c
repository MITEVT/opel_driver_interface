#include "types.h"
#include "di_util.h"
#include "init.h"
#include "chip.h"

static uint32_t threshold_wait_time_heartbeats_ms;
static uint32_t threshold_wait_time_bms_ms;
static uint32_t threshold_wait_time_pdm_ms;

void Init_Config(Init_Config_T *init_config) {
    threshold_wait_time_heartbeats_ms = init_config->threshold_wait_time_heartbeats_ms;
    threshold_wait_time_bms_ms = init_config->threshold_wait_time_bms_ms;
    threshold_wait_time_pdm_ms = init_config->threshold_wait_time_pdm_ms;
}

void Init_Cleanup(STATE *state) {
    state->time_started_init_tests_ms = 0;
    state->time_started_close_contactors_request_ms = 0;
    state->time_started_PDM_tests_ms = 0;
}

DI_ERROR check_pdm(STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    if(time_started_PDM_tests_ms != 0) {
        // To check if PDM results are OK; tests all heartbeats too!
        DI_ERROR hb_content_error = no_heartbeat_errors(state, msTicks, false); 
        
        if(hb_content_error == ERROR_NONE) {
            change_mode(mode_request, state, output);
            return ERROR_NONE;
        } else if (msTicks - time_started_PDM_tests_ms > threshold_wait_time_pdm_ms) {
            return ERROR_PDM_TEST_TIMEOUT;
        } else {
            return ERROR_NONE:
        }
    } else {
        output->critical_systems_relay_on = true;
        time_started_PDM_tests_ms = msTicks;
        return ERROR_NONE;
    }
}

DI_ERROR check_precharge_and_pdm(STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    if(state->time_started_close_contactors_request_ms != 0) {
        // We started BMS request to precharge/close contactors

        DI_ERROR bms_precharge_status = check_bms_precharge(state);
        if(bms_precharge_status == ERROR_NONE) {
            return check_pdm(state, output, mode_request, msTicks);
        } else if(msTicks - time_started_close_contactors_request_ms > threshold_wait_time_bms_ms) {
            return ERROR_BMS_PRECHARGE_TIMEOUT;
        } else {
            return ERROR_NONE;
        }
    } else {
        // We did not yet start BMS request to precharge
        output->messages->di_packet->ignition = DI_START;
        time_started_close_contactors_request_ms = msTicks;
        return ERROR_NONE;
    }
}

// 1. check if that all heartbeats are there
// 2. check that there are no errors
// 3. get BMS precharge status and no BMS errors

DI_ERROR Init_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    if(state->time_started_init_tests_ms != 0) {
        // We started tests for heartbeat existence and content
        
        DI_ERROR hb_presence = all_hb_exist(state, msTicks);
        DI_ERROR hb_content_error = no_heartbeat_errors(state, msTicks, false);
        if((hb_presence == ERROR_NONE) && (hb_content_error == ERROR_NONE)) {
            return check_precharge_and_pdm(state, output, mode_request, msTicks);
        } else if((hb_presence == ERROR_NONE) && (hb_content_error != ERROR_NONE)) {
            return hb_content_error;
        } else if(msTicks - state->time_started_init_tests_ms > threshold_wait_time_heartbeats_ms) {
            return hb_presence;
        } else {
            return ERROR_NONE;
        }
    } else {
        // We haven't started tests for heartbeat existence
        
        output->low_voltage_relay_on = true;
        output->messages->di_packet->ignition = DI_RUN;
        output->messages->di_packet->mode = OUT_INIT;
        state->time_started_init_tests_ms = msTicks;
        return ERROR_NONE;
    }
}

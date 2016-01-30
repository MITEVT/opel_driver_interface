#include "types.h"
#include "chip.h"
#include "string.h"

// TODO: Add documentation for all functions

// TODO:
// write initialize_state()

/**
 * Initializes the heartbeat data struct to sensible start values
 * @param HEARTBEAT_DATA *hb_data pointer to an allocated heartbeat struct
 * @return void
 */
void initialize_heartbeat_data(HEARTBEAT_DATA *hb_data) { 
    hb_data->started_heartbeats->bms_heartbeat1=false;
    hb_data->started_heartbeats->bms_heartbeat2=false;
    hb_data->started_heartbeats->bms_heartbeat3=false;
    hb_data->started_heartbeats->throttle_heartbeat=false;
    hb_data->started_heartbeats->wv1_heartbeat=false;
    hb_data->started_heartbeats->wv2_heartbeat=false;
    hb_data->started_heartbeats->pdm_heartbeat=false;
    hb_data->started_heartbeats->ui_heartbeat=false;
    hb_data->started_heartbeats->mi_heartbeat=false;

    hb_data->last_rcvd_bms_heartbeat1 = 0;
    hb_data->last_rcvd_bms_heartbeat2 = 0;
    hb_data->last_rcvd_bms_heartbeat3 = 0;

    hb_data->last_rcvd_wv1_heartbeat = 0;
    hb_data->last_rcvd_wv2_heartbeat = 0;
    hb_data->last_rcvd_throttle_heartbeat = 0;
    hb_data->last_rcvd_ui_heartbeat = 0;
    hb_data->last_rcvd_mi_heartbeat = 0;
    hb_data->last_rcvd_pdm_heartbeat = 0;

    hb_data->wv1_status->velocity = 0;
    hb_data->wv2_status->velocity = 0;

    hb_data->pdm_status->low_voltage_status=false;
    hb_data->pdm_status->low_voltage_dcdc=false;
    hb_data->pdm_status->low_voltage_battery=false;
    hb_data->pdm_status->critical_systems_status=false;
    hb_data->pdm_status->critical_systems_battery=false;
    hb_data->pdm_status->critical_systems_dcdc=false;

    hb_data->throttle_status->brake_value=0;
    hb_data->throttle_status->throttle_value=0;

    hb_data->bms_precharge_status->contactor_error[0] = false;
    hb_data->bms_precharge_status->contactor_output[0] = false;
    hb_data->bms_precharge_status->contactor_error[1] = false;
    hb_data->bms_precharge_status->contactor_output[1] = false;
    hb_data->bms_precharge_status->contactor_error[2] = false;
    hb_data->bms_precharge_status->contactor_output[2] = false;
    hb_data->bms_precharge_status->precharge_status = 0;

    hb_data->bms_pack_status->cells_over_voltage = false;
    hb_data->bms_pack_status->cells_under_voltage = false;
    hb_data->bms_pack_status->cells_over_temperature = false;
    hb_data->bms_pack_status->measurement_untrusted = false;
    hb_data->bms_pack_status->cmu_comm_timeout = false;
    hb_data->bms_pack_status->vehicle_comm_timeout = false;
    hb_data->bms_pack_status->cmu_can_power_on = false;
    hb_data->bms_pack_status->bmu_setup_mode = false;

    hb_data->ui_status->rasp_pi_on = false;
} 

void process_input_heartbeat_data(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *hb_data, uint32_t msTicks) {
    if(input_messages->recieved_heartbeats->bms_heartbeat1) {
        hb_data->started_heartbeats->bms_heartbeat1 = true;
        hb_data->last_rcvd_bms_heartbeat1 = msTicks;
    }

    if(input_messages->recieved_heartbeats->bms_heartbeat2) {
        hb_data->started_heartbeats->bms_heartbeat2 = true;
        hb_data->last_rcvd_bms_heartbeat2 = msTicks;
        memcpy(hb_data->bms_precharge_status, input_messages->bms_precharge_status, sizeof(BMS_PRECHARGE_STATUS));
    }

    if(input_messages->recieved_heartbeats->bms_heartbeat3) {
        hb_data->started_heartbeats->bms_heartbeat3 = true;
        hb_data->last_rcvd_bms_heartbeat3 = msTicks;
        memcpy(hb_data->bms_pack_status, input_messages->bms_pack_status, sizeof(BMS_PACK_STATUS));
    }

    if(input_messages->recieved_heartbeats->ui_heartbeat) {
        hb_data->started_heartbeats->ui_heartbeat = true;
        hb_data->last_rcvd_ui_heartbeat = msTicks;
    }

    if(input_messages->recieved_heartbeats->throttle_heartbeat) {
        hb_data->started_heartbeats->throttle_heartbeat = true;
        hb_data->last_rcvd_throttle_heartbeat = msTicks;
        memcpy(hb_data->throttle_status, input_messages->throttle_status, sizeof(THROTTLE_STATUS));
    }

    if(input_messages->recieved_heartbeats->pdm_heartbeat) {
        hb_data->started_heartbeats->pdm_heartbeat = true;
        hb_data->last_rcvd_pdm_heartbeat = msTicks;
        memcpy(hb_data->pdm_status, input_messages->pdm_status, sizeof(PDM_STATUS));
    }

    if(input_messages->recieved_heartbeats->wv1_heartbeat) {
        hb_data->started_heartbeats->wv1_heartbeat = true;
        hb_data->last_rcvd_wv1_heartbeat = msTicks;
        memcpy(hb_data->wv1_status, input_messages->wv1_status, sizeof(WV_STATUS));
    }

    if(input_messages->recieved_heartbeats->wv2_heartbeat) {
        hb_data->started_heartbeats->wv2_heartbeat = true;
        hb_data->last_rcvd_wv2_heartbeat = msTicks;
        memcpy(hb_data->wv2_status, input_messages->wv2_status, sizeof(WV_STATUS));
    }

    if(input_messages->recieved_heartbeats->pdm_heartbeat) {
        hb_data->started_heartbeats->pdm_heartbeat = true;
        hb_data->last_rcvd_pdm_heartbeat = msTicks;
        memcpy(hb_data->pdm_status, input_messages->pdm_status, sizeof(PDM_STATUS));
    }

    if(input_messages->recieved_heartbeats->mi_heartbeat) {
        hb_data->started_heartbeats->mi_heartbeat = true;
        hb_data->last_rcvd_mi_heartbeat = msTicks;
    }
}

void convert_acc(ACCESSORIES_INPUT_STATE *acc_in, bool brakes_on, ACCESSORIES_OUTPUT_REQUEST *out_req) { 
    out_req->brake_lights_on = brakes_on;
    out_req->wipers_on = acc_in->wipers_on;
    out_req->turn_blinker = acc_in->turn_blinker_switches;
    out_req->headlight_state = acc_in->headlight_switches;	
}

void turn_all_acc_off(ACCESSORIES_OUTPUT_REQUEST *out_req) {
    out_req->brake_lights_on = false;
    out_req->wipers_on = false;
    out_req->turn_blinker = BLINKER_OFF;
    out_req->headlight_state = HEADLIGHT_OFF;
}


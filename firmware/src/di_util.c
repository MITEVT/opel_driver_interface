#include "types.h"
#include "chip.h"
#include "string.h"

// TODO: Add documentation for all functions

// TODO:
// write initialize_state()
// write initialize_output()
// write initialize_input()

static uint32_t bms_hb1_threshold; 
static uint32_t bms_hb2_threshold; 
static uint32_t bms_hb3_threshold; 
static uint32_t throttle_hb_threshold; 
static uint32_t wv1_hb_threshold; 
static uint32_t wv2_hb_threshold; 
static uint32_t pdm_hb_threshold; 
static uint32_t ui_hb_threshold; 
static uint32_t mi_hb_threshold; 


void Util_Config(Util_Config_T util_config){
    //TODO:
}


void change_mode(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request) {
    if(mode_request == REQ_OFF) {
        state->dsm_mode = MODE_SHUTDOWN;
        output->messages->di_packet->mode = OUT_SHUTDOWN_IMPENDING;
    } else if (mode_request == REQ_ACCESSORIES) {
        state->dsm_mode = MODE_ACCESSORIES;
        output->messages->di_packet->mode = OUT_PARKING;
    } else if (mode_request == REQ_CHARGE) {
        state->dsm_mode = MODE_CHARGE;
        output->messages->di_packet->mode = OUT_CHARGE;
    } else if (mode_request == REQ_DRIVE) {
        if(input->dcl == FORWARD) {
            output->messages->di_packet->mode = OUT_FORWARD;
            state->dsm_mode = MODE_DRIVE;
            state->direction = DRIVE_FORWARD;
        } else if (input->dcl == REVERSE) {
            output->messages->di_packet->mode = OUT_REVERSE;
            state->dsm_mode = MODE_DRIVE;
            state->direction = DRIVE_REVERSE;
        } else {
            return ERROR_INCONSISTENT_MODE_REQUEST;
        }
    } else if (mode_request == REQ_INIT) {
        return ERROR_INCONSISTENT_MODE_REQUEST;
    } else if (mode_request == REQ_SHUTDOWN) {
        return ERROR_INCONSISTENT_MODE_REQUEST;
    }
}


MODE_REQUEST get_mode_request(INPUT *input) {
    if(input->keymodes == KEYMODE_OFF) {
        return REQ_OFF;
    } else if (input->keymodes == KEYMODE_CHARGE) {
        return REQ_CHARGE;
    } else { //Keymode Drive
        if(input->dcl == PARK) {
            return REQ_ACCESSORIES;
        } else {
            return REQ_DRIVE;
        }
    }
}


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

    if(input_messages->recieved_heartbeats->ui_heartbeat) {
        hb_data->started_heartbeats->ui_heartbeat = true;
        hb_data->last_rcvd_ui_heartbeat = msTicks;
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

DI_ERROR no_heartbeat_errors(INPUT *input *state){


}

DI_ERROR all_hb_exist(STATE *state, uint32_t msTicks){
    RECIEVED_HEARTBEATS *started_hb = state->heartbeat_data->started_heartbeats;

    //Have we received all the heartbeats?
    
    bool rcvd_bms_hbs = (started_hb->bms_heartbeat1 && started_hb->bms_heartbeat2 && started_hb->bms_heartbeat);
    bool rcvd_wv_hbs = (started_hb->wv1_heartbeat && started_hb->wv2_heartbeat);
    bool rcvd_motor_hbs = (started_hb->throttle_heartbeat && started_hb->mi_heartbeat);
    bool rcvd_rest_hbs = (started_hb->pdm_heartbeat && started_hb->ui_heartbeat);

    bool rcvd_all_hbs = (rcvd_bms_hbs && rcvd_wv_hbs && rcvd_motor_hbs && rcvd_rest_hbs);

    HEARTBEAT_DATA *hb_data = state->heartbeat_hdata;
   
    uint32_t last_bms1 = hb_data->last_rcvd_bms_heartbeat1;
    uint32_t last_bms2 = hb_data->last_rcvd_bms_heartbeat2;
    uint32_t last_bms3 = hb_data->last_rcvd_bms_heartbeat3;
    uint32_t last_wv1 = hb_data->last_rcvd_wv1_heartbeat;
    uint32_t last_wv2 = hb_data->last_rcvd_wv2_heartbeat;
    uint32_t last_throt = hb_data->last_rcvd_throttle_heartbeat;
    uint32_t last_ui = hb_data->last_rcvd_ui_heartbeat;
    uint32_t last_mi = hb_data->last_rcvd_mi_heartbeat;
    uint32_t last_pdm = hb_data->last_rcvd_pdm_heartbeat;

    bool rcvd_wv1 = started_hb->wv1_heartbeat;
    bool rcvd_mi = started_hb->mi_heartbeat;
    bool rcvd_ui = started_hb->ui_heartbeat;
    bool timeout_wv1 = (msTicks - last_wv1 > wv1_hb_threshold);
    bool timeout_mi = (msTicks - last_mi > mi_hb_threshold);
    bool timeout_ui = (msTicks - last_ui > ui_hb_threshold); 
     
    bool timeout_bms_hbs = (msTicks - last_bms1 > bms_hb1_threshold && msTicks - last_bms2 > bms_hb2_threshold && msTicks - last_bms3 > bms_hb3_threshold);
    bool timeout_wv_hbs = (msTicks - last_wv1 > wv1_hb_threshold && msTicks - last_wv2 > wv2_hb_threshold);
    bool timeout_motor_hbs = (msTicks - last_throt > throttle_hb_threshold && msTicks - last_mi > mi_hb_threshold);
    bool timeout_rest_hbs = (msTicks - last_ui > ui_hb_threshold && msTicks - last_pdm > pdm_hb_threshold);

    bool didnt_timeout = (!timeout_bms_hbs && !timeout_wv_hbs && !timeout_motor_hbs && !timeout_rest_hbs);

    if(rcvd_all_hbs && didnt_timeout) {
        return ERROR_NONE;

    } else if(!rcvd_all_hbs) {
        if(!rcvd_bms_hbs){
            return ERROR_INIT_BMS_HEARTBEAT;
        } else if(!rcvd_wv_hbs) {
            return (!rdvd_wv1) ? ERROR_INIT_VELOCITY1_HEARTBEAT : ERROR_INIT_VELOCITY2_HEARTBEAT; 
        } else if(!rcvd_motor_hbs) {
            return (!rcvd_mi) ? ERROR_INIT_MI_HEARTBEAT : ERROR_INIT_THROTTLE_HEARTBEAT;
        } else if(!rcvd_rest_hbs) {
            return (!rcvd_ui) ? ERROR_INIT_UI_HEARTBEAT : ERROR_INIT_PDM_HEARTBEAT;
        }
    } else if(!didnt_timeout) {
        if(timeout_bms_hbs){
            return ERROR_LOST_BMS_HEARTBEAT;
        } else if(timeout_wv_hbs) {
            return (timeout_wv1) ? ERROR_LOST_VELOCITY1_HEARTBEAT : ERROR_LOST_VELOCITY2_HEARTBEAT;
        } else if(timeout_motor_hbs) {
            return (timeout_mi) ? ERROR_LOST_MI_HEARTBEAT : ERROR_LOST_THROTTLE_HEARTBEAT;
        } else if(timeout_rest_hbs) {
            return (timeout_ui) ? ERROR_LOST_UI_HEARTBEAT : ERROR_LOST_PDM_HEARTBEAT;
        }
    }
} 

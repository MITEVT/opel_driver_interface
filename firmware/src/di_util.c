#include "types.h"
#include "chip.h"
#include "di_util.h"
#include "string.h"

static uint32_t bms_hb1_threshold_ms; 
static uint32_t bms_hb2_threshold_ms; 
static uint32_t bms_hb3_threshold_ms; 
static uint32_t throttle_hb_threshold_ms; 
static uint32_t wv1_hb_threshold_ms; 
static uint32_t wv2_hb_threshold_ms; 
static uint32_t pdm_hb_threshold_ms; 
static uint32_t ui_hb_threshold_ms; 
static uint32_t mi_hb_threshold_ms; 

void initialize_state(STATE *state){
    state->dsm_mode = MODE_OFF;
    state->direction = DRIVE_FORWARD;
    
    HEARTBEAT_DATA *hb_data = state->heartbeat_data;
    initialize_heartbeat_data(hb_data);

    state->time_started_init_tests_ms = 0;
    state->time_started_close_contactors_request_ms = 0;
    state->time_started_PDM_test_ms = 0;
    state->critical_systems_relay_on = false;
    state->low_voltage_relay_on = false;
}

void initialize_output(OUTPUT *output){
    ACCESSORIES_OUTPUT_REQUEST *acc_out = output->acc_output;
    OUTPUT_MESSAGES *out_msgs = output->messages;
    
    turn_all_acc_off(acc_out);
    out_msgs->error = ERROR_NONE;
    
    DI_PACKET *di_pkt = out_msgs->di_packet;
    
    di_pkt->ignition = DI_NONE;
    di_pkt->mode = OUT_OFF;

    output->low_voltage_relay_on = false;
    output->critical_systems_relay_on = false;
}

void initialize_input(INPUT *input){
    ACCESSORIES_INPUT_STATE *acc_in = input->acc_input;
    INPUT_MESSAGES *inp_msgs = input->messages;    
    RECIEVED_HEARTBEATS *rcvd_hbs = inp_msgs->recieved_heartbeats;

    acc_in->wipers_on = false;
    acc_in->headlight_switches = HEADLIGHT_OFF;
    acc_in->turn_blinker_switches = BLINKER_OFF;

    rcvd_hbs->bms_heartbeat1 = false;
    rcvd_hbs->bms_heartbeat2 = false;
    rcvd_hbs->bms_heartbeat3 = false;
    rcvd_hbs->throttle_heartbeat = false;
    rcvd_hbs->wv1_heartbeat = false;
    rcvd_hbs->wv2_heartbeat = false;
    rcvd_hbs->pdm_heartbeat = false;
    rcvd_hbs->ui_heartbeat = false;
    rcvd_hbs->mi_heartbeat = false;

    input->keymodes = KEYMODE_OFF;
    input->dcl = PARK;
   
    inp_msgs->wv1_status->velocity = 0;
    inp_msgs->wv2_status->velocity = 0;

    inp_msgs->pdm_status->low_voltage_status=false;
    inp_msgs->pdm_status->low_voltage_dcdc=false;
    inp_msgs->pdm_status->low_voltage_battery=false;
    inp_msgs->pdm_status->critical_systems_status=false;
    inp_msgs->pdm_status->critical_systems_battery=false;
    inp_msgs->pdm_status->critical_systems_dcdc=false;

    inp_msgs->throttle_status->brake_value=0;
    inp_msgs->throttle_status->throttle_value=0;

    inp_msgs->bms_precharge_status->contactor_error[0] = false;
    inp_msgs->bms_precharge_status->contactor_output[0] = false;
    inp_msgs->bms_precharge_status->contactor_error[1] = false;
    inp_msgs->bms_precharge_status->contactor_output[1] = false;
    inp_msgs->bms_precharge_status->contactor_error[2] = false;
    inp_msgs->bms_precharge_status->contactor_output[2] = false;
    inp_msgs->bms_precharge_status->precharge_status = 0;

    inp_msgs->bms_pack_status->cells_over_voltage = false;
    inp_msgs->bms_pack_status->cells_under_voltage = false;
    inp_msgs->bms_pack_status->cells_over_temperature = false;
    inp_msgs->bms_pack_status->measurement_untrusted = false;
    inp_msgs->bms_pack_status->cmu_comm_timeout = false;
    inp_msgs->bms_pack_status->vehicle_comm_timeout = false;
    inp_msgs->bms_pack_status->cmu_can_power_off = false;
    inp_msgs->bms_pack_status->bmu_setup_mode = false;

    inp_msgs->ui_status->rasp_pi_on = false;
}

void Util_Config(Util_Config_T *util_config){
    bms_hb1_threshold_ms = util_config->bms_hb1_threshold_ms;
    bms_hb2_threshold_ms = util_config->bms_hb2_threshold_ms;
    bms_hb3_threshold_ms = util_config->bms_hb3_threshold_ms;
    throttle_hb_threshold_ms = util_config->throttle_hb_threshold_ms;
    wv1_hb_threshold_ms = util_config->wv1_hb_threshold_ms;
    wv2_hb_threshold_ms = util_config->wv2_hb_threshold_ms;
    pdm_hb_threshold_ms = util_config->pdm_hb_threshold_ms;
    ui_hb_threshold_ms = util_config->ui_hb_threshold_ms;
    mi_hb_threshold_ms = util_config->mi_hb_threshold_ms;
}


DI_ERROR change_mode(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request) {
    if(mode_request == REQ_OFF) {
        state->dsm_mode = MODE_SHUTDOWN;
        output->messages->di_packet->mode = OUT_SHUTDOWN_IMPENDING;
        return ERROR_NONE;
    } else if (mode_request == REQ_ACCESSORIES) {
        state->dsm_mode = MODE_ACCESSORIES;
        output->messages->di_packet->mode = OUT_PARKED;
        return ERROR_NONE;
    } else if (mode_request == REQ_CHARGE) {
        state->dsm_mode = MODE_CHARGE;
        output->messages->di_packet->mode = OUT_CHARGE;
        return ERROR_NONE;
    } else if (mode_request == REQ_DRIVE) {
        if(input->dcl == FORWARD) {
            output->messages->di_packet->mode = OUT_FORWARD;
            state->dsm_mode = MODE_DRIVE;
            state->direction = DRIVE_FORWARD;
            return ERROR_NONE;
        } else if (input->dcl == REVERSE) {
            output->messages->di_packet->mode = OUT_REVERSE;
            state->dsm_mode = MODE_DRIVE;
            state->direction = DRIVE_REVERSE;
            return ERROR_NONE;
        } else {
            return ERROR_INCONSISTENT_MODE_REQUEST;
        }
    } else if (mode_request == REQ_INIT) {
        return ERROR_INCONSISTENT_MODE_REQUEST;
    } else if (mode_request == REQ_SHUTDOWN) {
        return ERROR_INCONSISTENT_MODE_REQUEST;
    } else {
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
    hb_data->bms_pack_status->cmu_can_power_off = false;
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

DI_ERROR check_bms_precharge(STATE *state) {
   HEARTBEAT_DATA *hb_data = state->heartbeat_data;
   BMS_PRECHARGE_STATUS *s = hb_data->bms_precharge_status;
   if(s->contactor_error[0] || s->contactor_error[1] || s->contactor_error[2]) {
       return ERROR_BMS_PRECHARGE;
   } else if(s->precharge_status != 4) {
       return ERROR_BMS_PRECHARGE;
   }

   if(s->contactor_output[0] && s->contactor_output[1] && s->contactor_output[2]) {
        return ERROR_BMS_PRECHARGE;
   } else {
       return ERROR_NONE;
   }
}

DI_ERROR no_heartbeat_errors(STATE *state, bool check_pdm_cs) {
   HEARTBEAT_DATA *hb_data = state->heartbeat_data;
   UI_STATUS *ui_status = hb_data->ui_status;
   BMS_PACK_STATUS *bms_pack_status = hb_data->bms_pack_status;
   PDM_STATUS *pdm_status = hb_data->pdm_status;

   // TODO add range checks on these values based on results of group meeting
   // WV_STATUS *wv1_status = hb_data->wv1_status;
   // WV_STATUS *wv2_status = hb_data->wv2_status;
   // THROTTLE_STATUS *throttle_status = hb_data->throttle_status;

   if(!ui_status->rasp_pi_on) {
       return ERROR_INIT_UI_HEARTBEAT;
   } else if(bms_pack_status->cells_over_voltage) {
       return ERROR_CONTENT_BMS_OVER_VOLTAGE;
   } else if(bms_pack_status->cells_under_voltage) {
       return ERROR_CONTENT_BMS_UNDER_VOLTAGE;
   } else if(bms_pack_status->cells_over_temperature) {
        return ERROR_CONTENT_BMS_OVER_TEMP;
   } else if(bms_pack_status->measurement_untrusted) {
       return ERROR_CONTENT_BMS_MEASUREMENT_UNTRUSTED;
   } else if(bms_pack_status->cmu_comm_timeout) {
       return ERROR_CONTENT_BMS_CMU_COMM_TIMEOUT;
   } else if(bms_pack_status->vehicle_comm_timeout) {
       return ERROR_CONTENT_BMS_VEHICLE_COMM_TIMEOUT;
   } else if(bms_pack_status->cmu_can_power_off) {
       return ERROR_CONTENT_BMS_CAN_POWER;
   } else if(pdm_status->low_voltage_status) {
       if(pdm_status->low_voltage_dcdc) {
           return ERROR_LVS_DC_TEST_FAILED;
       } else {
           return ERROR_LVS_BATTERY_TEST_FAILED;
       }
   } 
   
   if(check_pdm_cs) {
       if(pdm_status->critical_systems_status) {
            if(pdm_status->critical_systems_dcdc) {
                return ERROR_CS_DC_TEST_FAILED;
            } else {
                return ERROR_CS_BATTERY_TEST_FAILED;
            }
       }
   }

   return ERROR_NONE;
}

DI_ERROR all_hb_exist(STATE *state, uint32_t msTicks) {
    HEARTBEAT_DATA *hb_data = state->heartbeat_data;
    RECIEVED_HEARTBEATS *sh = hb_data->started_heartbeats;

    // Have we received all the heartbeats?
    bool bms_hbs = sh->bms_heartbeat1 && sh->bms_heartbeat2 && sh->bms_heartbeat3;
    bool wv1_hb = sh->wv1_heartbeat;
    bool wv2_hb = sh->wv2_heartbeat;
    bool throttle_hb = sh->throttle_heartbeat;
    bool mi_hb = sh->mi_heartbeat;
    bool pdm_hb = sh->pdm_heartbeat;
    bool ui_hb = sh->ui_heartbeat;
     
    if(!bms_hbs) {
        return ERROR_INIT_BMS_HEARTBEAT;
    } else if (!wv1_hb) {
        return ERROR_INIT_VELOCITY1_HEARTBEAT;
    } else if (!wv2_hb) {
        return ERROR_INIT_VELOCITY2_HEARTBEAT;
    } else if (!throttle_hb) {
        return ERROR_INIT_THROTTLE_HEARTBEAT;
    } else if (!mi_hb) {
        return ERROR_INIT_MI_HEARTBEAT;
    } else if (!ui_hb) {
        return ERROR_INIT_UI_HEARTBEAT;
    } else if (!pdm_hb) {
        return ERROR_INIT_PDM_HEARTBEAT;
    }

    uint32_t last_bms1 = hb_data->last_rcvd_bms_heartbeat1;
    uint32_t last_bms2 = hb_data->last_rcvd_bms_heartbeat2;
    uint32_t last_bms3 = hb_data->last_rcvd_bms_heartbeat3;
    uint32_t last_wv1 = hb_data->last_rcvd_wv1_heartbeat;
    uint32_t last_wv2 = hb_data->last_rcvd_wv2_heartbeat;
    uint32_t last_throttle = hb_data->last_rcvd_throttle_heartbeat;
    uint32_t last_ui = hb_data->last_rcvd_ui_heartbeat;
    uint32_t last_mi = hb_data->last_rcvd_mi_heartbeat;
    uint32_t last_pdm = hb_data->last_rcvd_pdm_heartbeat;

    bool timeout_wv1 = (msTicks - last_wv1 > wv1_hb_threshold_ms);
    bool timeout_wv2 = (msTicks - last_wv2 > wv2_hb_threshold_ms);
    bool timeout_mi = (msTicks - last_mi > mi_hb_threshold_ms);
    bool timeout_ui = (msTicks - last_ui > ui_hb_threshold_ms); 
    bool timeout_throttle = (msTicks - last_throttle > throttle_hb_threshold_ms); 
    bool timeout_pdm = (msTicks - last_pdm > pdm_hb_threshold_ms); 
     
    bool timeout_bms_hb1 = msTicks - last_bms1 > bms_hb1_threshold_ms;
    bool timeout_bms_hb2 = msTicks - last_bms2 > bms_hb2_threshold_ms;
    bool timeout_bms_hb3 = msTicks - last_bms3 > bms_hb3_threshold_ms;
    
    if(timeout_bms_hb1 || timeout_bms_hb2 || timeout_bms_hb3) {
        return ERROR_LOST_BMS_HEARTBEAT;
    } else if(timeout_wv1) {
        return ERROR_LOST_VELOCITY1_HEARTBEAT;
    } else if(timeout_wv2) {
        return ERROR_LOST_VELOCITY2_HEARTBEAT;
    } else if(timeout_mi) {
        return ERROR_LOST_MI_HEARTBEAT;
    } else if(timeout_ui) {
        return ERROR_LOST_UI_HEARTBEAT;
    } else if(timeout_throttle) {
        return ERROR_LOST_THROTTLE_HEARTBEAT;
    } else if(timeout_pdm) {
        return ERROR_LOST_PDM_HEARTBEAT;
    }
    
    return ERROR_NONE;
} 

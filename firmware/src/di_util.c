#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include <stdlib.h>

void initialize_heartbeat_data(HEARTBEAT_DATA *hb_data) { 
    hb_data->time_since_bms_heartbeat = 0;
    hb_data->time_since_wv1_heartbeat = 0;
    hb_data->time_since_wv2_heartbeat = 0;
    hb_data->time_since_throttle_heartbeat = 0;
    hb_data->time_since_ui_heartbeat = 0;
    hb_data->time_since_mi_heartbeat = 0;
    hb_data->time_since_pdm_heartbeat = 0;
} 

void process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *hb_data, uint32_t msTicks) {
    hb_data->time_since_bms_heartbeat = (input_messages->bms_heartbeat) ? msTicks : hb_data->time_since_bms_heartbeat;
    hb_data->time_since_throttle_heartbeat = (input_messages->throttle_heartbeat) ? msTicks : hb_data->time_since_throttle_heartbeat;
    hb_data->time_since_wv1_heartbeat = (input_messages->wv1_heartbeat) ? msTicks : hb_data->time_since_wv1_heartbeat;
    hb_data->time_since_wv2_heartbeat = (input_messages->wv2_heartbeat) ? msTicks : hb_data->time_since_wv2_heartbeat;
    hb_data->time_since_pdm_heartbeat = (input_messages->pdm_heartbeat) ? msTicks : hb_data->time_since_pdm_heartbeat;
    hb_data->time_since_ui_heartbeat = (input_messages->ui_heartbeat) ? msTicks : hb_data->time_since_ui_heartbeat;
    hb_data->time_since_mi_heartbeat = (input_messages->mi_heartbeat) ? msTicks : hb_data->time_since_mi_heartbeat;
   
}
void convert_acc(ACCESSORIES_INPUT_STATE *acc_in, bool brakes_on, ACCESSORIES_OUTPUT_REQUEST *out_req) { 
    out_req->brake_lights_on = brakes_on;
    out_req->wipers_on = acc_in->wipers_on;
    out_req->turn_blinker = acc_in->turn_blinker_switches;
    out_req->headlight_state = acc_in->headlight_switches;	
}

void turn_all_off(ACCESSORIES_OUTPUT_REQUEST *out_req) {
    out_req->brake_lights_on = false;
    out_req->wipers_on = false;
    out_req->turn_blinker = BLINKER_OFF;
    out_req->headlight_state = HEADLIGHT_OFF;
}


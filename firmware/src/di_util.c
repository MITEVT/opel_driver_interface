#include <stdio.h>
#include <stdbool.h>

bool is_value_in_arr(uint8_t val, uint8_t *arr, uint8_t size) {
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

HEARTBEAT_DATA *initialize_heartbeat_data() { 
    HEARTBEAT_DATA *hb_data;
    hb_data->ignore_heartbeats = true;
    hb_data->time_since_BMS_heartbeat = 0;
    hb_data->time_since_throttle_heartbeat = 0;
    hb_data->time_since_PDM_heartbeat = 0;
    hb_data->time_since_velocity_heartbeat = 0;
    hb_data->velocity = 0;
    return hb_data;
} 

HEARTBEAT_DATA *process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *heartbeat_data, uint32_t msTicks) {
    HEARTBEAT_DATA *hb_data;
    
    hb_data->time_since_BMS_heartbeat = (input_messages->BMS_heartbeat) ? msTicks : heartbeat_data->time_since_BMS_heartbeat;
    hb_data->time_since_PDM_heartbeat = (input_messages->PDM_heartbeat) ? msTicks : heartbeat_data->time_since_PDM_heartbeat;
    hb_data->time_since_throttle_heartbeat = (input_messages->throttle_heartbeat) ? msTicks : heartbeat_data->time_since_throttle_heartbeat;
    hb_data->time_since_velocity_heartbeat = (input_messages->velocity != NULL) ? msTicks : heartbeat_data->time_since_velocity_heartbeat;
    hb_data->velocity = (input_messages->velocity != NULL) ? input_messages->velocity : heartbeat_data->velocity;

    return hb_data;
}

ACCESSORIES_OUTPUT_REQUEST_T *convert_acc(ACCESSORIES_INPUT_STATE_T *acc_in) {
    ACCESSORIES_OUTPUT_REQUEST_T out_req;
    out_req.wipers_on = acc_in->wipers_on;
    out_req.brake_lights_on = acc_in->brake_lights_on;
    out_req.turn_blinker = acc_in->turn_blinker_switches;
    out_req.headlight_state = acc_in->headlight_switches;	
    return &out_req;
}

ACCESSORIES_OUTPUT_REQUEST_T *turn_all_off() {
    ACCESSORIES_OUTPUT_REQUEST_T out_req;
    out_req.wipers_on = false;
    out_req.brake_lights_on = false;
    out_req.turn_blinker = BLINKER_OFF;
    out_req.headlight_state = HEADLIGHT_OFF;
    return &out_req;
}

